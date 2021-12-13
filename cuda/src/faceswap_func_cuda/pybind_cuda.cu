#include <cuda.h>
#include <cuda_runtime.h>
#include <cudaProfiler.h>

#include "pybind_cuda.cuh"

__global__ void gpu_square(int* v, int size)
{
    int id = blockIdx.x*blockDim.x+threadIdx.x;
    if ( id < size ) v[id] = v[id]*v[id];
}


void gpu_square_main(int * v_in, int size, int * v_out)
{
    int nbThread = 512;
    dim3 dimBlock(nbThread);
    dim3 dimGrid((size+nbThread-1)/nbThread);
    int * v_in_cuda = NULL;
    cudaMalloc((void **)&v_in_cuda, size*sizeof(int));
    cudaMemcpy(v_in_cuda, v_in, size*sizeof(int), cudaMemcpyHostToDevice);
    gpu_square<<<dimGrid, dimBlock>>>(v_in_cuda, size);
    cudaThreadSynchronize();
    cudaMemcpy(v_out, v_in_cuda, size*sizeof(int), cudaMemcpyDeviceToHost);
    cudaFree(v_in_cuda);
}

