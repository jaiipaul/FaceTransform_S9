#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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

void comatrices(double* a, double* c, int i, int j, int dim)
{
    int l, k;
    for (l = 0; l < dim; l++) for (k = 0; k < dim; k++)
    {
        if ((l < i) && (k < j)) c[      l*dim + k    ] = a[l*dim+k];
        if ((l > i) && (k < j)) c[(l - 1)*dim + k    ] = a[l*dim+k];
        if ((l < i) && (k > j)) c[      l*dim + k - 1] = a[l*dim+k];
        if ((l > i) && (k > j)) c[(l - 1)*dim + k - 1] = a[l*dim+k];
    }
    //printf("Comatrice ok\n");
}
// calcul du determinant

double det(double* a, int dim)
{
    int sous_dim, j;
    sous_dim = dim - 1;
    double* c, s;
    c = (double*)calloc(dim * dim, sizeof(double));

    if (dim == 1) return(a[0]);

    s = 0;
    for (j = 0; j < dim; j++)
    {
        //printf("%d calcul comatrice\n", j);
        comatrices(a, c, sous_dim, j, dim);
        //printMat(c, sous_dim, sous_dim);
        double res = pow(-1, sous_dim + j) * a[sous_dim * dim + j] * det(c, sous_dim);
        //printf("%f + %f = ", s, res);
        s = s + res;
        //printf("%f\n", s);
    }
    free(c);
    return(s);
    //printf("%f\n", s);
}

void cramer(double* a, double* b, double* x, int n)
{   
    printf("\n => CRAMER\n");
    double *A, deter; 
    int i, j, k;

    A = (double*)calloc(n * n, sizeof(double));
    //x = (double*)calloc(n, sizeof(double));

    deter = det(a, n);
    printf("%f", deter);
    if (deter == 0)
    {
        printf("\n => Determinant nul, pas de solutions \n\n");
    }

    for (j = 0; j < n; j++)
    {
        for (k = 0; k < n; k++)
        {
            if (k == j) for (i = 0; i < n; i++) A[i*n+k] = b[i];
            else for (i = 0; i < n; i++) A[i*n+k] = a[i*n+k];
        }
        x[j] = det(A, n) / deter;
    }
    printf("\n-------------- Cramer -------------\n");
    printf("\n * Le determiant du systeme : %f \n", deter);
    printf("\n * La resolution donne :\n\n");
    for (i = 0; i < n; i++) printf(" X_%d = %f ;\n", i + 1, x[i]);
    free(A);
}

void create_A(int size, double *A, double *p, double *m)
{
    printMat(p, 8, 1);
    printMat(m, 8, 1);
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
    printMat(A, 8, 8);
}

void Permute(int size, double *A, int i)
{
    k=i+1
    while (A[k*size+i]==0)
        k=k+1;
    for(int m=i;m<size;m++){
        double c=A[i*size+m];
        A[i*size+m]=A[k*size+m];
        A[k*size+m]=c;
    }
}


void pivotdeGauss(int size, double *A, double *b)
{
    for(int i=0;i<size-1;i++){
        for (int j=i+1;j<size;j++){
            if (A[i*size+i]==0){
            Permute(size,A,i);
            }
            double coeff=A[j*size+i]/A[i*size+i];
            printf("%f / %f = %f\n", A[j * size + i], A[i * size + i], coeff);
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
    printMat(src, 8, 1);
    printMat(dst, 8, 1);
    create_A(8,A,src,dst);
    cramer(A, B, h, 8);
    //pivotdeGauss(8,A,B);
    //SystemeTriangulaireSuperieur(8,A,B,h);
    free(A);     
}

void ApplyPointHomography(double * h, double *m, double *p)
{
    //printf("Apply point homography\n");
        p[0]=(h[0]*m[0]+h[3]*m[1]+h[6])/(h[2]*m[0]+h[5]*m[1]+1);
        p[1]=(h[1]*m[0]+h[4]*m[1]+h[7])/(h[2]*m[0]+h[5]*m[1]+1);
}

//void CoefficientsGivens(int p,int q,double *c,double *s,double (*a)[50])
//{
//    double norme;
//    norme=sqrt(pow(a[p][p],2)+pow(a[q][p],2));
//    if(norme==0)
//    {
//        *c=1;
//        *s=0;
//    }
//    else{
//
//        *c=a[p][p]/norme;
//        *s=a[q][p]/norme;
//    }
//}
//
//void Premultiplier(int n,int p,int q,double c,double s,double (*a)[50])
//{
//int j;
//double v,w;
//for(j=0;j<n;j++)
//{
//    v=a[p][j];
//    w=a[q][j];
//    a[p][j]=(c*v)+(s*w);
//    a[q][j]=(-s*v)+(c*w);
//}
//}
//
//void IterationGivens(int n,double (*a)[50],double (*u)[50])
//{
//    int p,q,i,j;
//    double c,s;
//    for(p=0;p<n-1;p++)
//        for(q=p+1;q<n;q++)
//    {
//     CoefficientsGivens(p,q,&c,&s,a);
//     Premultiplier(n,p,q,c,s,u);
//     Premultiplier(n,p,q,c,s,a);
//     printf("\np=%d,q=%d\n",p+1,q+1);
//      for(i=0;i<n;i++){printf("\n");
//        for(j=0;j<n;j++)printf("  %.2lf  ",a[i][j]);
//        }
//    }
//}
//void transposer(int n,double (*q)[50])
//{
//    double qt[50][50];
//    int i,j;
//    for(i=0;i<n;i++)
//        for(j=0;j<n;j++)qt[j][i]=q[i][j];
//
//     for(i=0;i<n;i++)
//        for(j=0;j<n;j++)q[i][j]=qt[i][j];
//
//}
//void DecompositionQRparGivens(int n,double (*a)[50],double (*q)[50],double (*r)[50])
//{
//    int i,j;
// for(i=0;i<n;i++)
// for(j=0;j<n;j++)r[i][j]=a[i][j];
// ///MatriceUnite
// for(i=0;i<n;i++)q[i][i]=1;
// IterationGivens(n,r,q);
// transposer(n,q);
//}
//void MatriceparVecteur(int n,double (*q)[50],double *b,double *y)
//{
//    double s=0;
//    int i,j;
//    for(i=0;i<n;i++)
//    {
//        for(j=0;j<n;j++){s+=q[i][j]*b[j];}
//        y[i]=s;
//        s=0;
//    }
//}



