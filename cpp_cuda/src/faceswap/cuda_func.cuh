#ifndef _PYBIND_CUDA_H_
#define _PYBIND_CUDA_H_

extern "C"{
    void gpu_square_main(int * v_in, int size, int * v_out);
    //void ApplyAllHomography(int width, int height, int* imgLabel, double** H, double* XI, double* YI, int n_quadrangles);
    float ApplyAllHomography_CUDA(int width, int height, int* imgLabel, float* H_flat, float* XI, float* YI, int n_quadrangles); 
    void RecreateImage_CUDA(int* imgOut,
	                        int* imgCAM, int width_CAM, int height_CAM,
	                        int* imgFTA, int width_FTA, int height_FTA,
 	                        float* XI, float* YI, int* imgLabel);
}

 
#endif // _PYBIND_CUD_CUH_