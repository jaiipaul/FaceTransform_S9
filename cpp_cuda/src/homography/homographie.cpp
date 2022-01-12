#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <opencv2/calib3d.hpp>
#include <iostream>


void printMat(double* Mat, int col, int line) {
    printf("Mat = \n");
    for(int i = 0; i < line; i++) {
        printf("[");
        for(int j = 0; j < col; j++) {
            printf("%f,", Mat[i * line + j]);
        }
        printf("]\n");
    }
}
void printMat(int* Mat, int col, int line) {
    printf("Mat = \n");
    for (int i = 0; i < line; i++) {
        printf("[");
        for (int j = 0; j < col; j++) {
            printf("%d,", Mat[i * line + j]);
        }
        printf("]\n");
    }
}


void create_A(int size, double *A, double *p, double *m)
{
    //printMat(p, 8, 1);
    //printMat(m, 8, 1);
    for(int i=0;i<4;i++){
        A[2 * i * size    ] =  m[i * 2];
        A[2 * i * size + 1] =  0;
        A[2 * i * size + 2] = -m[i * 2] * p[i * 2];
        A[2 * i * size + 3] =  m[i * 2 + 1];
        A[2 * i * size + 4] =  0;
        A[2 * i * size + 5] = -m[i * 2 + 1] * p[i * 2];
        A[2 * i * size + 6] =  1;
        A[2 * i * size + 7] =  0;
        A[(2 * i + 1) * size    ] =  0;
        A[(2 * i + 1) * size + 1] =  m[i * 2];
        A[(2 * i + 1) * size + 2] = -m[i * 2] * p[i * 2 + 1];
        A[(2 * i + 1) * size + 3] =  0;
        A[(2 * i + 1) * size + 4] =  m[i * 2 + 1];
        A[(2 * i + 1) * size + 5] = -m[i * 2 + 1] * p[i * 2 + 1];
        A[(2 * i + 1) * size + 6] =  0;
        A[(2 * i + 1) * size + 7] =  1;
    }
    //printMat(A, 8, 8);
}

void Permute(int size, double *A, double *b, int i)
{
    int k=i+1;
    while(A[k*size+i]==0)
        k=k+1;
    for(int m=i;m<size;m++){
        double cA=A[i*size+m];
        A[i*size+m]=A[k*size+m];
        A[k*size+m]=cA;  
    }
    double cb = b[i];
    b[i] = b[k];
    b[k] = cb;
    //printMat(A, 8, 8);
}


void pivotdeGauss(int size, double *A, double *b)
{
    for(int i=0;i<size-1;i++){
        for (int j=i+1;j<size;j++){
            if (A[i*size+i]==0){
            Permute(size,A, b, i);
            }
            double coeff=A[j*size+i]/A[i*size+i];
            //printf("%f / %f = %f\n", A[j * size + i], A[i * size + i], coeff);
            for (int k=0;k<size;k++){
                A[j*size+k]=A[j*size+k]-A[i*size+k]*coeff;
            }
            b[j]-=b[i]*coeff;
        }
    }
}

void SystemeTriangulaireSuperieur(int n,double *A,double *y,double *x)
{
    double R_X=0;
    int i,j;
    for(i=n-1;i>=0;i--)
    {
        if(i==(n-1)){ 
            x[i]=y[i]/A[i*n+i];
        }
        else{
            for(j=n-1;j>i;j--){
                R_X+=x[j]*A[i*n+j];
            }
            x[i]=(y[i]-R_X)/A[i*n+i];
            R_X=0;
        }
        
    }
}

void Find_Homography(double* src, double* dst, double *h){
    double* A =(double*)calloc(64,sizeof(double));
    double* B =(double*)calloc(8, sizeof(double));
    for(int i = 0; i < 8; i++){
        B[i] = src[i];
    }
    //printMat(src, 8, 1);
    //printMat(dst, 8, 1);
    create_A(8,A, src, dst);
    pivotdeGauss(8,A,B);
    SystemeTriangulaireSuperieur(8,A,B,h);
    h[8] = 1.0f;
    //printMat(h, 8, 1);
    free(A);
    free(B);   
}

void CV_Find_Homography(float* src, float* dst, float *h){
    //std::cout << "convert to vector" << std::endl;
    std::vector<cv::Point2f> Src;
    std::vector<cv::Point2f> Dst;
    for(int i = 0; i < 4; i++ ){
        Src.push_back(cv::Point2f(src[2*i],src[2*i+1]));
        Dst.push_back(cv::Point2f(dst[2*i],dst[2*i+1]));
    }

    cv::Mat H = (cv::Mat_<double>(3,3));
    //std::cout << "Open CV homography" << std::endl;
    H = cv::findHomography(Dst, Src);
    
    for( int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            h[i*3 + j] = (float)H.at<double>(j,i);
        }
    }
}

void ApplyPointHomography(float *h, float *m, float *p)
{
    //printf("Apply point homography\n");
        float den = 1.0f / (h[2]*m[0]+h[5]*m[1]+h[8]);
        p[0]=(h[0]*m[0]+h[3]*m[1]+h[6])*den;
        p[1]=(h[1]*m[0]+h[4]*m[1]+h[7])*den;
}

