import sys
sys.path.insert(0, "../lib/Release")
sys.path.insert(0, "../ext/dlib/build/lib")
sys.path.insert(0, "../ext/opencv")

import FaceTransform as FT
import matplotlib.pyplot as plt
import numpy as np
import dlib 
import cv2 

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
##-------------------------------------------------------------------------------------------------##
##ADDING BEARD ALGORITHM
cap = cv2.VideoCapture(0, cv2.CAP_ANY) # ouvrir la caméra 
_, temp = cap.read()
#tempBGRA = cv2.cvtColor(temp, cv2.COLOR_BGR2BGRA)
width_CAM = np.int32(len(temp[1,:,1]))
height_CAM = np.int32(len(temp[:,1,1]))

## WEBCAM FACE TREATEMENT // PASTE THE FIRST FACE ON THE FACES CAPTURED BY THE CAMERA
while cap.isOpened():   
    ##Find faces
    _, camera_img_arr = cap.read()
    img_CAM = camera_img_arr.flatten(order='C')
    camera_img_gray, camera_faces = get_faces(camera_img_arr)
    camera_img_new_face = np.zeros_like(camera_img_arr) 

    img_Out = np.ndarray((height_CAM, width_CAM, 4), dtype=np.uint8)
    img_Out_line = np.uint8(img_CAM)
    if (0 < len(camera_faces)):
        for face in camera_faces:
            landmarks_CAM_arr = get_landmarks(camera_img_gray, face)
            landmarks_CAM = np.int32(landmarks_CAM_arr.flatten(order='C'))
            points_CAM = np.array(landmarks_CAM_arr, dtype=np.int32)
            #hull_camera = cv2.convexHull(points_CAM)

            img_Out_line = FT.AddBeard_CPP(img_Out_line, width_CAM, height_CAM, landmarks_CAM)
            print("Added beard")
            img_Out = np.uint8(np.reshape(img_Out_line, (height_CAM, width_CAM, 3), order='C'))
            #fs.loadImage(img_CAM, width_CAM, height_CAM)

            #img_Out = camera_img_arr;
            #for i in range(n_quadrangles):
            #    for j in range(4):
            #        point1 = (landmarks_CAM[2*Quadrangles_arr[i,j]], landmarks_CAM[2*Quadrangles_arr[i,j] + 1])
            #        point2 = (landmarks_CAM[2*Quadrangles_arr[i,(j+1)%4]], landmarks_CAM[2*Quadrangles_arr[i,(j+1)%4] + 1])
            #        
            #        img_Out = cv2.line(img_Out, point1, point2, (0, 255, 0), 1)

        #img_OutRGB = cv2.cvtColor(img_Out, cv2.COLOR_BGRA2BGR)
        result = cv2.flip(img_Out, 1)
        cv2.imshow("Beard", result)
    else:
        result = cv2.flip(camera_img_arr, 1)
        cv2.imshow("Beard", result)
    if cv2.waitKey(1) == ord('q'):
        cv2.destroyAllWindows()
        break      

cap.release()
cv2.destroyAllWindows()