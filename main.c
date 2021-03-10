#include "bmpFunction.c"

int main(int argc, char *argv[]){

  FILE *inFile; //read file pointer

  BITMAPFILEHEADER bitmapFileHeader; //bitmap file header
  BITMAPINFOHEADER bitmapInfoHeader;
  unsigned char *inputImage;

  int width, height; //width and height of input image.


  inFile = fopen(argv[1],"rb");

  if(inFile == NULL)
    {
    printf("Cannot open the file!");
    return 0;
    }

  fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER),1,inFile); //read bmp file header

  fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER),1,inFile); //read bmp info header


  width = bitmapInfoHeader.biWidth;  //assign width and height value from InfoHeader
  height = bitmapInfoHeader.biHeight;

  int imSize = bitmapFileHeader.bfSize-sizeof(bitmapInfoHeader)-sizeof(bitmapFileHeader); //Image size in bytes

  inputImage = (unsigned char*)malloc(imSize); //allocate memory for input image

  if(!inputImage){ //check for memory allocation.
    printf("inputImage cannot created!\n");
    fclose(inFile);
    return 0;
  }

  fread(inputImage,imSize,1,inFile); //read from file into inputImage
  
  fclose(inFile);//close the file.
  
  //down scaled part
  BITMAPFILEHEADER outFileHeader = bitmapFileHeader; //create new file and info header for output image
  BITMAPINFOHEADER outInfoHeader = bitmapInfoHeader;

  outFileHeader.bfSize = imSize/4 + sizeof(bitmapInfoHeader) + sizeof(bitmapFileHeader); //update bitmapfile size, pixels numbers decreased by 4

  outInfoHeader.biWidth  = width/2;     //width and height of downsapled image
  outInfoHeader.biHeight = height/2;


  unsigned char *downSampledIm;         //downsapledimage
  downSampledIm = (unsigned char*)malloc(imSize/4); //allocating memory for downsampledimage

  downSample(inputImage, downSampledIm, width, height);   //downsampling

  FILE *outFile;            
  outFile = fopen("itu-downscaled.bmp","wb");

  fwrite(&outFileHeader,sizeof(BITMAPFILEHEADER),1,outFile);
  fwrite(&outInfoHeader,sizeof(BITMAPINFOHEADER),1,outFile);
  fwrite(downSampledIm,imSize/4,1,outFile);
  fclose(outFile);
  
  free(downSampledIm);



  //smoothing part
  
  //first allocate memory paddedImage
  int pH = height +2; //padded height
  int pW = width  +2;  //padded width

  int paddedSize = pH * pW * 3;  //size of padded image in bytes. (each pixel components(RGB) is 1 byte)
  unsigned char* paddedImage;    //padded image

  paddedImage = (unsigned char*)malloc(paddedSize); //allocate memory for padded Image
  
  
  paddImage(inputImage, paddedImage,width,height); //padd image
  
  unsigned char* smoothedImage;
  smoothedImage  = (unsigned char*)malloc(imSize); //allocate memory for smoothed image

  smoothImage(paddedImage,smoothedImage,width,height); 

  //save smoothed image!
  outFile = fopen("itu-smoothed.bmp","wb");

  fwrite(&bitmapFileHeader,sizeof(BITMAPFILEHEADER),1,outFile);
  fwrite(&bitmapInfoHeader,sizeof(BITMAPINFOHEADER),1,outFile);
  fwrite(smoothedImage,imSize,1,outFile);

  //close file and free allocated memory
  fclose(outFile);

  free(smoothedImage);
  free(paddedImage);



  printf("The image precessing is done!!\n");

  return 0;
}
