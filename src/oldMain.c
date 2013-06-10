/**
Implementa a movimentação simples de uma camera em primeira pessoa para um personagem que anda
sobre um plano.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cmath>
#include <iostream>
#include <GL/glut.h>

//openal (sound lib)
#include <AL/alut.h>
//#include <AL/al.h>


//bitmap class to load bitmaps for textures
#include "bitmap.h"

#pragma comment(lib, "OpenAL32.lib")
#pragma comment(lib, "alut.lib")

//#include <mmsystem.h>
//#pragma comment(lib, "winmm.lib")

#define PI 3.14159265

// sound stuff
#define NUM_BUFFERS 1
#define NUM_SOURCES 1
#define NUM_ENVIRONMENTS 1

#define FRAMEMS 20
#define RUNSPEED 2.0
#define CROUCHSPEED 0.8
#define FLOOR 0.0
#define FRICTION 0.99
#define FLOORFRICTION 0.7
#define BOUNCE 0.8
#define JUMPSTR 0.1
#define GRAVITY 0.01
#define NORMALHEIGHT 0.2
#define CROUCHHEIGHT 0.12

typedef struct s_rect
{
	double x,y,z;
	double w,h,d;


}t_rect;

void mainInit();
void initSound();
void initTexture();
void createGLUI();
void mainRender();
void mainCreateMenu();
void onMouseButton(int button, int state, int x, int y);
void onMouseMove(int x, int y);
void onKeyDown(unsigned char key, int x, int y);
void onKeyUp(unsigned char key, int x, int y);
void onGLUIEvent(int id);
void onWindowReshape(int x, int y);
void mainIdle();
int main(int argc, char **argv);
void setWindow();
void setViewport(GLint left, GLint right, GLint bottom, GLint top);
void updateState();
void renderFloor();
void updateCam();

double speed = 1.0;

/**
Screen dimensions
*/
int windowWidth = 600;
int windowHeight = 480;

/**
Screen position
*/
int windowXPos = 100;
int windowYPos = 150;

int mainWindowId = 0;

double xOffset = -1.9;
double yOffset = -1.3;
int mouseLastX = 0;
int mouseLastY = 0;

float roty = 0.0f;
float rotx = 90.0f;

bool rightPressed = false;
bool leftPressed = false;
bool upPressed = false;
bool downPressed = false;

bool spacePressed = false;

float speedX = 0.0f;
float speedY = 0.0f;
float speedZ = 0.0f;

float posX = 0.0f;
float posY = 0.4f;
float posZ = 1.0f;

double gravity = GRAVITY;
double height = NORMALHEIGHT;

/*
variavel auxiliar pra dar variação na altura do ponto de vista ao andar.
*/
float headPosAux = 0.0f;

float maxSpeed = 0.25f;

float planeSize = 4.0f;

// more sound stuff (position, speed and orientation of the listener)
ALfloat listenerPos[]={0.0,0.0,4.0};
ALfloat listenerVel[]={0.0,0.0,0.0};
ALfloat listenerOri[]={0.0,0.0,1.0,
						0.0,1.0,0.0};

// now the position and speed of the sound source
ALfloat source0Pos[]={ -2.0, 0.0, 0.0};
ALfloat source0Vel[]={ 0.0, 0.0, 0.0};

// buffers for openal stuff
ALuint  buffer[NUM_BUFFERS];
ALuint  source[NUM_SOURCES];
ALuint  environment[NUM_ENVIRONMENTS];
ALsizei size,freq;
ALenum  format;
ALvoid  *data;



// parte de código extraído de "texture.c" por Michael Sweet (OpenGL SuperBible)
// texture buffers and stuff
int i;					   /* Looping var */
BITMAPINFO	*info;		   /* Bitmap information */
GLubyte		*bits;		   /* Bitmap RGB pixels */
GLubyte	 *ptr;			/* Pointer into bit buffer */
GLubyte		*rgba;		   /* RGBA pixel buffer */
GLubyte		*rgbaptr;		/* Pointer into RGBA buffer */
GLubyte	 temp;			/* Swapping variable */
GLenum	  type;			/* Texture type */
GLuint	  texture;		 /* Texture object */

bool jumping = false;
bool crouched = false;

void jump()
{
	speedY = JUMPSTR;
	speedX += JUMPSTR*speedX;
	speedZ += JUMPSTR*speedZ;
	height = CROUCHHEIGHT;
	jumping = true;
}

/**
  * simula a fisica do personagem
  */
void simulatePhysics(double gravity,float speed[3], float pos[3] )
{

	int i;
	for(i=0 ; i<3 ; i++)
	{
		pos[i] += speed[i];
		if(jumping)
			speed[i]*=FRICTION;
		else
			speed[i]*=FLOORFRICTION;
	}

	if(pos[1] < FLOOR+height)
	{
		if(jumping)
		{
			jumping = false;
			height = NORMALHEIGHT;
		}
		//speed[1] *=-1*FRICTION*BOUNCE;
		speed[1]=0;
		pos[1] += 0.5*((FLOOR + height) - pos[1]);

	}
	else
	{
		speed[1] -= gravity;
	}
	double area = planeSize*0.95;
	if(pos[0] < -area)
		pos[0] = -area;
	else if(pos[0] > area)
		pos[0] = area;

	if(pos[2] < -area)
		pos[2] = -area;
	else if(pos[2] > area)
		pos[2] = area;

}

void timerF(int value)
{
	float speed[3] = {speedX, speedY,speedZ};
	float pos[3] = {posX,posY,posZ};
	simulatePhysics(gravity,speed,pos);
	speedX = speed[0];
	speedY = speed[1];
	speedZ = speed[2];

	posX = pos[0];
	posY = pos[1];
	posZ = pos[2];

	//printf("POS  : (%09.6f,%09.6f,%09.6f)\n",pos[0],pos[1],pos[2]);
	//printf("SPEED: (%09.6f,%09.6f,%09.6f)\n",speed[0],speed[1],speed[2]);

	glutPostRedisplay();
	glutTimerFunc(FRAMEMS,timerF,0);
}



void setWindow() {
	//roty = 0.0f;
	//rotx = 90.0f;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f,(GLfloat)windowWidth/(GLfloat)windowHeight,0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(posX,posY + 0.025 * std::abs(sin(headPosAux*PI/180)),posZ,
		posX + sin(roty*PI/180),posY + 0.025 * std::abs(sin(headPosAux*PI/180)) + cos(rotx*PI/180),posZ -cos(roty*PI/180),
		0.0,1.0,0.0);
}

/**
Atualiza a posição e orientação da camera
*/
void updateCam() {
	gluLookAt(posX,posY + 0.025 * std::abs(sin(headPosAux*PI/180)),posZ,
		posX + sin(roty*PI/180),posY + 0.025 * std::abs(sin(headPosAux*PI/180)) + cos(rotx*PI/180),posZ -cos(roty*PI/180),
		0.0,1.0,0.0);
/*
	listenerPos[0] = posX;
	listenerPos[1] = posY;
	listenerPos[2] = posZ;

	source0Pos[0] = posX;
	source0Pos[1] = posY;
	source0Pos[2] = posZ;
	*/
}

void setViewport(GLint left, GLint right, GLint bottom, GLint top) {
	glViewport(left, bottom, right - left, top - bottom);
}


/**
Initialize
*/
void mainInit() {
	glClearColor(1.0,1.0,1.0,0.0);
	glColor3f(0.0f,0.0f,0.0f);
	setWindow();
	setViewport(0, windowWidth, 0, windowHeight);

	initSound();

	initTexture();

	printf("w - andar \n");
	printf("a - esquerda \n");
	printf("d - direita \n");

}

/**
Initialize openal and check for errors
*/
void initSound() {
//#if 0
	printf("Initializing OpenAl \n");

	// Init openAL
	alutInit(0, NULL);

	alGetError(); // clear any error messages

	// Generate buffers, or else no sound will happen!
	alGenBuffers(NUM_BUFFERS, buffer);

	if(alGetError() != AL_NO_ERROR)
	{
		printf("- Error creating buffers !!\n");
		exit(1);
	}
	else
	{
		printf("init() - No errors yet.\n");
	}

	ALboolean booleano;
	booleano = false;
	alutLoadWAVFile((signed char *)"res/Footsteps.wav",&format,&data,&size,&freq,&booleano);
	alBufferData(buffer[0],format,data,size,freq);
	//alutUnloadWAV(format,data,size,freq);

	alGetError(); /* clear error */
	alGenSources(NUM_SOURCES, source);

	if(alGetError() != AL_NO_ERROR)
	{
		printf("- Error creating sources !!\n");
		exit(2);
	}
	else
	{
		printf("init - no errors after alGenSources\n");
	}

	listenerPos[0] = posX;
	listenerPos[1] = posY;
	listenerPos[2] = posZ;

	source0Pos[0] = posX;
	source0Pos[1] = posY;
	source0Pos[2] = posZ;

	alListenerfv(AL_POSITION,listenerPos);
	alListenerfv(AL_VELOCITY,listenerVel);
	alListenerfv(AL_ORIENTATION,listenerOri);

	alSourcef(source[0], AL_PITCH, 1.0f);
	alSourcef(source[0], AL_GAIN, 1.0f);
	alSourcefv(source[0], AL_POSITION, source0Pos);
	alSourcefv(source[0], AL_VELOCITY, source0Vel);
	alSourcei(source[0], AL_BUFFER,buffer[0]);
	alSourcei(source[0], AL_LOOPING, AL_TRUE);
//#endif
	printf("Sound ok! \n\n");
}

/**
Initialize the texture
*/
void initTexture(void)
{
	printf ("\nLoading texture..\n");
	// Load a texture object (256x256 true color)
	bits = LoadDIBitmap("res/tiledbronze.bmp", &info);
	if (bits == (GLubyte *)0) {
		printf ("Error loading texture!\n\n");
		return;
	}


	// Figure out the type of texture
	if (info->bmiHeader.biHeight == 1)
	  type = GL_TEXTURE_1D;
	else
	  type = GL_TEXTURE_2D;

	// Create and bind a texture object
	glGenTextures(1, &texture);
	glBindTexture(type, texture);

	// Set texture parameters
	glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Create an RGBA image
	rgba = (GLubyte *)malloc(info->bmiHeader.biWidth * info->bmiHeader.biHeight * 4);

	i = info->bmiHeader.biWidth * info->bmiHeader.biHeight;
	for( rgbaptr = rgba, ptr = bits;  i > 0; i--, rgbaptr += 4, ptr += 3)
	{
			rgbaptr[0] = ptr[2];	 // windows BMP = BGR
			rgbaptr[1] = ptr[1];
			rgbaptr[2] = ptr[0];
			rgbaptr[3] = (ptr[0] + ptr[1] + ptr[2]) / 3;
	}

	glTexImage2D(type, 0, 4, info->bmiHeader.biWidth, info->bmiHeader.biHeight,
				  0, GL_RGBA, GL_UNSIGNED_BYTE, rgba );

	printf("Textura %d\n", texture);
	printf("Textures ok.\n\n");
}

void renderFloor(t_rect region) {

	// i want some nice, smooth, antialiased lines
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);

	// set things up to render the floor with the texture
	glShadeModel(GL_FLAT);
	glEnable (type);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	glPushMatrix();

	float textureScaleX = 10.0;
	float textureScaleY = 10.0;
	glColor4f(0.4f,0.4f,0.4f,1.0f);
	glBegin(GL_QUADS);
		glTexCoord2f(textureScaleX, 0.0f);   // coords for the texture
		glVertex3f(region.x,region.y + region.h,region.z + region.d);

		glTexCoord2f(0.0f, 0.0f);  // coords for the texture
		glVertex3f(region.x + region.w, region.y + region.h,region.z);

		glTexCoord2f(0.0f, textureScaleY);  // coords for the texture
		glVertex3f(region.x + region.w,region.y,region.z);

		glTexCoord2f(textureScaleX, textureScaleY);  // coords for the texture
		glVertex3f(region.x, region.y, region.z + region.d);
	glEnd();
	/*
	printf("#########\n");
	printf("(%lf,%lf,%lf)\n",region.x,region.y + region.h,region.z + region.d);
	printf("(%lf,%lf,%lf)\n",region.x + region.w,region.y + region.h,region.z);
	printf("(%lf,%lf,%lf)\n",region.x + region.w,region.y,region.z);
	printf("(%lf,%lf,%lf)\n",region.x,region.y,region.z + region.d);
	printf("#########\n");
	*/
	glDisable(type);
	glDisable(GL_BLEND);

	glColor4f(0.8f,0.8f,0.8f,1.0f);
	glBegin(GL_LINES);
	for (int i = 0; i <= 10; i++) {
		glVertex3f(-planeSize, 0.0f, -planeSize + i*(2*planeSize)/10.0f);
		glVertex3f(planeSize, 0.0f, -planeSize + i*(2*planeSize)/10.0f);
	}
	for (int i = 0; i <= 10; i++) {
		glVertex3f(-planeSize + i*(2*planeSize)/10.0f, 0.0f, -planeSize);
		glVertex3f(-planeSize + i*(2*planeSize)/10.0f, 0.0f, planeSize);
	}
	glEnd();
	glDisable(GL_LINE_SMOOTH);
	glPopMatrix();
}

void buildLeftWall(t_rect *region)
{
	region->x = -planeSize;
	region->y = 0;
	region->z = -planeSize;
	region->w = 2*planeSize;
	region->h = planeSize/3;
	region->d = 0;
}

void buildRightWall(t_rect *region)
{
	region->x = -planeSize;
	region->y = 0;
	region->z = planeSize;
	region->w = 2*planeSize;
	region->h = planeSize/3;
	region->d = 0;
}

void buildFrontWall(t_rect *region)
{
	region->x = planeSize;
	region->y = 0;
	region->z = -planeSize;
	region->w = 0;
	region->h = planeSize/3;
	region->d = 2*planeSize;
}

void buildBackWall(t_rect *region)
{
	region->x = -planeSize;
	region->y = 0;
	region->z = -planeSize;
	region->w = 0;
	region->h = planeSize/3;
	region->d = 2*planeSize;
}

void buildFloor(t_rect *region)
{
	region->x = -planeSize;
	region->y = 0;
	region->z = -planeSize;
	region->w = 2*planeSize;
	region->h = 0;
	region->d = 2*planeSize;
}

void renderScene() {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	updateCam();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	t_rect region;

	buildFrontWall(&region); renderFloor(region);
	buildBackWall(&region);  renderFloor(region);
	buildLeftWall(&region);  renderFloor(region);
	buildRightWall(&region); renderFloor(region);
	//buildFloor(&region);     renderFloor(region);

}

void updateState() {

	if (leftPressed) {
		roty -= 4.0f;
	}

	if (rightPressed) {
		roty += 4.0f;
	}

	if(!jumping)
	{
		if (upPressed)
		{

			speedX = (0.025 * sin(roty*PI/180))*speed;
			speedZ = (-0.025 * cos(roty*PI/180))*speed;;

			// efeito de "sobe e desce" ao andar
			headPosAux += 7.0f;
			if (headPosAux > 180.0f) {
				headPosAux = 0.0f;
			}

			posX += speedX;
			posZ += speedZ;

		}
		else if(downPressed)
		{
			speedX = -(0.025 * sin(roty*PI/180))*speed;
			speedZ = -(-0.025 * cos(roty*PI/180))*speed;;

			// efeito de "sobe e desce" ao andar
			headPosAux += 7.0f;
			if (headPosAux > 180.0f) {
				headPosAux = 0.0f;
			}

			posX += speedX;
			posZ += speedZ;
		}
		else
		{
			// parou de andar, para com o efeito de "sobe e desce"
			headPosAux = fmod(headPosAux, 90) - 1 * headPosAux / 90;
			headPosAux -= 4.0f;
			if (headPosAux < 0.0f)
				headPosAux = 0.0f;

		}
	}


}

/**
Render scene
*/
void mainRender() {
	updateState();
	renderScene();
	glFlush();
}

/**
Handles events from the mouse right button menu
*/
void mainHandleMouseRightButtonMenuEvent(int option) {
	switch (option) {
		case 1 :
			exit(0);
			break;
		default:
			break;
	}
}

/**
Create mouse button menu
*/
void mainCreateMenu() {
	glutCreateMenu(mainHandleMouseRightButtonMenuEvent);
	glutAddMenuEntry("Quit", 1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

/**
Mouse button event handler
*/
void onMouseButton(int button, int state, int x, int y) {

}

/**
Mouse move while button pressed event handler
*/
void onMouseMove(int x, int y) {

	mouseLastX = x;
	mouseLastY = y;

	if(x != windowWidth/2 || y != windowHeight/2)
		glutWarpPointer(windowWidth/2,windowHeight/2);
}

/**
Mouse move with no button pressed event handler
*/
void onMousePassiveMove(int x, int y) {

	if(x != windowWidth/2 || y != windowHeight/2)
	{
		//verifica a variacao em x
		int dx = x - mouseLastX;
		roty += (dx/6.0);

		int dy = y - mouseLastY;
		rotx += (dy/6.0);
		if(rotx > 180)
			rotx=180;
		else if (rotx < 0)
			rotx = 0;


		glutWarpPointer(windowWidth/2,windowHeight/2);
	}
	mouseLastX = x;
	mouseLastY = y;
}

/**
Key press event handler
*/

void onSpecial(int key, int x, int y)
{

	//printf("key:%d\n",key);

}

void onKeyDown(unsigned char key, int x, int y) {
	//se o shift estiver ativo o personagem estará correndo
	if(glutGetModifiers() == GLUT_ACTIVE_SHIFT)
	   speed = 2.0;
	switch (key) {
		case ' ' : //space
			spacePressed = true;
			if(!jumping)
				jump();
			jumping = true;
			break;
		//diminui a gravidade
		case 't':
			gravity -= 0.0001;
			break;
		//aumenta a gravidade
		case 'g':
			gravity += 0.0001;
			break;
		//se agacha
		case 'c':
		case 'C':
			crouched = true;
			height = CROUCHHEIGHT;
			break;
		case 'E':
		case 'e':
			if(!crouched)
				speed = RUNSPEED;
			break;
		case 'w': //w
			speed = 1;
			if (!upPressed) {
				//alSourcePlay(source[0]);
				//PlaySound((LPCSTR) "..\\..\\Footsteps.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
			}
			upPressed = true;
			break;
		case 'S':
		case 115: //s
			downPressed = true;
			break;
		case 'A':
		case 'a': //a
			leftPressed = true;
			break;
		case 'D':
		case 'd': //d
			rightPressed = true;
			break;

		default:
			break;
	}
	//printf("Key down:%d\t%c\n",(int)key,key);
	//glutPostRedisplay();
}

/**
Key release event handler
*/
void onKeyUp(unsigned char key, int x, int y) {
	//se o shift nao estiver ativo o personagem estará caminhando
	switch (key) {
		case 32: //space
			spacePressed = false;
			//jump();
			break;
		case 'E':
		case 'e':
			speed = 1;
			break;
		case 'c':
			if(crouched)
			{
				crouched = false;
				height = NORMALHEIGHT;
			}
			break;
		case 'W':
		case 119: //w
			if (upPressed) {
				//alSourceStop(source[0]);
				//PlaySound(NULL, 0, 0);
			}
			upPressed = false;
			break;
		case 'S':
		case 115: //s
			downPressed = false;
			break;
		case 'A':
		case 97: //a
			leftPressed = false;
			break;
		case 'D':
		case 100: //d
			rightPressed = false;
			break;
		case 27:
			exit(0);
			break;
		default:
			break;
	}
	//printf("Key up:%d\t%c\n",(int)key,key);
	//glutPostRedisplay();
}

void onWindowReshape(int x, int y) {
	windowWidth = x;
	windowHeight = y;
	setWindow();
	setViewport(0, windowWidth, 0, windowHeight);
}

/**
Glut idle funtion
*/
void mainIdle() {
	/**
	Set the active window before send an glutPostRedisplay call
	so it wont be accidently sent to the glui window
	*/
	glutSetWindow(mainWindowId);
	glutPostRedisplay();
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(windowWidth,windowHeight);
	glutInitWindowPosition(windowXPos,windowYPos);

	/**
	Store main window id so that glui can send it redisplay events
	*/
	mainWindowId = glutCreateWindow("FPS");

	glutDisplayFunc(mainRender);

	glutReshapeFunc(onWindowReshape);

	glutTimerFunc(FRAMEMS,timerF,0);

	/**
	Register mouse events handlers
	*/
	glutMouseFunc(onMouseButton);
	glutMotionFunc(onMouseMove);
	glutPassiveMotionFunc(onMousePassiveMove);

	/**
	Register keyboard events handlers
	*/
	glutKeyboardFunc(onKeyDown);
	glutSpecialFunc(onSpecial);
	glutKeyboardUpFunc(onKeyUp);

	mainInit();

	/**
	Create GLUT mouse button menus
	*/
	//mainCreateMenu();

	glutMainLoop();

	return 0;
}
