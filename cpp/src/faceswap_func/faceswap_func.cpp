#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "faceswap_func.h"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "homographie.h"
namespace py = pybind11;
// Define functions here

void FindAllHomography(int n_quadrangles, int* Quadrangles, int* landmarks_src, int* landmarks_dst, double ** H){
    //printf("Find homographies\n");
    double * Q_coords_src, * Q_coords_dst;
    Q_coords_src = (double*)calloc(8, sizeof(double));
    Q_coords_dst = (double*)calloc(8, sizeof(double));

    for(int i = 0; i < n_quadrangles; i++){
        for(int j = 0; j < 4; j++){
            //Q_coords = [ x0, y0, x1, y1, x2, y2, x3, y3]
            Q_coords_src[2*j]   = (double)landmarks_src[2*Quadrangles[i*4+j]];
            Q_coords_src[2*j+1] = (double)landmarks_src[2*Quadrangles[i*4+j]+1];

            Q_coords_dst[2*j]   = (double)landmarks_dst[2*Quadrangles[i*4+j]];
            Q_coords_dst[2*j+1] = (double)landmarks_dst[2*Quadrangles[i*4+j]+1];
        }
        Find_Homography(Q_coords_dst, Q_coords_src, H[i]);
        //printf("H[%d] = [ %f  %f  %f ]\n        [ %f  %f  %f ]\n        [ %f  %f  %f ]\n", i, H[i][0], H[i][3], H[i][6], H[i][1], H[i][4], H[i][7], H[i][2], H[i][5], H[i][8]);
    }
    free(Q_coords_dst);
    free(Q_coords_src);
}  

void Find_rectangle(double *src,double *rect)
{
    double minx,maxx,miny,maxy;
    minx=maxx=src[0];
    miny=maxy=src[1];
    for(int i=1; i<4;i++){
        if (src[i*2]<minx)
            minx=src[i*2];
        else if (src[i*2]>maxx)
            maxx=src[i*2];
        if (src[i*2+1]<miny)
            miny=src[i*2+1];
        else if (src[i*2+1]>maxy)
            maxy=src[i*2+1];    
    }
    rect[0]=minx;rect[1]=miny;
    rect[2]=maxx;rect[3]=miny;
    rect[4]=maxx;rect[5]=maxy;
    rect[6]=minx;rect[7]=maxy;

}
//TO FINISH
void ApplyAllHomography(int width, int height, int* imgLabel, double ** H, double* XI, double* YI){
    //printf("Apply homographies\n");
    //std::fstream XIfile;
    //XIfile.open("XI.txt", std::fstream::out);
    //std::fstream YIfile;
    //YIfile.open("YI.txt", std::fstream::out);

    /*if(!(XIfile.is_open()) || !(YIfile.is_open())){
        std::cout << "Error opening file" << std::endl;
    }*/
    
    double* m, *p;
    m = (double*)calloc(2, sizeof(double));
    p = (double*)calloc(2, sizeof(double));
    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            if (imgLabel[y*width+x] > 0){
                m[0] = x; m[1] = y;
                ApplyPointHomography(H[imgLabel[y*width+x]-1], m, p);
                
                XI[y*width+x] = p[0];
                YI[y*width+x] = p[1];
                //XIfile << p[0] << " " << std::endl;
                //YIfile << p[1] << " " << std::endl;
            }else{
                XI[y*width+x] = 0;
                YI[y*width+x] = 0;
            }
        }
        //XIfile << std::endl;
        //YIfile << std::endl;
    }
    free(m);
    free(p);
    //XIfile.close();
    //YIfile.close();
}
void RecreateImage(int* imgOut, 
                   int* imgCAM ,int width_CAM, int height_CAM, 
                   int* imgFTA, int width_FTA, int height_FTA, 
                   double* XI, double* YI, int* imgLabel){

    for(int y = 0; y < height_CAM; y++){
        for(int x = 0; x < width_CAM; x++){
            if(imgLabel[y*width_CAM+x] > 0){ 
               
                double x_FTA = floor(XI[y * width_CAM + x]);
                double y_FTA = floor(YI[y * width_CAM + x]);
                //printf("x : %f / y : %f \n", x_FTA, y_FTA);
                if(x_FTA >= 0 && x_FTA < width_FTA && y_FTA >= 0 && y_FTA < height_FTA){
                    imgOut[y * 3*width_CAM + x * 3]     = imgFTA[(int)y_FTA * 3*width_FTA + (int)x_FTA * 3];
                    imgOut[y * 3*width_CAM + x * 3 + 1] = imgFTA[(int)y_FTA * 3*width_FTA + (int)x_FTA * 3 + 1];
                    imgOut[y * 3*width_CAM + x * 3 + 2] = imgFTA[(int)y_FTA * 3*width_FTA + (int)x_FTA * 3 + 2];
                }else{
                    imgOut[y * 3*width_CAM + x * 3]     = 0;
                    imgOut[y * 3*width_CAM + x * 3 + 1] = 0;
                    imgOut[y * 3*width_CAM + x * 3 + 2] = 0;
                }
                
            }else{
                imgOut[y * 3*width_CAM + x * 3]     = imgCAM[y * 3*width_CAM + x*3];
                imgOut[y * 3*width_CAM + x * 3 + 1] = imgCAM[y * 3*width_CAM + x*3 + 1];
                imgOut[y * 3*width_CAM + x * 3 + 2] = imgCAM[y * 3*width_CAM + x*3 + 2];
                //printf("x : %d / y : %d \n", x, y);  
            }
        }
    }
    /*
    for (int y = 0; y < 480; y++) {
        for(int x = 0; x < 640; x++){ 
            if(imgLabel[y*width_CAM+x] > 0){
                int a = ((255-50)/54)*imgLabel[y * width_CAM + x] +50;
                imgOut[y * 3*width_CAM + x * 3] = a;// (imgLabel[y * width_CAM + x]);
                imgOut[y * 3*width_CAM + x * 3 + 1] = 0;// (imgLabel[y * width_CAM + x]);
                imgOut[y * 3*width_CAM + x * 3 + 2] = 255-a;// (imgLabel[y * width_CAM + x]);       
            }else{
                imgOut[y * 3*width_CAM + x * 3]     = imgCAM[y * 3*width_CAM + x*3];
                imgOut[y * 3*width_CAM + x * 3 + 1] = imgCAM[y * 3*width_CAM + x*3 + 1];
                imgOut[y * 3*width_CAM + x * 3 + 2] = imgCAM[y * 3*width_CAM + x*3 + 2];
            }
        }
    }
    */
}

void CreateLabelledImage(int n_quadrangles, int* Quadrangles, int* landmarks, int width, int height, int* imgLabel) {
	//init labelled image
    //printf("Create label image\n");
    for(int i = 0; i < width*height; i++){
        imgLabel[i] = 0; 
    }
    //std::fstream LabelFile;//, XY;
    //LabelFile.open("./tests/label.txt", std::fstream::out);
    //XY.open("./tests/XY.txt", std::fstream::out);
    double* Sqr_coords = (double*)calloc(8, sizeof(double));
    Sqr_coords[0] =  -1.0;
    Sqr_coords[1] =  -1.0;
    Sqr_coords[2] =  1.0;
    Sqr_coords[3] =  -1.0;
    Sqr_coords[4] =  1.0;
    Sqr_coords[5] =  1.0;
    Sqr_coords[6] =  -1.0;
    Sqr_coords[7] =  1.0;

    //printMat(Sqr_coords, 8, 1);
    //printf("Sqr_coords_ptr = [ %f  %f  %f  %f  %f  %f  %f  %f]\n", Sqr_coords_ptr[0], Sqr_coords_ptr[1], Sqr_coords_ptr[2], Sqr_coords_ptr[3], Sqr_coords_ptr[4], Sqr_coords_ptr[5], Sqr_coords_ptr[6], Sqr_coords_ptr[7]);
    double *Q_coords, *Q_box, *H, *m, *p;
    Q_coords = (double*)calloc(8, sizeof(double));
    Q_box    = (double*)calloc(8, sizeof(double));
    H        = (double*)calloc(9, sizeof(double));
    m        = (double*)calloc(2, sizeof(double));
    p        = (double*)calloc(2, sizeof(double));

    for(int i = 0; i < n_quadrangles; i++){
        for(int j = 0; j < 4; j++){
            //Q_coords = [ x0, y0, x1, y1, x2, y2, x3, y3]
            Q_coords[2*j]   = (double)landmarks[2*Quadrangles[i*4 + j]];
            Q_coords[2*j+1] = (double)landmarks[2*Quadrangles[i*4 + j]+1];
        }
        //printf("Q_coords[%d] = [ %f  %f  %f  %f  %f  %f  %f  %f]\n", i, Q_coords[0], Q_coords[1], Q_coords[2], Q_coords[3], Q_coords[4], Q_coords[5], Q_coords[6], Q_coords[7]);
        Find_rectangle(Q_coords, Q_box);
        //Q_box = [ x0, y0, x1, y1, x2, y2, x3, y3]
        Find_Homography(Q_coords, Q_box, H);
        //printf("H[%d] = [ %f  %f  %f ]\n        [ %f  %f  %f ]\n        [ %f  %f  %f ]\n", i, H[0], H[3], H[6], H[1], H[4], H[7], H[2], H[5], H[8]);
        
        //printMat(Q_box, 4, 1);
        //
        //LabelFile << i << std::endl;
        //XY << i << std::endl;
        //LabelFile << "Q_BOX[" << i << "] =" << std::endl;
        //LabelFile << "["<< Q_box[0]<<" "<< Q_box[1]<<" | "<< Q_box[2] << " " << Q_box[3] << "]" << std::endl;       
        //LabelFile << "["<< Q_box[6]<<" "<< Q_box[7]<<" | "<< Q_box[4] << " " << Q_box[5] << "]" << std::endl;      
     
        //LabelFile << "H[" << i << "] =" << std::endl;
        //LabelFile << "["<< H[0]<<" "<< H[3]<<" "<< H[6] << "]" << std::endl;       
        //LabelFile << "["<< H[1]<<" "<< H[4]<<" "<< H[7] << "]" << std::endl;      
        //LabelFile << "["<< H[2]<<" "<< H[5]<<" "<< H[8] << "]" << std::endl;   
        for(int y = Q_box[1]; y < Q_box[5]+1; y++){
            for(int x = Q_box[0]; x < Q_box[2]+1; x++){
                m[0] = (double)x; m[1] = (double)y;
                //printf("m = %f // %f\n", m[0], m[1]);
                ApplyPointHomography(H, m, p);
                //XY << "["<< m[0]<<" "<< m[1]<<" | "<< p[0] << " " << p[1] << "]" << std::endl;
                //if( (x == Q_coords[0] && y == Q_coords[1]) || (x == Q_coords[2] && y == Q_coords[3]) || (x == Q_coords[4] && y == Q_coords[5]) || (x == Q_coords[6] && y == Q_coords[7])){
                    //LabelFile << "o";
                //}
                //else
                if(p[0] >= Q_box[0] && p[0] <= Q_box[2] && p[1] >= Q_box[1] && p[1] <= Q_box[5]){
                    //printf("in %d", i+1);
                    imgLabel[y*width+x] = i+1;
                    //LabelFile << "x";
                }else{
                    //LabelFile << "-";
                }
                
            }
            //LabelFile << std::endl;
        }
        //LabelFile << std::endl;
    }
    
    free(H);
    free(Sqr_coords);
    free(Q_coords);
    free(Q_box);
    free(m);
    free(p);
    //LabelFile.close();
    //XY.close();
}

py::array_t<int> FaceSwap(py::array_t<int> img_CAM, py::array_t<int> img_FTA,
              int width_CAM,int height_CAM, int width_FTA, int height_FTA,
              int n_quadrangles, py::array_t<int> Quadrangles, 
              py::array_t<int> landmarks_CAM, py::array_t<int> landmarks_FTA){
    
    //Define pointers for images
    py::buffer_info imgFTA_Buff = img_FTA.request();
    py::buffer_info imgCAM_Buff = img_CAM.request();

    int* imgFTA_Ptr = (int*)imgFTA_Buff.ptr;
    int* imgCAM_Ptr = (int*)imgCAM_Buff.ptr;

    //Define pointers for landmark points tab
    //Landmarks = [x0, y0, x1, y1, ... , xn, yn]
    py::buffer_info landmarksFTA_Buff = landmarks_FTA.request();
    py::buffer_info landmarksCAM_Buff = landmarks_CAM.request();
    int* landmarksFTA_Ptr = (int*)landmarksFTA_Buff.ptr;
    int* landmarksCAM_Ptr = (int*)landmarksCAM_Buff.ptr;

    //printMat(landmarksCAM_Ptr, 136, 1);
    //Define pointer for quadrangle tab
    //Quadrangles = [idx01, idx02, idx03, idx04, idx11, idx12, idx13, idx14, ... , idxn1, idxn2, idxn3, idxn4] 
    py::buffer_info Quadrangles_Buff = Quadrangles.request();
    int* Quadrangles_Ptr = (int*)Quadrangles_Buff.ptr;

    
    //1) Create Labelled image to know wich homographt to apply
    int* imgLabel =NULL;
    imgLabel = (int*)calloc(width_CAM*height_CAM, sizeof(int));
    CreateLabelledImage(n_quadrangles, Quadrangles_Ptr, landmarksCAM_Ptr, width_CAM, height_CAM, imgLabel);

    //2) Determine all homographies
    double** H = NULL;
    H = (double**)calloc(n_quadrangles, sizeof(double*));
    H[0] = (double*)calloc(9*n_quadrangles, sizeof(double));
    for(int i = 0; i < n_quadrangles; i++){
        H[i] = H[0] + (i*9);
    }
        
    FindAllHomography(n_quadrangles, Quadrangles_Ptr, landmarksFTA_Ptr, landmarksCAM_Ptr, H);
    //3) Apply homographies and create interpolation grids
    double *XI = NULL, *YI = NULL;
    XI = (double*)calloc(width_CAM*height_CAM, sizeof(double));
    YI = (double*)calloc(width_CAM*height_CAM, sizeof(double));
    ApplyAllHomography(width_CAM, height_CAM, imgLabel, H, XI, YI);
    
    //4) Recreate new image
    auto imgOut = py::array_t<int>(width_CAM*height_CAM*3);
    py::buffer_info imgOut_Buff = imgOut.request();
    int* imgOut_Ptr = (int*)(imgOut_Buff.ptr);
    
    
    //printf("Recreate image\n");
    RecreateImage(imgOut_Ptr, 
                  imgCAM_Ptr, width_CAM, height_CAM, 
                  imgFTA_Ptr, width_FTA, height_FTA, 
                  XI, YI, imgLabel);
    
    /*for (int i = 0; i < width_CAM * height_CAM; i++) {
        imgOut_Ptr[i] = imgLabel[i];
    }*/
    //printf("CPP done");

    //Liberation mémoire
    
    free(H[0]);
    free(H);
    free(XI);
    free(YI);
    free(imgLabel);

    return(imgOut);
}


