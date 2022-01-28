#include <cuda.h>
#include <cuda_runtime.h>
#include <cudaProfiler.h>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "cuda_func.cuh"

#define BLOCK_SIZE_X 16
#define BLOCK_SIZE_Y 16

__global__ void kernel_ApplyAllHomography(int width, int height, 
                                          int* imgLabel, float* H_flat, 
                                          float* XI,    float* YI){
    int i = blockIdx.x*blockDim.x + threadIdx.x,
		j = blockIdx.y*blockDim.y + threadIdx.y;
    float den;
    int label;
    if (i < width  && j < height){
        //printf("%d | %d\n", i, j);
        label = imgLabel[j*width+i];
        if (label > 0){
            label -= 1;
            den = 1.0 / (H_flat[9*label + 2]*i + H_flat[9*label + 5]*j + 1);
            
            XI[j*width + i] = (H_flat[9*label    ]*i+H_flat[9*label + 3]*j+H_flat[9*label + 6])*den;
            YI[j*width + i] = (H_flat[9*label + 1]*i+H_flat[9*label + 4]*j+H_flat[9*label + 7])*den;
        }
    }
}   

__global__ void kernel_RecreateImage(int* imgOut, 
                                     int* imgCAM ,int width_CAM, int height_CAM, 
                                     int* imgFTA, int width_FTA, int height_FTA, 
                                     float* XI, float* YI, int* imgLabel){
    int x_CAM = blockIdx.x*blockDim.x + threadIdx.x,
		y_CAM = blockIdx.y*blockDim.y + threadIdx.y;

    int x_FTA, y_FTA;

    if (x_CAM < width_CAM  && y_CAM < height_CAM){
        if(imgLabel[y_CAM*width_CAM+x_CAM] > 0){
            x_FTA = (int)XI[y_CAM * width_CAM + x_CAM];
            y_FTA = (int)YI[y_CAM * width_CAM + x_CAM];  
  
            imgOut[y_CAM * 3*width_CAM + x_CAM * 3 + 0] = imgFTA[y_FTA * 3*width_FTA + x_FTA*3 + 0];
            imgOut[y_CAM * 3*width_CAM + x_CAM * 3 + 1] = imgFTA[y_FTA * 3*width_FTA + x_FTA*3 + 1];
            imgOut[y_CAM * 3*width_CAM + x_CAM * 3 + 2] = imgFTA[y_FTA * 3*width_FTA + x_FTA*3 + 2];          
        }
    }          
}

float ApplyAllHomography_CUDA(int width, int height, int* imgLabel, float* H_flat, float* XI, float* YI, int n_quadrangles)
{   
    int size = width*height;
    int nbThreadx = BLOCK_SIZE_X, nbThready = BLOCK_SIZE_Y;
    dim3 dimBlock(nbThreadx, nbThready);
    dim3 dimGrid((width+nbThreadx-1)/nbThreadx, (height+nbThready-1)/nbThready);

    cudaEvent_t start, stop;
    float dt = 0.0f;

    /* Copy Homography matrix */
    float *H_in_cuda = NULL;
    cudaMalloc((void **)&H_in_cuda, 9*n_quadrangles*sizeof(float));
    cudaMemcpy(H_in_cuda, H_flat, 9*n_quadrangles*sizeof(float), cudaMemcpyHostToDevice);

    /* Copy Label Image */
    int * ImgLabel_in_cuda = NULL;
    cudaMalloc((void **)&ImgLabel_in_cuda, size*sizeof(int));
    cudaMemcpy(ImgLabel_in_cuda, imgLabel, size*sizeof(int), cudaMemcpyHostToDevice);

    /* Allocate memory for interpolation matrix */
    float *XI_out_cuda = NULL;
    float *YI_out_cuda = NULL;
    cudaMalloc((void **)&XI_out_cuda, size*sizeof(float));
    cudaMalloc((void **)&YI_out_cuda, size*sizeof(float));
    
    cudaEventCreate(&start);
	cudaEventCreate(&stop);	
    cudaEventRecord(start, 0);
    kernel_ApplyAllHomography<<<dimGrid, dimBlock>>>(width, height, ImgLabel_in_cuda, H_in_cuda, XI_out_cuda, YI_out_cuda);
    cudaDeviceSynchronize();
    cudaEventRecord(stop, 0);
	cudaEventSynchronize(stop);	
	cudaEventElapsedTime(&dt, start, stop);
	cudaEventDestroy(start);
	cudaEventDestroy(stop);
    
    
    /* Copy device result to host memory */
    cudaMemcpy(XI, XI_out_cuda, size*sizeof(float), cudaMemcpyDeviceToHost);
    cudaMemcpy(YI, YI_out_cuda, size*sizeof(float), cudaMemcpyDeviceToHost);

    /* Free device memory */
    cudaFree(ImgLabel_in_cuda);
    cudaFree(H_in_cuda);
    cudaFree(XI_out_cuda);
    cudaFree(YI_out_cuda);

    //std::cout << "Time spent in kernel : " << dt << std::endl;
    return dt;
}

void RecreateImage_CUDA(int* imgOut,
	               int* imgCAM, int width_CAM, int height_CAM,
	               int* imgFTA, int width_FTA, int height_FTA,
 	               float* XI, float* YI, int* imgLabel)
{
	int size_CAM = width_CAM * height_CAM;
    int size_FTA = width_FTA * height_FTA;

	int* imgout_cuda = NULL, *imgCAM_cuda = NULL, *imgFTA_cuda = NULL, *imgLabel_cuda = NULL;
	float *XI_cuda = NULL, *YI_cuda = NULL;
	
    int nbThreadx = BLOCK_SIZE_X, nbThready = BLOCK_SIZE_Y;
    dim3 dimBlock(nbThreadx, nbThready);
    dim3 dimGrid((width_CAM+nbThreadx-1)/nbThreadx, (height_CAM+nbThready-1)/nbThready);

	cudaMalloc((void **)&imgCAM_cuda, 3*size_CAM*sizeof(int));
	cudaMemcpy(imgCAM_cuda, imgCAM, 3*size_CAM*sizeof(int), cudaMemcpyHostToDevice);

	cudaMalloc((void **)&imgFTA_cuda, 3*size_FTA*sizeof(int));	
	cudaMemcpy(imgFTA_cuda, imgFTA, 3*size_FTA*sizeof(int), cudaMemcpyHostToDevice);

	cudaMalloc((void **)&imgout_cuda, 3*size_CAM*sizeof(int));
    cudaMemcpy(imgout_cuda, imgCAM, 3*size_CAM*sizeof(int), cudaMemcpyHostToDevice);

	cudaMalloc((void**)&XI_cuda, size_CAM * sizeof(float));
	cudaMemcpy(XI_cuda, XI, size_CAM * sizeof(float), cudaMemcpyHostToDevice);

	cudaMalloc((void**)&YI_cuda, size_CAM * sizeof(float));
	cudaMemcpy(YI_cuda, YI, size_CAM * sizeof(float), cudaMemcpyHostToDevice);

	cudaMalloc((void**)&imgLabel_cuda, size_CAM * sizeof(int));
	cudaMemcpy(imgLabel_cuda, imgLabel, size_CAM * sizeof(int), cudaMemcpyHostToDevice);



    /* Kernel */
	kernel_RecreateImage<<<dimGrid, dimBlock>>>(imgout_cuda, 
                                                imgCAM_cuda, width_CAM, height_CAM, 
                                                imgFTA_cuda, width_FTA, height_FTA,
                                                XI_cuda, YI_cuda, imgLabel_cuda);	

	cudaDeviceSynchronize();	
	
    /* Copy result to host memory */
    cudaMemcpy(imgOut, imgout_cuda, 3*size_CAM*sizeof(int), cudaMemcpyDeviceToHost);
	
    /* Free device memory */
    cudaFree(XI_cuda);
	cudaFree(YI_cuda);
	cudaFree(imgCAM_cuda);
	cudaFree(imgFTA_cuda);
	cudaFree(imgLabel_cuda);
	cudaFree(imgout_cuda);	
}