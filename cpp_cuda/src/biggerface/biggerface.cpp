#include "biggerface.h"
#include "homographie.h"

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

void BF_FindAllHomography(int n_quadrangles, int* Quadrangles, int* landmarks_src, int* landmarks_dst, float ** H){
    //printf("Find homographies\n");
    float * Q_coords_src, * Q_coords_dst;
    Q_coords_src = (float*)calloc(8, sizeof(float));
    Q_coords_dst = (float*)calloc(8, sizeof(float));

    for(int i = 0; i < n_quadrangles; i++){
        for(int j = 0; j < 4; j++){
            //Q_coords = [ x0, y0, x1, y1, x2, y2, x3, y3]
            Q_coords_src[2*j]   = (float)landmarks_src[2*Quadrangles[i*4+j]];
            Q_coords_src[2*j+1] = (float)landmarks_src[2*Quadrangles[i*4+j]+1];

            Q_coords_dst[2*j]   = (float)landmarks_dst[2*Quadrangles[i*4+j]];
            Q_coords_dst[2*j+1] = (float)landmarks_dst[2*Quadrangles[i*4+j]+1];
        }
        CV_Find_Homography(Q_coords_src, Q_coords_dst, H[i]);
        //printf("H[%d] = [ %f  %f  %f ]\n        [ %f  %f  %f ]\n        [ %f  %f  %f ]\n", i, H[i][0], H[i][3], H[i][6], H[i][1], H[i][4], H[i][7], H[i][2], H[i][5], H[i][8]);
    }
    free(Q_coords_dst);
    free(Q_coords_src);
}  

void BF_Find_rectangle(float *src, float *rect)
{
    float minx,maxx,miny,maxy;
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

void BF_ApplyAllHomography(int width, int height, int* imgLabel, float** H, float* XI, float* YI){
    float* m, *p;
    m = (float*)calloc(2, sizeof(float));
    p = (float*)calloc(2, sizeof(float));
    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            if (imgLabel[y*width+x] > 0){
                m[0] = x; m[1] = y;
                ApplyPointHomography(H[imgLabel[y*width+x]-1], m, p);
                
                XI[y*width+x] = p[0];
                YI[y*width+x] = p[1];
            }else{
                XI[y*width+x] = 0;
                YI[y*width+x] = 0;
            }
        }
    }
    free(m);
    free(p);
}

void BF_RecreateImage(int* imgOut, 
                   int* imgCAM ,int width_CAM, int height_CAM, 
                   float* XI, float* YI, int* imgLabel){ 
    for(int y = 0; y < height_CAM; y++){
        for(int x = 0; x < width_CAM; x++){
            if(imgLabel[y*width_CAM+x] > 0){ 
               
                int x_FTA = floor(XI[y * width_CAM + x]);
                int y_FTA = floor(YI[y * width_CAM + x]);
                if(x_FTA >= 0 && x_FTA < width_CAM && y_FTA >= 0 && y_FTA < height_CAM){
                    float dx_FTA = fmod(XI[y * width_CAM + x],x_FTA);
                    float dy_FTA = fmod(YI[y * width_CAM + x],y_FTA);

                    for(int c = 0; c < 3; c++){
                        int pixel_value =   (1.f-dx_FTA)*(1.f-dy_FTA)*imgCAM[y_FTA * 3*width_CAM + x_FTA * 3 + c]
                                          + dx_FTA*(1.f-dy_FTA)*imgCAM[y_FTA * 3*width_CAM + (x_FTA + 1) * 3 + c]
                                          + dy_FTA*(1.f-dx_FTA)*imgCAM[(y_FTA + 1) * 3*width_CAM + x_FTA * 3 + c]
                                          + dy_FTA*dx_FTA*imgCAM[(y_FTA + 1) * 3*width_CAM + (x_FTA + 1) * 3 + c];

                        imgOut[y * 3*width_CAM + x * 3 + c] = pixel_value;
                    }
                
                //printf("x : %f / y : %f \n", x_FTA, y_FTA);
                // if(x_FTA >= 0 && x_FTA < width_FTA && y_FTA >= 0 && y_FTA < height_FTA){
                //     imgOut[y * 3*width_CAM + x * 3]     = imgFTA[(int)y_FTA * 3*width_FTA + (int)x_FTA * 3];
                //     imgOut[y * 3*width_CAM + x * 3 + 1] = imgFTA[(int)y_FTA * 3*width_FTA + (int)x_FTA * 3 + 1];
                //     imgOut[y * 3*width_CAM + x * 3 + 2] = imgFTA[(int)y_FTA * 3*width_FTA + (int)x_FTA * 3 + 2];
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
    int colors[53][3];
    for(int i = 0; i < 53; i++){
        colors[i][0] = rand()%255;
        colors[i][1] = rand()%255;
        colors[i][2] = rand()%255;
    }

    for (int y = 0; y < height_CAM; y++) {
        for(int x = 0; x <width_CAM; x++){ 
            int label = imgLabel[y*width_CAM+x];
            if(imgLabel[y*width_CAM+x] > 0){
                //int a = ((255-50)/54)*imgLabel[y * width_CAM + x] +50;
                imgOut[y * 3*width_CAM + x * 3]     = colors[label-1][0];// (imgLabel[y * width_CAM + x]);
                imgOut[y * 3*width_CAM + x * 3 + 1] = colors[label-1][1];// (imgLabel[y * width_CAM + x]);
                imgOut[y * 3*width_CAM + x * 3 + 2] = colors[label-1][2];// (imgLabel[y * width_CAM + x]);       
            }else{
                imgOut[y * 3*width_CAM + x * 3]     = imgCAM[y * 3*width_CAM + x*3];
                imgOut[y * 3*width_CAM + x * 3 + 1] = imgCAM[y * 3*width_CAM + x*3 + 1];
                imgOut[y * 3*width_CAM + x * 3 + 2] = imgCAM[y * 3*width_CAM + x*3 + 2];
            }
        }
    }
    */
}

void BF_CreateLabelledImage(int n_quadrangles, int* Quadrangles, int* landmarks, int width, int height, int* imgLabel) {
	//init labelled image
    //printf("Create label image\n");
    for(int i = 0; i < width*height; i++){
        imgLabel[i] = 0; 
    }
    //std::fstream LabelFile;//, XY;
    //LabelFile.open("./tests/label.txt", std::fstream::out);
    //XY.open("./tests/XY.txt", std::fstream::out);
    float* Sqr_coords = (float*)calloc(8, sizeof(float));
    Sqr_coords[0] =  -100.0f;
    Sqr_coords[1] =  -100.0f;
    Sqr_coords[2] =  100.0f;
    Sqr_coords[3] =  -100.0f;
    Sqr_coords[4] =  100.0f;
    Sqr_coords[5] =  100.0f;
    Sqr_coords[6] =  -100.0f;
    Sqr_coords[7] =  100.0f;

    //printMat(Sqr_coords, 8, 1);
    //printf("Sqr_coords_ptr = [ %f  %f  %f  %f  %f  %f  %f  %f]\n", Sqr_coords_ptr[0], Sqr_coords_ptr[1], Sqr_coords_ptr[2], Sqr_coords_ptr[3], Sqr_coords_ptr[4], Sqr_coords_ptr[5], Sqr_coords_ptr[6], Sqr_coords_ptr[7]);
    float *Q_coords, *Q_box, *H, *m, *p;
    Q_coords = (float*)calloc(8, sizeof(float));
    Q_box    = (float*)calloc(8, sizeof(float));
    H        = (float*)calloc(9, sizeof(float));
    m        = (float*)calloc(2, sizeof(float));
    p        = (float*)calloc(2, sizeof(float));

    for(int i = 0; i < n_quadrangles; i++){
        for(int j = 0; j < 4; j++){
            //Q_coords = [ x0, y0, x1, y1, x2, y2, x3, y3]
            Q_coords[2*j]   = (float)landmarks[2*Quadrangles[i*4 + j]];
            Q_coords[2*j+1] = (float)landmarks[2*Quadrangles[i*4 + j]+1];
        }
        //printf("Q_coords[%d] = [ %f  %f  %f  %f  %f  %f  %f  %f]\n", i, Q_coords[0], Q_coords[1], Q_coords[2], Q_coords[3], Q_coords[4], Q_coords[5], Q_coords[6], Q_coords[7]);
        BF_Find_rectangle(Q_coords, Q_box);
        //Q_box = [ x0, y0, x1, y1, x2, y2, x3, y3]
        CV_Find_Homography(Sqr_coords, Q_coords, H);
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
                m[0] = (float)x; m[1] = (float)y;
                //printf("m = %f // %f\n", m[0], m[1]);
                ApplyPointHomography(H, m, p);
                //XY << "["<< m[0]<<" "<< m[1]<<" | "<< p[0] << " " << p[1] << "]" << std::endl;
                //if( (x == Q_coords[0] && y == Q_coords[1]) || (x == Q_coords[2] && y == Q_coords[3]) || (x == Q_coords[4] && y == Q_coords[5]) || (x == Q_coords[6] && y == Q_coords[7])){
                    //LabelFile << "o";
                //}
                //else
                /*if(p[0] >= Q_box[0] && p[0] <= Q_box[2] && p[1] >= Q_box[1] && p[1] <= Q_box[5]){*/
                if(abs(p[0]) <= 100.0 && abs(p[1]) <= 100.0){ 
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
    BF_CreateLabelledImage(n_quadrangles, Quadrangles_Ptr, landmarks_deform, width_CAM, height_CAM, imgLabel);

    //2) Determine all homographies
    float** H = NULL;
    H = (float**)calloc(n_quadrangles, sizeof(float*));
    H[0] = (float*)calloc(9*n_quadrangles, sizeof(float));
    for(int i = 0; i < n_quadrangles; i++){
        H[i] = H[0] + (i*9);
    }
        
    BF_FindAllHomography(n_quadrangles, Quadrangles_Ptr, landmarksCAM_Ptr, landmarks_deform, H);
    //3) Apply homographies and create interpolation grids
    float *XI = NULL, *YI = NULL;
    XI = (float*)calloc(width_CAM*height_CAM, sizeof(float));
    YI = (float*)calloc(width_CAM*height_CAM, sizeof(float));

    //for(int i = 0; i < 100;  i++){
    BF_ApplyAllHomography(width_CAM, height_CAM, imgLabel, H, XI, YI);
    //}
    
    //4) Recreate new image
    auto imgOut = py::array_t<int>(width_CAM*height_CAM*3);
    py::buffer_info imgOut_Buff = imgOut.request();
    int* imgOut_Ptr = (int*)(imgOut_Buff.ptr);
    
    
    //printf("Recreate image\n");
    BF_RecreateImage(imgOut_Ptr, 
                     imgCAM_Ptr, width_CAM, height_CAM,  
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

    time = clock() - time;

    //std::cout << "Swapping faces took : " << ((float)time)/CLOCKS_PER_SEC << " seconds with C++" << std::endl;

    return(imgOut);
}