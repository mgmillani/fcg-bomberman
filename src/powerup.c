#include <stdlib.h>

#include "powerup.h"
#include "build.h"
#include "draw.h"

/**
  * cria um novo powerup com uma caixa de tamanho size
  * se o ponteiro passado for NULL, aloca um novo powerup
  */
t_powerup *powerupInit(t_powerup *pwup,double size,GLuint border,GLuint image)
{
	if(pwup == NULL)
		pwup = malloc(sizeof(*pwup));

	pwup->border = border;
	pwup->image = image;

	buildBox(pwup->frameBox,size*0.1,size*1.5,size*1.5);
	buildBox(pwup->imageBox,size,size,size);

	return pwup;
}

/**
  * desenha um powerup
  */
void drawPowerup(t_powerup *pwup)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,pwup->border);
	unsigned int i;
	for(i=0 ; i<6 ; i++)
		drawRectangle(pwup->frameBox+i,1.0,1.0);

	glBindTexture(GL_TEXTURE_2D,pwup->image);
	for(i=0 ; i<6 ; i++)
		drawRectangle(pwup->imageBox+i,1.0,1.0);

}
