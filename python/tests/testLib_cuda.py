import sys
sys.path.insert(0, "../../lib/Release")
import FaceTransform as FT
import matplotlib.pyplot as plt
import numpy as np
import dlib 
import cv2 
import time

#Face detector
detector = dlib.get_frontal_face_detector()
#Landmarks model
predictor = dlib.shape_predictor("../shape_predictor_68_face_landmarks.dat")

## FIND FACE IN THE IMAGE
def get_faces(img):
    img_gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    faces = detector(img_gray)
    print("Found ")
    print(len(faces))
    return img_gray, faces
##----------------------------------------------------------------------------------------------##
## FIND FACE FEATURES AND LANDMARKS 
def get_landmarks(img_gray, face):
    landmarks=predictor(img_gray, face)
    landmarks_point = []
    for n in range(0, 68):
        x = landmarks.part(n).x
        y = landmarks.part(n).y
        landmarks_point.append((x, y))
    return np.array(landmarks_point)
##----------------------------------------------------------------------------------------------##
def set_quadranglesA():
    q    =   np.zeros((53,4))
    #CONTOUR VISAGE
    q[0] =   np.array([1,2,37,18])
    q[1] =   np.array([2,3,42,37])
    q[2] =   np.array([3,4,49,42])
    q[3] =   np.array([4,5,6,49])
    q[4] =   np.array([6,7,60,49])
    q[5] =   np.array([7,8,59,60])
    q[6] =   np.array([8,9,58,59])
    q[7] =   np.array([9,10,57,58])
    q[8] =   np.array([10,11,56,57])
    q[9] =   np.array([11,12,55,56])
    q[10] =  np.array([12,13,14,55])
    q[11] =  np.array([14,15,47,55])
    q[12] =  np.array([15,16,46,47])
    q[13] =  np.array([17,27,46,16])
    q[14] =  np.array([27,26,45,46])
    q[15] =  np.array([26,25,24,45])
    q[16] =  np.array([24,23,44,45])
    q[17] =  np.array([23,28,43,44])
    q[18] =  np.array([28,22,39,40])
    q[19] =  np.array([22,21,38,39])
    q[20] =  np.array([21,20,19,38])
    q[21] =  np.array([19,18,37,38])
    q[22] =  np.array([41,42,49,50])
    q[23] =  np.array([40,41,50,32])
    q[24] =  np.array([47,48,54,55])
    q[25] =  np.array([48,43,36,54])
    #YEUX
    q[26] =  np.array([38,37,42,41])
    q[27] =  np.array([39,38,41,40])
    q[28] =  np.array([45,44,43,48])
    q[29] =  np.array([45,48,47,46])
    #NEZ
    q[30] =  np.array([28,40,29,43])
    q[31] =  np.array([29,40,32,30])
    q[32] =  np.array([30,32,33,31])
    q[33] =  np.array([31,33,34,35])
    q[34] =  np.array([33,32,50,51])
    q[35] =  np.array([43,29,30,36])
    q[36] =  np.array([30,31,35,36])
    q[37] =  np.array([34,52,53,35])
    q[38] =  np.array([36,35,53,54])
    q[39] =  np.array([33,51,52,34])
    #BOUCHE
    q[40] =  np.array([51,50,49,61])
    q[41] =  np.array([51,61,68,62])
    q[42] =  np.array([52,51,62,63])
    q[43] =  np.array([53,52,63,64])
    q[44] =  np.array([54,53,65,55])
    q[45] =  np.array([53,64,66,65])
    q[46] =  np.array([62,68,67,63])
    q[47] =  np.array([63,67,66,64])
    q[48] =  np.array([61,49,60,68])
    q[49] =  np.array([68,60,59,67])
    q[50] =  np.array([67,59,58,57])
    q[51] =  np.array([66,67,57,56])
    q[52] =  np.array([65,66,56,55])
    for i in range(53):
        for j in range(4):
            q[i][j] = q[i][j] - 1 
    return q

def set_quadranglesB():
    q    =   np.zeros((53,4))
    #CONTOUR VISAGE
    q[0] =   np.array([1,2,42,37])
    q[1] =   np.array([2,3,32,42])
    q[2] =   np.array([3,4,50,32])
    q[3] =   np.array([4,5,49,50])
    q[4] =   np.array([5,60,61,49])
    q[5] =   np.array([5,6,59,60])
    q[6] =   np.array([6,7,58,59])
    q[7] =   np.array([7,8,9,58])
    q[8] =   np.array([9,10,11,58])
    q[9] =   np.array([11,12,57,58])
    q[10] =  np.array([12,13,56,57])
    q[11] =  np.array([13,55,65,56])
    q[12] =  np.array([13,14,54,55])
    q[13] =  np.array([14,15,36,54])
    q[14] =  np.array([15,16,47,36])
    q[15] =  np.array([16,17,46,47])
    q[16] =  np.array([17,27,26,46])
    q[17] =  np.array([26,25,45,46])
    q[18] =  np.array([25,24,44,45])
    q[19] =  np.array([24,23,43,44])
    q[20] =  np.array([23,28,29,43])
    q[21] =  np.array([28,22,40,29])
    q[22] =  np.array([22,21,39,40])
    q[23] =  np.array([21,20,38,39])
    q[24] =  np.array([20,19,37,38])
    q[25] =  np.array([19,18,1,37])
    #YEUX
    q[26] =  np.array([38,37,42,41])
    q[27] =  np.array([39,38,41,40])
    q[28] =  np.array([45,44,43,48])
    q[29] =  np.array([45,48,47,46])
    #NEZ
    q[30] =  np.array([40,41,30,29])
    q[31] =  np.array([41,42,32,30])
    q[32] =  np.array([30,32,33,31])
    q[33] =  np.array([31,33,34,35])
    q[34] =  np.array([33,32,50,51])
    q[35] =  np.array([43,29,30,48])
    q[36] =  np.array([48,30,36,47])
    q[37] =  np.array([30,31,35,36])
    q[38] =  np.array([34,52,53,35])
    q[39] =  np.array([36,35,53,54])
    q[40] =  np.array([33,51,52,34])
    #BOUCHE
    q[41] =  np.array([51,50,49,61])
    q[42] =  np.array([51,61,62,52])
    q[43] =  np.array([52,62,63,64])
    q[44] =  np.array([53,52,64,65])
    q[45] =  np.array([54,53,65,55])
    q[46] =  np.array([62,61,60,68])
    q[47] =  np.array([63,62,68,67])
    q[48] =  np.array([64,63,67,66])
    q[49] =  np.array([65,64,66,56])
    q[50] =  np.array([68,60,59,67])
    q[51] =  np.array([67,59,58,57])
    q[52] =  np.array([66,67,57,56])
    for i in range(53):
        for j in range(4):
            q[i][j] = q[i][j] - 1 
    return q

## Find the face
face_to_add1_arr = cv2.imread("../../img/singe.jpg")
face_to_add1 = face_to_add1_arr.flatten(order='C')

faceswapped2_arr = cv2.imread("../../img/macron.jpg")
faceswapped2 = faceswapped2_arr.flatten(order='C')

width_FTA = np.int32(len(face_to_add1_arr[1,:,1]))
height_FTA = np.int32(len(face_to_add1_arr[:,1,1]))

width_FSD = np.int32(len(faceswapped2_arr[1,:,1]))
height_FSD = np.int32(len(faceswapped2_arr[:,1,1]))

face_to_add_gray1, faces1 = get_faces(face_to_add1_arr)
faceswapped2_gray2, faces2 = get_faces(faceswapped2_arr)

## Get landmarks/Face features
for face in faces1:
        landmarks_FTA_arr1 = get_landmarks(face_to_add_gray1, face)
landmarks_FTA1 = np.int32(landmarks_FTA_arr1.flatten(order='K'))

for face in faces2:
        landmarks_FSD_arr2 = get_landmarks(faceswapped2_gray2, face)
landmarks_FSD2 = np.int32(landmarks_FSD_arr2.flatten(order='K'))

Quadrangles_arr = np.int32(set_quadranglesA())
Quadrangles = np.int32(Quadrangles_arr.flatten(order='C'))

img_Out = np.ndarray((height_FSD, width_FSD, 3), dtype=np.uint8)
img_Out_line = faceswapped2

n_quadrangles = np.int32(len(Quadrangles_arr))
#start = time.time()
img_Out_line = fs.FaceSwap_CUDA(img_Out_line, face_to_add1, width_FSD, height_FSD, width_FTA, height_FTA, n_quadrangles, Quadrangles, landmarks_FSD2, landmarks_FTA1)
#end = time.time()
#print("Swapping faces took : "+ str(end-start)+" seconds with CUDA")
img_Out = np.uint8(np.reshape(img_Out_line, (height_FSD, width_FSD, 3), order='C'))

#for i in range(n_quadrangles):
#    for j in range(4):
#        point1 = (landmarks_FSD2[2*Quadrangles_arr[i,j]], landmarks_FSD2[2*Quadrangles_arr[i,j] + 1])
#        point2 = (landmarks_FSD2[2*Quadrangles_arr[i,(j+1)%4]], landmarks_FSD2[2*Quadrangles_arr[i,(j+1)%4] + 1])
#        
#        img_Out = cv2.line(img_Out, point1, point2, (0, 255, 0), 1)       


#result = cv2.flip(img_Out, 1)
cv2.imshow("faceswap.jpg", img_Out)
cv2.waitKey(0)
