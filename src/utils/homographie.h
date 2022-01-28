#ifndef _HOMOGRAPHIE_H_
#define _HOMOGRAPHIE_H_

void Find_Homography(float* src, float* dst, float *h);
void FindAllHomography(int n_quadrangles, int* Quadrangles, int* landmarks_src, int* landmarks_dst, float** H);

void ApplyPointHomography(float* h, float *m, float *p);
void ApplyAllHomography(int width, int height, int* imgLabel, float** H, float* XI, float* YI);

#endif //_HOMOGRAPHIE_H_ 