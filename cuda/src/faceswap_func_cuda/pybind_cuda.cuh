#ifndef _PYBIND_CUDA_H_
#define _PYBIND_CUDA_H_

extern "C"{
    void gpu_square_main(int * v_in, int size, int * v_out);
    void ApplyAllHomography(int width, int height, int* imgLabel, double** H, double* XI, double* YI, int n_quadrangles); 
}

 
#endif // _PYBIND_CUD_CUH_