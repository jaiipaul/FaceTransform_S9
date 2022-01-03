#include <cuda.h>
#include <cuda_runtime.h>
#include <cudaProfiler.h>

#include <stdio.h>
#include "pybind_cuda.cuh"

#define BLOCK_SIZE_X 16
#define BLOCK_SIZE_Y 16

__global__ void gpu_square(int* v, int size){
    int id = blockIdx.x*blockDim.x+threadIdx.x;
    if ( id < size ){
        v[id] = (255-v[id]);
    }
}

__global__ void kernel_ApplyAllHomography(int width, int height, int* imgLabel, double** H, double* XI, double* YI){
    int i = blockIdx.x*blockDim.x + threadIdx.x,
		j = blockIdx.y*blockDim.y + threadIdx.y;
    double *h, den;
    if (i < width  && j < height){
        if (imgLabel[j*width+i] > 0){
            h = H[imgLabel[j*width+i]];

            den = 1.0f / (h[2]*i+h[5]*j+h[8]);
            XI[j*width + i] =(h[0]*i+h[3]*j+h[6])*den;
            YI[j*width + i] =(h[1]*i+h[4]*j+h[7])*den;
        }
    }
    __syncthreads();
}   

__global__ void kernel_RecreateImage(int* imgOut, 
                              int* imgCAM ,int width_CAM, int height_CAM, 
                              int* imgFTA, int width_FTA, int height_FTA, 
                              double* XI, double* YI, int* imgLabel){
    int x_CAM = blockIdx.x*blockDim.x + threadIdx.x,
		y_CAM = blockIdx.y*blockDim.y + threadIdx.y;

    int canal;
    int x_FTA, y_FTA;

    if (x_CAM < width_CAM  && y_CAM < height_CAM){
        if(imgLabel[y_CAM*width_CAM+x_CAM] > 0){
            x_FTA = (int)floor(XI[y_CAM * width_CAM + x_CAM]);
            y_FTA = (int)floor(YI[y_CAM * width_CAM + x_CAM]);  

            for(canal = 0; canal < 3; canal++){
                imgOut[y_CAM * 3*width_CAM + x_CAM * 3 + canal] = imgFTA[y_FTA * 3*width_FTA + x_FTA*3 + canal];
            }         
        }else{
            for(canal = 0; canal < 3; canal++){
                imgOut[y_CAM * 3*width_CAM + x_CAM * 3 + canal] = imgCAM[y_CAM * 3*width_CAM + x_CAM*3 + canal];
            }
        }
    }    
    __syncthreads();       
}


void gpu_square_main(int * v_in, int size, int * v_out)
{
    int nbThread = BLOCK_SIZE_X*BLOCK_SIZE_Y;
    dim3 dimBlock(nbThread);
    dim3 dimGrid((size+nbThread-1)/nbThread);
    int * v_in_cuda = NULL;
    cudaMalloc((void **)&v_in_cuda, size*sizeof(int));
    cudaMemcpy(v_in_cuda, v_in, size*sizeof(int), cudaMemcpyHostToDevice);
    gpu_square<<<dimGrid, dimBlock>>>(v_in_cuda, size);
    cudaError_t err = cudaGetLastError();
    if (err != cudaSuccess) 
        printf("Error: %s\n", cudaGetErrorString(err));

    //printf("Kernel DONE\n");
    cudaThreadSynchronize();
    cudaMemcpy(v_out, v_in_cuda, size*sizeof(int), cudaMemcpyDeviceToHost);
    cudaFree(v_in_cuda);
}

void ApplyAllHomography(int width, int height, int* imgLabel, double** H, double* XI, double* YI, int n_quadrangles)
{   
    int size = width*height;
    int nbThread = BLOCK_SIZE_X*BLOCK_SIZE_Y;
    dim3 dimBlock(nbThread);
    dim3 dimGrid((size+nbThread-1)/nbThread);
    double * XI_out_cuda = NULL;
    double * YI_out_cuda = NULL;

    /* Copy Homography matrix */
    double ** H_in_cuda = NULL;
    cudaMalloc((void **)&H_in_cuda, n_quadrangles*sizeof(double*));
    cudaMalloc((void **)&H_in_cuda[0], 9*n_quadrangles*sizeof(double));
    for(int i = 0; i < n_quadrangles; i++){
        H_in_cuda[i] = H_in_cuda[0] + i*9;
    }
    cudaMemcpy(H_in_cuda, H, n_quadrangles*sizeof(double*), cudaMemcpyHostToDevice);
    cudaMemcpy(H_in_cuda[0], H[0], 9*n_quadrangles*sizeof(double), cudaMemcpyHostToDevice);

    /* Copy Label Image */
    int * ImgLabel_in_cuda = NULL;
    cudaMalloc((void **)&ImgLabel_in_cuda, size*sizeof(int));
    cudaMemcpy(ImgLabel_in_cuda, imgLabel, size*sizeof(int), cudaMemcpyHostToDevice);

    cudaMalloc((void **)&XI_out_cuda, size*sizeof(double));
    cudaMalloc((void **)&YI_out_cuda, size*sizeof(double));

    
    kernel_ApplyAllHomography<<<dimGrid, dimBlock>>>(width, height, ImgLabel_in_cuda, H_in_cuda, XI_out_cuda, YI_out_cuda);
    cudaError_t err = cudaGetLastError();
    if (err != cudaSuccess) 
        printf("Error: %s\n", cudaGetErrorString(err));

    //printf("Kernel DONE\n");
    cudaThreadSynchronize();
    cudaMemcpy(XI, XI_out_cuda, size*sizeof(double), cudaMemcpyDeviceToHost);
    cudaMemcpy(XI, XI_out_cuda, size*sizeof(double), cudaMemcpyDeviceToHost);

    cudaFree(ImgLabel_in_cuda);
    cudaFree(H_in_cuda[0]);
    cudaFree(H_in_cuda);
    cudaFree(XI_out_cuda);
    cudaFree(YI_out_cuda);
}

