#include <stdio.h>
#include "faceswap_func.h"


void hello() {
	printf("Hello World !\n");
}

void addition(int a, int b) {
	int res;
	res = a + b;
	printf("addition : %d + %d = %d\n", a, b, res);
}

// Define functions here

/*void FindAllHomography(int n_quadrangles, int** Q_coords_src, int ** Q_coords_dst, int ** H){
    for(int i = 0; i < n_quadrangles; i++){
        Find_Homography(Q_coords_src[i], Q_coords_dst[i], H[i]);
    }
}

int ApplyAllHomography(unsigned char * imgSrc, unsigned char * imgDst, int width, int height, int * imgLabel, int ** H){
    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            if (imgLabel[y*width+x] > -1){
                ApplyPointHomography(imgSrc, imgDst, width, height, H[imgLabel[y*width+x]] );
            }
        }
    }*/