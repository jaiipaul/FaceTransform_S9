#ifndef _HOMOGRAPHIE_H_
#define _HOMOGRAPHIE_H_

void printMat(double* Mat, int col, int line);
void printMat(int* Mat, int col, int line);

void Permute(int size, double *A, double *b, int i);
void create_A(int size, double *A, double *p, double *m);

void pivotdeGauss(int size, double *A, double *b);

void SystemeTriangulaireSuperieur(int n,double *A,double *y,double *x);

void Find_Homography(double* src, double* dst, double *h);
void CV_Find_Homography(float* src, float* dst, float *h);

void ApplyPointHomography(float* h, float *m, float *p);

#endif //_HOMOGRAPHIE_H_ 