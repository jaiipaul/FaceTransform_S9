#ifndef _HOMOGRAPHIE_H_
#define _HOMOGRAPHIE_H_

void printMat(double* Mat, int col, int line);

void comatrices(double* a, double* c, int i, int j, int n);
double det(double* a, int dim);
void cramer(double* a, double* b, double* x, int n);

void create_A(int size, double *A, double *p, double *m);

void pivotdeGauss(int size, double *A, double *b);

void SystemeTriangulaireSuperieur(int n,double *A,double *y,double *x);

void Find_Homography(double* src, double* dst, double *h);

void ApplyPointHomography(double * h, double *m, double *p);

//void CoefficientsGivens(int p,int q,double *c,double *s,double (*a)[50]);
//
//void Premultiplier(int n,int p,int q,double c,double s,double (*a)[50]);
//
//void IterationGivens(int n,double (*a)[50],double (*u)[50]);
//
//void transposer(int n,double (*q)[50]);
//
//void DecompositionQRparGivens(int n,double (*a)[50],double (*q)[50],double (*r)[50]);
//
//void MatriceparVecteur(int n,double (*q)[50],double *b,double *y);


#endif //_HOMOGRAPHIE_H_ 