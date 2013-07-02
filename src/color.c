#include "color.h"

#include "debug.h"

/**
  * compara duas cores
  * retorna a-b
  */
int colorCompare(const unsigned char *a,const unsigned char *b,int depth)
{
	int i;
	int diff=0;

	/*for(i=0 ; i< depth && diff==0 ; i++)
		ERR("A[%d] = %d\n",i,(int)a[i]);
	for(i=0 ; i< depth && diff==0 ; i++)
		ERR("B[%d] = %d\n",i,(int)b[i]);
	ERR("#######\n");*/
	for(i=0 ; i< depth && diff==0 ; i++)
	{
		diff = a[i] - b[i];
	}
	return diff;
}
