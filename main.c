#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <math.h>
#include <stdint.h>
#include <string.h>

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
    unsigned char *img = (unsigned char *)malloc(3*WIDTH*HEIGHT);
    memset(img,0,3*WIDTH*HEIGHT);
    
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            int val = grey_value(j,i,WIDTH,HEIGHT);
            int idx = 3*(i*WIDTH + j);
            img[idx] = val;
            img[idx+1] = val;
            img[idx+2] = val;
        }
    }

    FILE *f = fopen(argv[3],"wb");
    
    unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0};
    unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0};
    unsigned char bmppad[3] = {0,0,0};

    int filesize = 54 + 3*WIDTH*HEIGHT;

    bmpfileheader[ 2] = (unsigned char)(filesize    );
    bmpfileheader[ 3] = (unsigned char)(filesize>> 8);
    bmpfileheader[ 4] = (unsigned char)(filesize>>16);
    bmpfileheader[ 5] = (unsigned char)(filesize>>24);

    bmpinfoheader[ 4] = (unsigned char)(       WIDTH    );
    bmpinfoheader[ 5] = (unsigned char)(       WIDTH>> 8);
    bmpinfoheader[ 6] = (unsigned char)(       WIDTH>>16);
    bmpinfoheader[ 7] = (unsigned char)(       HEIGHT>>24);
    bmpinfoheader[ 8] = (unsigned char)(       HEIGHT    );
    bmpinfoheader[ 9] = (unsigned char)(       HEIGHT>> 8);
    bmpinfoheader[10] = (unsigned char)(       HEIGHT>>16);
    bmpinfoheader[11] = (unsigned char)(       HEIGHT>>24);
    fwrite(bmpfileheader,1,14,f);
    fwrite(bmpinfoheader,1,40,f);

    for(int i=0; i<HEIGHT; i++)
    {
    fwrite(img+(WIDTH*(HEIGHT-i-1)*3),3,WIDTH,f);
    fwrite(bmppad,1,(4-(WIDTH*3)%4)%4,f);
    }
    fclose(f);
    free(img);
    return 0;
}
