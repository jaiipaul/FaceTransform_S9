import sys
sys.path.insert(0, "./cpp/build/Debug")

import faceswap_func as fs
import dlib 
import cv2 
import numpy as np 

#Face detector
detector = dlib.get_frontal_face_detector()
#Landmarks model
predictor = dlib.shape_predictor("shape_predictor_68_face_landmarks.dat")

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
## SET QUADRANGLE BASED ON LANDMARKS INDEX
#def set_quadrangles():
#    q    =   np.zeros((57,4))
#    q[0] =   np.array([0,0,0,0])
#    q[1] =   np.array([8,9,57,56])
#    q[2] =   np.array([10,9,55,56])
#    q[3] =   np.array([10,11,55,64])
#    q[4] =   np.array([12,11,54,64])
#    q[5] =   np.array([12,35,54,13])
#    q[6] =   np.array([57,56,65,66])
#    q[7] =   np.array([55,56,65,64])
#    q[8] =   np.array([62,63,65,66])
#    q[9] =   np.array([64,63,65,53])
#    q[10] =  np.array([64,53,54,35])
#    q[11] =  np.array([63,53,52,35])
#    q[12] =  np.array([62,63,51,52])
#    q[13] =  np.array([34,35,51,52])
#    q[14] =  np.array([34,50,51,33])
#    q[15] =  np.array([34,30,35,33])
#    q[16] =  np.array([14,30,35,13])
#    q[17] =  np.array([14,30,15,29])
#    q[18] =  np.array([47,46,15,29])
#    q[19] =  np.array([45,46,15,16])
#    q[20] =  np.array([45,44,26,16])
#    q[21] =  np.array([47,46,45,44])
#    q[22] =  np.array([24,25,26,44])
#    q[23] =  np.array([24,23,43,44])
#    q[24] =  np.array([42,47,43,44])
#    q[25] =  np.array([42,22,43,23])
#    q[26] =  np.array([28,22,27,42])
#    q[27] =  np.array([28,29,47,42])
#    q[28] =  np.array([21,22,27,42])
#    q[29] =  np.array([21,22,20,23])
#    q[30] =  np.array([19,24,20,23])
#    q[31] =  np.array([21,38,20,39])
#    q[32] =  np.array([19,38,20,37])
#    q[33] =  np.array([17,18,19,37])
#    q[34] =  np.array([17,0,36,37])
#    q[35] =  np.array([1,0,36,41])
#    q[36] =  np.array([37,38,36,41])
#    q[37] =  np.array([38,39,40,41])
#    q[38] =  np.array([21,27,28,39])
#    q[39] =  np.array([1,2,40,41])
#    q[40] =  np.array([28,2,40,39])
#    q[41] =  np.array([2,3,29,28])
#    q[42] =  np.array([4,3,29,31])
#    q[43] =  np.array([4,5,48,31])
#    q[44] =  np.array([59,5,48,60])
#    q[45] =  np.array([6,5,59,58])
#    q[46] =  np.array([6,7,57,58])
#    q[47] =  np.array([8,7,57,56])
#    q[48] =  np.array([67,58,57,66])
#    q[49] =  np.array([67,66,61,62])
#    q[50] =  np.array([50,51,61,62])
#    q[51] =  np.array([30,32,33,50])
#    q[52] =  np.array([31,32,30,29])
#    q[53] =  np.array([31,32,49,61])
#    q[54] =  np.array([48,60,49,31])
#    q[55] =  np.array([49,60,67,61])
#    q[56] =  np.array([58,59,67,60])
#    return q
#
def set_quadrangles():
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
    q[17] =  np.array([26,25,45,26])
    q[18] =  np.array([25,24,44,45])
    q[19] =  np.array([24,23,43,44])
    q[20] =  np.array([23,28,29,43])
    q[21] =  np.array([28,22,40,29])
    q[22] =  np.array([22,21,39,40])
    q[23] =  np.array([21,20,38,39])
    q[24] =  np.array([20,19,37,38])
    q[25] =  np.array([18,19,1,37])
    #YEUX
    q[26] =  np.array([38,37,42,41])
    q[27] =  np.array([39,38,41,40])
    q[28] =  np.array([45,44,43,48])
    q[29] =  np.array([45,58,47,46])
    #NEZ
    q[30] =  np.array([40,41,30,29])
    q[31] =  np.array([41,42,32,30])
    q[32] =  np.array([30,32,33,31])
    q[33] =  np.array([31,33,51,34])
    q[34] =  np.array([33,32,50,51])
    q[35] =  np.array([43,29,30,48])
    q[36] =  np.array([48,30,36,47])
    q[37] =  np.array([30,31,35,36])
    q[38] =  np.array([31,34,53,35])
    q[39] =  np.array([36,35,53,54])
    q[40] =  np.array([34,51,52,53])
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
    return q

##-------------------------------------------------------------------------------------------------##
##FRACE SWAPPING ALGORITHM
cap = cv2.VideoCapture(0, cv2.CAP_ANY) # ouvrir la caméra 
_, temp = cap.read()
width_CAM = np.int32(len(temp[1,:,1]))
height_CAM = np.int32(len(temp[:,1,1]))

## FIRST FACE TREATEMENT // THE ALGORITHM WILL PUT THIS FACE ON THE ONE CAPTURE BY THE CAMERA
## Find the face
face_to_add = cv2.imread("./img/macron.jpg")
width_FTA = np.int32(len(face_to_add[1,:,1]))
height_FTA = np.int32(len(face_to_add[:,1,1]))
face_to_add_gray, faces = get_faces(face_to_add)
mask = np.zeros_like(face_to_add_gray)

img_FTA = face_to_add.flatten(order='K')
## Get landmarks/Face features
for face in faces:
        landmarks_FTA_arr = get_landmarks(face_to_add_gray, face)
landmarks_FTA = np.int32(landmarks_FTA_arr.flatten(order='K'))

## Set Quadrangles for homography
Quadrangles_arr = set_quadrangles()
Quadrangles = np.int32(Quadrangles_arr.flatten(order='K'))
## WEBCAM FACE TREATEMENT // PASTE THE FIRST FACE ON THE FACES CAPTURED BY THE CAMERA
while cap.isOpened():   
    ##Find faces
    _, camera_img_arr = cap.read()
    img_CAM = camera_img_arr.flatten(order='K')
    camera_img_gray, camera_faces = get_faces(camera_img_arr)
    camera_img_new_face = np.zeros_like(camera_img_arr) 

    if (0 < len(camera_faces)):
        for face in camera_faces:
            landmarks_CAM_arr = get_landmarks(camera_img_gray, face)
            landmarks_CAM = np.int32(landmarks_CAM_arr.flatten(order='K'))
            points_CAM = np.array(landmarks_CAM_arr, dtype=np.int32)
            hull_camera = cv2.convexHull(points_CAM)

            
            n_quadrangles = np.int32(len(Quadrangles_arr))
            img_Out = np.ndarray((height_CAM, width_CAM, 3), dtype=np.uint8)
            img_Out_line = fs.FaceSwap(img_CAM, img_FTA, width_CAM, height_CAM, width_FTA, height_FTA, n_quadrangles, Quadrangles, landmarks_CAM, landmarks_FTA)
            #img_Out = np.uint8(np.reshape(img_Out_line, (height_CAM, width_CAM, 3)))
            #fs.loadImage(img_CAM, width_CAM, height_CAM)


            Dst_face_mask = np.zeros_like(camera_img_gray)
            Dst_head_mask = cv2.fillConvexPoly(Dst_face_mask, hull_camera, 255)
            Dst_face_mask = cv2.bitwise_not(Dst_head_mask)
    
    #maskage
            img2Noface = cv2.bitwise_and(camera_img_arr, camera_img_arr, mask=Dst_face_mask)
            imgOut = cv2.add(img2Noface, camera_img_new_face)
    
            (x, y, w, h) = cv2.boundingRect(hull_camera)
            center_face2 = (int((x + x + w) / 2), int((y + y + h) / 2))
    
    # Pour supprimer les lignes 
    # On copie le contenu de imgOut dans imgDst 
        seamlessclone = cv2.seamlessClone(imgOut, camera_img_arr, Dst_head_mask, center_face2, cv2.NORMAL_CLONE)
        result = cv2.flip(seamlessclone, 1)
        cv2.imshow("FaceSwap", img_Out)
    else:
        result = cv2.flip(camera_img_arr, 1)
        cv2.imshow("FaceSwap", result)
    if cv2.waitKey(1) == ord('q'):
        cv2.destroyAllWindows()
        break      

cap.release()
cv2.destroyAllWindows()