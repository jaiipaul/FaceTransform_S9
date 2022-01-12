#include "biggerface.h"

void BF_deformation(float* input, float* output){
    int i;
    //double proportion_x = input[32] - input[0];//difference abscisse entre points 1 et 17
    //double proportion_y = input[17] - input[55];//difference ordonées entre points 9 et 28
    double tableau_modifs[156] = { 0 };

    //CALCULER LES DIFF en X
    //POUR LISNTANT PAS DE DIFF EN X POUR POINTS 8 et 10
    double diffx_3_4 = input[6] - input[4];//points 4 et 14
    double diffx_4_5 = input[8] - input[6];//points 5 et 13
    double diffx_5_6 = input[10] - input[8];//points 6 et 12
    double diffx_6_7 = input[12] - input[10];//points 7 et 11
    double diffx_7_8 = input[14] - input[12];//points 8 et 10 PAS UTILISE

    //CALCULER LES DIFF en Y
    //POUR LISNTANT PAS DE DIFF EN Y POUR POINTS 4 et 14
    double diffy_4_5 = input[9] - input[7];//points 4 et 14 PAS UTILISE
    double diffy_5_6 = input[11] - input[9];//points 5 et 13
    double diffy_6_7 = input[13] - input[11];//points 6 et 12
    double diffy_7_8 = input[15] - input[13];//points 7 et 11
    double diffy_8_9 = input[17] - input[15];//points 8 et 10

    //MODIFICATION X
    tableau_modifs[6] -= diffx_3_4 / (double)2;//point 4
    tableau_modifs[26] += diffx_3_4 / (double)2;//point 14

    tableau_modifs[8] -= diffx_4_5 / (double)2;//point 5
    tableau_modifs[24] += diffx_4_5 / (double)2;//point 13

    tableau_modifs[10] -= diffx_5_6 / (double)2;//point 6
    tableau_modifs[22] += diffx_5_6 / (double)2;//point 12

    tableau_modifs[12] -= diffx_6_7 / (double)2;//point 7
    tableau_modifs[20] += diffx_6_7 / (double)2;//point 11

    //MODIFICATION Y
    //IL FAUT FAIRE LA DIFF AVEC CELUI DU BAS ET PAS CELUI DU HAUT
      //  JE DIVISE PAR 4 POUR PAS TROP MODIFIER, A VOIR CE QUE CA DONNE
    tableau_modifs[9] += diffy_5_6 / (double)4;//point 5
    tableau_modifs[25] += diffy_5_6 / (double)4;//point 13

    tableau_modifs[11] += diffy_6_7 / (double)4;//point 6
    tableau_modifs[23] += diffy_6_7 / (double)4;//point 12

    tableau_modifs[13] += diffy_7_8 / (double)4;//point 7
    tableau_modifs[21] += diffy_7_8 / (double)4;//point 11

    tableau_modifs[15] += diffy_8_9 / (double)4;//point 8
    tableau_modifs[19] += diffy_8_9 / (double)4;//point 10

    for (i = 0; i < 156; i++)
    {
        output[i] = input[i] + tableau_modifs[i];
    }
}