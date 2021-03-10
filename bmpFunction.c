#include "bmpFunction.h"

void downSample(unsigned char *inIm, unsigned char *outIm, int width, int height)
{
  //takes pointer of the input ımage then downSaple it by 2.
  int tmpSum; //beware it is not unsignedchar, otherwise the sum value would be truncated
  int tmpPos;  //tmpPos, upper left pixel of the image
  int factor = 2;
  int i,j;
  int W = width*3; //in pixels: For ex: if width 720, there is 2160value for one row (3*Width-RGB)
  int H = height;  


  // Until H/2, image rescaled by 2
  for (i = 0; i < H/2; i = i + 1)
  { //Until W/2, image rescaled by 2
    for (j = 0; j < W/2 ; j = j + 1)
    {
      tmpSum = 0;
      tmpPos = i *  W * factor; //factor is 2, image rescaled by 2. tmpPos for upperleft corner
      
      //could not generalize the position from left to right
      if(j%3==0) //if pixel component is R, then multiply by2 
        tmpPos += j*2;
      else if((j-1)%3==0) //if the pixel compenent is G
        tmpPos += (j-1)*2 + 1;
      else  //if the pixel component is B
        tmpPos += (j-2)*2 + 2;
      

      tmpSum = 0;
      //R Channel
      tmpSum += *(inIm +  tmpPos);                 //Upper Left
      tmpSum += *(inIm +  tmpPos + 3);             //Upper Right
      tmpSum += *(inIm +  tmpPos + 3 + W);         //Lower Right
      tmpSum += *(inIm +  tmpPos +     W);         //Lower Left

      *(outIm+i*W/2+j) = (unsigned char)(tmpSum/4);

    }
  }

}

void paddImage(unsigned char *inIm, unsigned char *outIm,int width,int height){
//padd the input image with 1pixel from every forder.
int i,j;
int pd = 2; //how many pixel are padded
int pW = width+pd;  // +pd/2 left/right 
int pH = height+pd; // +pd/2 up/down

//first initalize padded image to 0;
for(i=0;i<pW*pH*3;i++){
  *(outIm+i) = (unsigned char)0;
}
int x,tx;

//copy input image into padded image
for(i=0;i<height;i++){ 
  for(j=0;j<width*3;j++){
    x  = i*width*3+j;             //from pixel at position x to position tx.
    tx = (i+pd/2)*pW*3+(j+pd/2*3); //(i+pd/2)->(because we start from 1 pixel below), (pd/2*3)->(becasuse we start from 1pixel to left)
    *(outIm+tx) = *(inIm + x);
  }
}

}


double simpleConv(unsigned char *image,double kernel[3][3],int konum,int width){
  //simply calculates the convolution of symmetric kernel with given "konum-left upper corner" of ımage
  //width = width of the image in pixels
  double tmpVal = 0;

  tmpVal += (double)*(image+konum) * kernel[0][0];               //left upper
  tmpVal += (double)*(image+konum+3) * kernel[0][1];             //middle upper
  tmpVal += (double)*(image+konum+6) * kernel[0][2];             //right upper

  tmpVal += (double)*(image+(width+2)*3+konum) * kernel[1][0];       //left middle
  tmpVal += (double)*(image+(width+2)*3+konum+3) * kernel[1][1];     //middle middle
  tmpVal += (double)*(image+(width+2)*3+konum+6) * kernel[1][2];     //right middle

  tmpVal += (double)*(image+(width+2)*3*2+konum) * kernel[2][0];     //left lower
  tmpVal += (double)*(image+(width+2)*3*2+konum+3) * kernel[2][1];   //middle lower
  tmpVal += (double)*(image+(width+2)*3*2+konum+6) * kernel[2][2];   //right lower

  return tmpVal;
}

void smoothImage(unsigned char *padIm, unsigned char *outIm, int width, int height){

  int i,j;
  double tmpVal; //will hold the output value of convolution

  double conv2[3][3] = {
                        {0.0625,0.125,0.0625},
                        {0.125 ,0.25 ,0.125},
                        {0.0625,0.125,0.0625}}; //convolution kernel.


  
  int x;  //left upper corner of the padded image pixel's rgb component.
  int tx; //left uppder corner in the "SmoothedImage"

  for(i=0;i<height;i++){      //up to down
    for(j=0;j<width*3;j++){   //left to right, *3(RGB componenets)
    
      x = i*(width+2)*3 + j ;   //+2 beause of padding!

      tmpVal = simpleConv(padIm,conv2,x,width);

      tx = i*(width)*3 + j;
      *(outIm + tx) = (unsigned char)(tmpVal); //conv sonucu buraya gelecek. 

    }
  }


}