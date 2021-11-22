#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void create_A(int size, double *A, double *p, double *m)
{
    for(int i=0;i<4;i++){
        A[i*size]=m[i*2];
        A[i*size+2]=m[i*2]*p[i*2];
        A[i*size+3]=m[i*2+1];
        A[i*size+5]=m[i*2+1]*p[i*2+1];
        A[i*size+6]=1;
        A[2*i*size+1]=m[i*2]
        A[2*i*size+2]=m[i*2]*p[i*2];
        A[2*i*size+4]=m[i*2+1];
        A[2*i*size+5]=m[i*2+1]*p[i*2+1];
        A[2*i*size+7]=1;
    }
}


void pivotdeGauss(int size, double *A, double *b)
{
    for(int i=0;i<size-1;i++){
        for (int j=i+1;j<size;j++){
            double coeff=A[j*size+i]/A[i*size+i];
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

void Find_Homography(double * src, double *dst, double *h){
    double* A =(double*)calloc(64,sizeof(double));
    double* B =(double*)
    create_A(8,A,dst,src);
    pivotdeGauss(8,A,dst);
    SystemeTriangulaireSuperieur(8,A,dst,h);
    free(A);    
}

void applique_homo(double * h, double *m, double *p)
{
    for(int i=0;i<4;i++){
        p[i*2]=(h[0]*m[i*2]+h[3]*m[i*2+1]+h[6])/(h[2]*m[i*2]+h[5]*m[i*2+1]+1);
        p[i*2+1]=(h[1]*m[i*2]+h[4]*m[i*2+1]+h[7])/(h[2]*m[i*2]+h[5]*m[i*2+1]+1);    
    }
}

void CoefficientsGivens(int p,int q,double *c,double *s,double (*a)[50])
{
    double norme;
    norme=sqrt(pow(a[p][p],2)+pow(a[q][p],2));
    if(norme==0)
    {
        *c=1;
        *s=0;
    }
    else{

        *c=a[p][p]/norme;
        *s=a[q][p]/norme;
    }
}

void Premultiplier(int n,int p,int q,double c,double s,double (*a)[50])
{
int j;
double v,w;
for(j=0;j<n;j++)
{
    v=a[p][j];
    w=a[q][j];
    a[p][j]=(c*v)+(s*w);
    a[q][j]=(-s*v)+(c*w);
}
}

void IterationGivens(int n,double (*a)[50],double (*u)[50])
{
    int p,q,i,j;
    double c,s;
    for(p=0;p<n-1;p++)
        for(q=p+1;q<n;q++)
    {
     CoefficientsGivens(p,q,&c,&s,a);
     Premultiplier(n,p,q,c,s,u);
     Premultiplier(n,p,q,c,s,a);
     printf("\np=%d,q=%d\n",p+1,q+1);
      for(i=0;i<n;i++){printf("\n");
        for(j=0;j<n;j++)printf("  %.2lf  ",a[i][j]);
        }
    }
}
void transposer(int n,double (*q)[50])
{
    double qt[50][50];
    int i,j;
    for(i=0;i<n;i++)
        for(j=0;j<n;j++)qt[j][i]=q[i][j];

     for(i=0;i<n;i++)
        for(j=0;j<n;j++)q[i][j]=qt[i][j];

}
void DecompositionQRparGivens(int n,double (*a)[50],double (*q)[50],double (*r)[50])
{
    int i,j;
 for(i=0;i<n;i++)
 for(j=0;j<n;j++)r[i][j]=a[i][j];
 ///MatriceUnite
 for(i=0;i<n;i++)q[i][i]=1;
 IterationGivens(n,r,q);
 transposer(n,q);
}
void MatriceparVecteur(int n,double (*q)[50],double *b,double *y)
{
    double s=0;
    int i,j;
    for(i=0;i<n;i++)
    {
        for(j=0;j<n;j++){s+=q[i][j]*b[j];}
        y[i]=s;
        s=0;
    }
}



