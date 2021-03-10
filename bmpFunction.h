#include "bitmap.h"
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>


void downSample(unsigned char *inIm, unsigned char *outIm, int width, int height);

void paddImage(unsigned char *inIm, unsigned char *outIm,int width,int height);

double simpleConv(unsigned char *image,double kernel[3][3],int konum,int width);

void smoothImage(unsigned char *padIm, unsigned char *outIm, int width, int height);