
#include "Image.h"
#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
namespace py = pybind11;




void generateParam(int* landmarksPtr, int lenLand, int* moustacheLeftSide, int* moustacheTopSide, int* moustacheWidth){

    *moustacheLeftSide = 0; 
    
    *moustacheTopSide = 0; 
    int moustacheRightSide = 0; 
    int moustacheBottomSide = 0; 
    int x, y; 

    for (int i=0; i < lenLand; i++){
         x = landmarksPtr[2*i]; 
         y = landmarksPtr[2*i+1]; 
         //if (i + 1 == 37) {eyeLeftSide = x - 40; }
         //if (i + 1 == 38) {eyeTopSide = y - 30; }
         //if (i + 1 == 46) {eyeRightSide = x + 40; }
         //if (i + 1 == 48) {eyeBottomSide = y + 30; }
     
         if (i + 1 == 2){
             *moustacheLeftSide = x;
             *moustacheTopSide = y - 10;
             }

         if (i + 1 == 16) {moustacheRightSide = x; }
         if (i + 1 == 9) {moustacheBottomSide = y; }
    }

    //int eyesWidth= abs(eyeRightSide - eyeLeftSide)
    *moustacheWidth= abs(moustacheRightSide - *moustacheLeftSide);
}

py::array_t<uint8_t> AddBeard_CPP(py::array_t<uint8_t> img_CAM, 
                              int width_CAM,int height_CAM,
                              py::array_t<int> landmarks_CAM){
    //Define pointers for images
    py::buffer_info imgCAM_Buff = img_CAM.request();

    uint8_t* imgCAM_Ptr = (uint8_t*)imgCAM_Buff.ptr;

    //Define pointers for landmark points tab
    //Landmarks = [x0, y0? x1, y1, ... , xn, yn]
    py::buffer_info landmarksCAM_Buff = landmarks_CAM.request();
    int* landmarksCAM_Ptr = (int*)landmarksCAM_Buff.ptr;
 
    int moustacheTopSide, moustacheWidth, moustacheLeftSide; 

    generateParam(landmarksCAM_Ptr, 64, &moustacheLeftSide, &moustacheTopSide, &moustacheWidth);
    // les frames sont générées par le code python et stockées dans le répértoire du projet 
    // de imwrite dans le python 
    Image Face(width_CAM, height_CAM, 3);
    memcpy(Face.data, imgCAM_Ptr, width_CAM*height_CAM*3*sizeof(uint8_t));

    Image Beard("../img/fitedMoustache2.png", 4);

    float p = Beard.h/float(Beard.w);
    Beard.resize(floor(p*moustacheWidth),moustacheWidth);

    Face.overlay(Beard, moustacheLeftSide ,moustacheTopSide); 

    auto imgOut = py::array_t<uint8_t>(width_CAM*height_CAM*3);
    py::buffer_info imgOut_Buff = imgOut.request();
    uint8_t* imgOut_Ptr = (uint8_t*)(imgOut_Buff.ptr);

    memcpy(imgOut_Ptr, Face.data, width_CAM*height_CAM*3*sizeof(uint8_t));
    return(imgOut);
}