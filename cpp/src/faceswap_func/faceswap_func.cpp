#include <stdio.h>
#include "faceswap_func.h"
#include "homographie.h"

void hello() {
	printf("Hello World !\n");
}

void addition(int a, int b) {
	int res;
	res = a + b;
	printf("addition : %d + %d = %d\n", a, b, res);
}

void fillTab(list tab , int size){
    for(int i = 0; i < size; i++){
        tab[i] = i;
    }
}
// Define functions here


void FindAllHomography(int n_quadrangles, int** Quadrangles, int** landmarks_src, int** landmarks_dst, double ** H){
    int * Q_coords_src, Q_coords_dst;
    Q_coords_src = (int*)calloc(8, sizeof(int));
    Q_coords_dst = (int*)calloc(8, sizeof(int));

    for(int i = 0; i < n_quadrangles; i++){
        for(int j = 0; j < 4; j++){
            //Q_coords = [ x0, y0, x1, y1, x2, y2, x3, y3]
            Q_coords_src[2*j]   = landmarks_src[Quadrangles[i][j]][0];
            Q_coords_src[2*j+1] = landmarks_src[Quadrangles[i][j]][1];

            Q_coords_dst[2*j]   = landmarks_dst[Quadrangles[i][j]][0];
            Q_coords_dst[2*j+1] = landmarks_dst[Quadrangles[i][j]][1];
        }
        Find_Homography(Q_coords_src, Q_coords_dst, H[i]);
    }
}  


//TO FINISH
int ApplyAllHomography(unsigned char * imgSrc, unsigned char * imgDst, int width, int height, int * imgLabel, int ** H){
    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            if (imgLabel[y*width+x] > -1){
                ApplyPointHomography(imgSrc, imgDst, width, height, H[imgLabel[y*width+x]] );
            }
        }
    }
}

void CreateLabelledImage(int n_quadrangles, int** Quadrangles, int** landmarks, int heigth, int width, double* img_label) {
	
    double Sqr_coords[8] = {-1, 1, 1, 1, 1, -1, -1, -1};
	double * Q_coords; 
    Q_coords_src = (double*)calloc(8, sizeof(double));
    
    double** H;
    H = (double*)calloc(n_quadrangles, sizeof(double*));
    
    for(int i = 0; i < n_quadrangles; i++){
        H[i] = (double*)calloc(9, sizeof(double));
        for(int j = 0; j < 4; j++){
            //Q_coords = [ x0, y0, x1, y1, x2, y2, x3, y3]
            Q_coords[2*j]   = landmarks[Quadrangles[i][j]][0];
            Q_coords[2*j+1] = landmarks[Quadrangles[i][j]][1];
        }
        Find_Homography(Q_coords, Sqr_coords, H[i]);
    }
    
	for (int j_quadrangle = 0, j_quadrangle < nb_quadrangle, j_quadrangle++) {

		 rectangles_h[j_quadrangle] = applique_homo(quadrangles[j_quadrangle][0], quadrangles[1], quadrangles[2], quadrangles[3]);

		int x_min_pixel = min(landmarks(quadrangles[j_quadrangle])[0][0])), landmarks(quadrangles[1][0]), landmarks((quadrangles[2][0]), quadrangles[3][0]);
		int x_max_pixel = max(landmarks(quadrangles[j_quadrangle])[0][0])), landmarks(quadrangles[1][0]), landmarks((quadrangles[2][0]), quadrangles[3][0]);
		int y_min_pixel = min(landmarks(quadrangles[j_quadrangle])[0][1])), landmarks(quadrangles[1][1]), landmarks((quadrangles[2][1]), quadrangles[3][1]);
		int y_max_pixel = max(landmarks(quadrangles[j_quadrangle])[0][1])), landmarks(quadrangles[1][1]), landmarks((quadrangles[2][1]), quadrangles[3][1]);

		for (int index_x_pixel = x_min_pixel, index_x_pixel < x_max_pixel, index_x_pixel++) {

			for (int index_y_pixel = y_min_pixel, index_y_pixel < y_max_pixel, index_y_pixel++) {

				if ((homography(tableau_coordonees_pixel[index_x_pixel][index_y_pixel][0]) < 1) {

					if ((homography(tableau_coordonees_pixel[index_x_pixel][index_y_pixel][1]) < 1 {

						}
				}
			}
		}
}