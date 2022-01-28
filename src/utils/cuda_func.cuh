#ifndef _CUDA_FUNC_CUH_
#define _CUDA_FUNC_CUH_

extern "C"{
    float ApplyAllHomography_CUDA(int width, int height, int* imgLabel, float* H_flat, float* XI, float* YI, int n_quadrangles); 
    void RecreateImage_CUDA(int* imgOut,
	                        int* imgCAM, int width_CAM, int height_CAM,
	                        int* imgFTA, int width_FTA, int height_FTA,
 	                        float* XI, float* YI, int* imgLabel);
}

 
#endif // _CUDA_FUNC_CUH_