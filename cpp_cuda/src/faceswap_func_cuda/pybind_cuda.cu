#include <cuda.h>
#include <cuda_runtime.h>
#include <cudaProfiler.h>

#include <stdio.h>
#include <stdlib.h>
#include "pybind_cuda.cuh"

#define BLOCK_SIZE_X 32
#define BLOCK_SIZE_Y 32

__global__ void gpu_square(int* v, int size){
    int id = blockIdx.x*blockDim.x+threadIdx.x;
    if ( id < size ){
        v[id] = (255-v[id]);
    }
}


__global__ void kernel_ApplyAllHomography(int width, int height, int* imgLabel, double* H_flat, double* XI, double* YI){
    int i = blockIdx.x*blockDim.x + threadIdx.x,
		j = blockIdx.y*blockDim.y + threadIdx.y;
    double den;
    int label;
    if (i < width  && j < height){
        //printf("%d | %d\n", i, j);
        label = imgLabel[j*width+i];
        if (label > 0){
            den = 1.0 / (H_flat[9*label + 2]*i + H_flat[9*label + 5]*j + 1);
            
            XI[j*width + i] = (H_flat[9*label    ]*i+H_flat[9*label + 3]*j+H_flat[9*label + 6])*den;
            YI[j*width + i] = (H_flat[9*label + 1]*i+H_flat[9*label + 4]*j+H_flat[9*label + 7])*den;
        }
    }
    //__syncthreads();
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
        //printf("%d | %d\n", x_CAM, y_CAM);
        if(imgLabel[y_CAM*width_CAM+x_CAM] > 0){
            x_FTA = (int)XI[y_CAM * width_CAM + x_CAM];
            y_FTA = (int)YI[y_CAM * width_CAM + x_CAM];  
            //printf("in : %d | %d | %d \n", imgFTA[y_FTA * 3*width_FTA + x_FTA*3 + 0],
            //                               imgFTA[y_FTA * 3*width_FTA + x_FTA*3 + 1],
            //                               imgFTA[y_FTA * 3*width_FTA + x_FTA*3 + 2]);
  
            imgOut[y_CAM * 3*width_CAM + x_CAM * 3 + 0] = imgFTA[y_FTA * 3*width_FTA + x_FTA*3 + 0];
            imgOut[y_CAM * 3*width_CAM + x_CAM * 3 + 1] = imgFTA[y_FTA * 3*width_FTA + x_FTA*3 + 1];
            imgOut[y_CAM * 3*width_CAM + x_CAM * 3 + 2] = imgFTA[y_FTA * 3*width_FTA + x_FTA*3 + 2];          
        }
    }    
    //__syncthreads();       
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

void ApplyAllHomography_CUDA(int width, int height, int* imgLabel, double* H_flat, double* XI, double* YI, int n_quadrangles)
{   
    int size = width*height;
    int nbThreadx = BLOCK_SIZE_X, nbThready = BLOCK_SIZE_Y;
    dim3 dimBlock(nbThreadx, nbThready);
    dim3 dimGrid((width+nbThreadx-1)/nbThreadx, (height+nbThready-1)/nbThready);

    double *XI_out_cuda = NULL;
    double *YI_out_cuda = NULL;

    printf("Try to copy H matrix\n");
    /* Copy Homography matrix */
    double *H_in_cuda = NULL;
    cudaMalloc((void **)&H_in_cuda, 9*n_quadrangles*sizeof(double));
    cudaError_t errA = cudaGetLastError();
    if (errA != cudaSuccess) 
        printf("Error CUDA before homography: %s\n", cudaGetErrorString(errA));
    printf("cudaMalloc done for H\n");
    cudaMemcpy(H_in_cuda, H_flat, 9*n_quadrangles*sizeof(double), cudaMemcpyHostToDevice);

    printf("Copied H matrix\n");
    /* Copy Label Image */
    int * ImgLabel_in_cuda = NULL;
    cudaMalloc((void **)&ImgLabel_in_cuda, size*sizeof(int));
    cudaMemcpy(ImgLabel_in_cuda, imgLabel, size*sizeof(int), cudaMemcpyHostToDevice);
    printf("Copied ImgLabel\n");

    cudaMalloc((void **)&XI_out_cuda, size*sizeof(double));
    cudaMalloc((void **)&YI_out_cuda, size*sizeof(double));
    //cudaMemcpy(XI_out_cuda, XI, size*sizeof(double), cudaMemcpyHostToDevice);
    //cudaMemcpy(YI_out_cuda, YI, size*sizeof(double), cudaMemcpyHostToDevice);
    

    printf("Kernel H\n");
    kernel_ApplyAllHomography<<<dimGrid, dimBlock>>>(width, height, ImgLabel_in_cuda, H_in_cuda, XI_out_cuda, YI_out_cuda);
    cudaError_t errB = cudaGetLastError();
    if (errB != cudaSuccess) 
        printf("Error CUDA after homography: %s\n", cudaGetErrorString(errB));

    //printf("Kernel DONE\n");
    cudaThreadSynchronize();
    cudaMemcpy(XI, XI_out_cuda, size*sizeof(int), cudaMemcpyDeviceToHost);
    cudaMemcpy(YI, YI_out_cuda, size*sizeof(int), cudaMemcpyDeviceToHost);

    cudaFree(ImgLabel_in_cuda);
    cudaFree(H_in_cuda);
    cudaFree(XI_out_cuda);
    cudaFree(YI_out_cuda);
}

void RecreateImage_CUDA(int* imgOut,
	               int* imgCAM, int width_CAM, int height_CAM,
	               int* imgFTA, int width_FTA, int height_FTA,
 	               double* XI, double* YI, int* imgLabel)
{
	int size_CAM = width_CAM * height_CAM;
    int size_FTA = width_FTA * height_FTA;
    printf("%d\n", size_FTA);
	int* imgout_cuda = NULL, *imgCAM_cuda = NULL, *imgFTA_cuda = NULL, *imgLabel_cuda = NULL;
	double *XI_cuda = NULL, *YI_cuda = NULL;
	
    int nbThreadx = BLOCK_SIZE_X, nbThready = BLOCK_SIZE_Y;
    dim3 dimBlock(nbThreadx, nbThready);

	cudaMalloc((void **)&imgCAM_cuda, 3*size_CAM*sizeof(int));
    cudaError_t errB = cudaGetLastError();
    if (errB != cudaSuccess) 
        printf("Error CUDA R imgCAM: %s\n", cudaGetErrorString(errB));
	cudaMemcpy(imgCAM_cuda, imgCAM, 3*size_CAM*sizeof(int), cudaMemcpyHostToDevice);
    errB = cudaGetLastError();
    if (errB != cudaSuccess) 
        printf("Error CUDA R imgCAM copy: %s\n", cudaGetErrorString(errB));
	cudaMalloc((void **)&imgFTA_cuda, 3*size_FTA*sizeof(int));	
	cudaMemcpy(imgFTA_cuda, imgFTA, 3*size_FTA*sizeof(int), cudaMemcpyHostToDevice);
    printf("%d\n", imgFTA[0]);
    errB = cudaGetLastError();
    if (errB != cudaSuccess) 
        printf("Error CUDA R imgFTA: %s\n", cudaGetErrorString(errB));

	cudaMalloc((void **)&imgout_cuda, 3*size_CAM*sizeof(int));
    cudaMemcpy(imgout_cuda, imgCAM, 3*size_CAM*sizeof(int), cudaMemcpyHostToDevice);
    errB = cudaGetLastError();
    if (errB != cudaSuccess) 
        printf("Error CUDA R imgOut: %s\n", cudaGetErrorString(errB));

	cudaMalloc((void**)&XI_cuda, size_CAM * sizeof(double));
	cudaMemcpy(XI_cuda, XI, size_CAM * sizeof(double), cudaMemcpyHostToDevice);
    errB = cudaGetLastError();
    if (errB != cudaSuccess) 
        printf("Error CUDA R XI: %s\n", cudaGetErrorString(errB));
	cudaMalloc((void**)&YI_cuda, size_CAM * sizeof(double));
	cudaMemcpy(YI_cuda, YI, size_CAM * sizeof(double), cudaMemcpyHostToDevice);
    errB = cudaGetLastError();
    if (errB != cudaSuccess) 
        printf("Error CUDA R YI: %s\n", cudaGetErrorString(errB));

	cudaMalloc((void**)&imgLabel_cuda, size_CAM * sizeof(int));
	cudaMemcpy(imgLabel_cuda, imgLabel, size_CAM * sizeof(int), cudaMemcpyHostToDevice);
    errB = cudaGetLastError();
    if (errB != cudaSuccess) 
        printf("Error CUDA R imgLabel: %s\n", cudaGetErrorString(errB));	
	// appel bonne fonction
	dim3 dimGrid((width_CAM+nbThreadx-1)/nbThreadx, (height_CAM+nbThready-1)/nbThready);
    printf("Kernel R\n");
	kernel_RecreateImage<<<dimGrid, dimBlock>>>(imgout_cuda, 
                                                imgCAM_cuda, width_CAM, height_CAM, 
                                                imgFTA_cuda, width_FTA, height_FTA,
                                                XI_cuda, YI_cuda, imgLabel_cuda);	
	//
    cudaError_t err = cudaGetLastError();
    if (err != cudaSuccess) 
        printf("Error CUDA Recreate: %s\n", cudaGetErrorString(err));
	cudaDeviceSynchronize();	
	
	cudaFree(XI_cuda);
	cudaFree(YI_cuda);
	cudaFree(imgCAM_cuda);
	cudaFree(imgFTA_cuda);
	cudaFree(imgLabel_cuda);
	cudaMemcpy(imgOut, imgout_cuda, 3*size_CAM*sizeof(int), cudaMemcpyDeviceToHost);
	cudaFree(imgout_cuda);	
}