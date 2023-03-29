#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <math.h>
#include <stdint.h>

typedef uint16_t WORD;
typedef uint32_t DWORD;
typedef int32_t LONG;

typedef struct tagBITMAPFILEHEADER {
  WORD  bfType;
  DWORD bfSize;
  WORD  bfReserved1;
  WORD  bfReserved2;
  DWORD bfOffBits;
} BITMAPFILEHEADER, *LPBITMAPFILEHEADER, *PBITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER {
  DWORD biSize;
  LONG  biWidth;
  LONG  biHeight;
  WORD  biPlanes;
  WORD  biBitCount;
  DWORD biCompression;
  DWORD biSizeImage;
  LONG  biXPelsPerMeter;
  LONG  biYPelsPerMeter;
  DWORD biClrUsed;
  DWORD biClrImportant;
} BITMAPINFOHEADER, *PBITMAPINFOHEADER;


float map_mandel(int value, int in_min, int in_max, int out_min, int out_max)
{
    return 1.0*(value-in_min)*(out_max-out_min)/(1.0*(in_max-in_min))+1.0*out_min;
}

int grey_value(int x, int y, int width,int height){
    double complex c = map_mandel(x,0,width,-2,1) + map_mandel(y,0,height,-1,1) * I;
    double complex z = 0.0 + 0.0 * I;
    int i = 0;
    int repeats = 0;
    while (sqrt(creal(z)*creal(z) + cimag(z)*cimag(z)) <= 2.0 && repeats < 255)
    {
        i++;
        repeats++;
        z = z*z + c;
    }
    return i;
}
int main(int argc, char const *argv[])
{
    if(argc != 4)
    {
        printf("Expected 3 arguments (width, height, filename) but got %d",argc-1);
        exit(1);
    }
    int WIDTH = atoi(argv[1]);
    int HEIGHT = atoi(argv[2]);

    int *pixel_values = (int *)malloc(WIDTH*HEIGHT*sizeof(int));
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            int val = grey_value(j,i,WIDTH,HEIGHT);
            printf("%3d ",val);
            pixel_values[i*WIDTH+j] = val;
        }
        printf("\n");
        
    }
    free(pixel_values);
    return 0;
}
