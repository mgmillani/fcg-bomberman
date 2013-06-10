#ifndef DEBUG_H
#define DEBUG_H
#include <stdio.h>

#define FERR(...) {fprintf(stderr,"FUNCTION:%s\tLINE:%d\t",__FUNCTION__,__LINE__); fprintf(stderr,__VA_ARGS__);fprintf(stderr,"\n");}
#define ERR(...) fprintf(stderr,__VA_ARGS__);

#endif
