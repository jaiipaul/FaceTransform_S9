#include "biggerface.h"
#include "homographie.h"
#include "utils.h"

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

namespace py = pybind11;

void BF_deformation(float* Landmarks_input, float* Landmarks_output){
    int i;
    //double proportion_x = input[32] - input[0];//difference abscisse entre points 1 et 17
    //double proportion_y = input[17] - input[55];//difference ordonées entre points 9 et 28
    double tableau_modifs[136] = { 0 };

    //CALCULER LES DIFF en X
    //POUR LISNTANT PAS DE DIFF EN X POUR POINTS 8 et 10
    double diffx_3_4 = Landmarks_input[6] - Landmarks_input[4];//points 4 et 14
    double diffx_4_5 = Landmarks_input[8] - Landmarks_input[6];//points 5 et 13
    double diffx_5_6 = Landmarks_input[10] - Landmarks_input[8];//points 6 et 12
    double diffx_6_7 = Landmarks_input[12] - Landmarks_input[10];//points 7 et 11
    double diffx_7_8 = Landmarks_input[14] - Landmarks_input[12];//points 8 et 10 PAS UTILISE

    //CALCULER LES DIFF en Y
    //POUR LISNTANT PAS DE DIFF EN Y POUR POINTS 4 et 14
    double diffy_4_5 = Landmarks_input[9] - Landmarks_input[7];//points 4 et 14 PAS UTILISE
    double diffy_5_6 = Landmarks_input[11] - Landmarks_input[9];//points 5 et 13
    double diffy_6_7 = Landmarks_input[13] - Landmarks_input[11];//points 6 et 12
    double diffy_7_8 = Landmarks_input[15] - Landmarks_input[13];//points 7 et 11
    double diffy_8_9 = Landmarks_input[17] - Landmarks_input[15];//points 8 et 10

    //MODIFICATION X
    double modifX = 1;
    tableau_modifs[6] -= diffx_3_4 / modifX;//point 4
    tableau_modifs[26] += diffx_3_4 / modifX;//point 14

    tableau_modifs[8] -= diffx_4_5 / modifX;//point 5
    tableau_modifs[24] += diffx_4_5 / modifX;//point 13

    tableau_modifs[10] -= diffx_5_6 / modifX;//point 6
    tableau_modifs[22] += diffx_5_6 / modifX;//point 12

    tableau_modifs[12] -= diffx_6_7 / modifX;//point 7
    tableau_modifs[20] += diffx_6_7 / modifX;//point 11

    //MODIFICATION Y
    //IL FAUT FAIRE LA DIFF AVEC CELUI DU BAS ET PAS CELUI DU HAUT
      //  JE DIVISE PAR 4 POUR PAS TROP MODIFIER, A VOIR CE QUE CA DONNE
    double modifY = 1;
    tableau_modifs[9] += diffy_5_6 / modifY;//point 5
    tableau_modifs[25] += diffy_5_6 / modifY;//point 13

    tableau_modifs[11] += diffy_6_7 / modifY;//point 6
    tableau_modifs[23] += diffy_6_7 / modifY;//point 12

    tableau_modifs[13] += diffy_7_8 / modifY;//point 7
    tableau_modifs[21] += diffy_7_8 / modifY;//point 11

    tableau_modifs[15] += diffy_8_9 / modifY;//point 8
    tableau_modifs[19] += diffy_8_9 / modifY;//point 10

    for (i = 0; i < 136; i++)
    {
        Landmarks_output[i] = Landmarks_input[i] + tableau_modifs[i];
    }
}

py::array_t<int> BiggerFace_CPP(py::array_t<int> img_CAM,
                                int width_CAM, int height_CAM,
                                int n_quadrangles, py::array_t<int> Quadrangles, 
                                py::array_t<int> landmarks_CAM){
clock_t time;
    time = clock();

    //Define pointers for image
    py::buffer_info imgCAM_Buff = img_CAM.request();
    int* imgCAM_Ptr = (int*)imgCAM_Buff.ptr;

    //Define pointers for landmark points tab
    //Landmarks = [x0, y0, x1, y1, ... , xn, yn]
    py::buffer_info landmarksCAM_Buff = landmarks_CAM.request();
    int* landmarksCAM_Ptr = (int*)landmarksCAM_Buff.ptr;

    int* landmarks_deform = (int*)calloc(136, sizeof(int));
    BF_deformation((float*)landmarksCAM_Ptr, (float*)landmarks_deform);
    //Define pointer for quadrangle tab
    //Quadrangles = [idx01, idx02, idx03, idx04, idx11, idx12, idx13, idx14, ... , idxn1, idxn2, idxn3, idxn4] 
    py::buffer_info Quadrangles_Buff = Quadrangles.request();
    int* Quadrangles_Ptr = (int*)Quadrangles_Buff.ptr;

    
    //1) Create Labelled image to know wich homographt to apply
    int* imgLabel =NULL;
    imgLabel = (int*)calloc(width_CAM*height_CAM, sizeof(int));
    CreateLabelledImage(n_quadrangles, Quadrangles_Ptr, landmarks_deform, width_CAM, height_CAM, imgLabel);

    //2) Determine all homographies
    float** H = NULL;
    H = (float**)calloc(n_quadrangles, sizeof(float*));
    H[0] = (float*)calloc(9*n_quadrangles, sizeof(float));
    for(int i = 0; i < n_quadrangles; i++){
        H[i] = H[0] + (i*9);
    }
        
    FindAllHomography(n_quadrangles, Quadrangles_Ptr, landmarksCAM_Ptr, landmarks_deform, H);
    //3) Apply homographies and create interpolation grids
    float *XI = NULL, *YI = NULL;
    XI = (float*)calloc(width_CAM*height_CAM, sizeof(float));
    YI = (float*)calloc(width_CAM*height_CAM, sizeof(float));

    //for(int i = 0; i < 100;  i++){
    ApplyAllHomography(width_CAM, height_CAM, imgLabel, H, XI, YI);
    //}
    
    //4) Recreate new image
    auto imgOut = py::array_t<int>(width_CAM*height_CAM*3);
    py::buffer_info imgOut_Buff = imgOut.request();
    int* imgOut_Ptr = (int*)(imgOut_Buff.ptr);
    
    RecreateImage_1(imgOut_Ptr, 
                    imgCAM_Ptr, width_CAM, height_CAM,  
                    XI, YI, imgLabel, 2);


    //Liberation mémoire
    free(H[0]);
    free(H);
    free(XI);
    free(YI);
    free(imgLabel);

    time = clock() - time;

    //std::cout << "Swapping faces took : " << ((float)time)/CLOCKS_PER_SEC << " seconds with C++" << std::endl;

    return(imgOut);
}