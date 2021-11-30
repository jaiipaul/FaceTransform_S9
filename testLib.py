import sys
sys.path.insert(0, "./cpp/build/Debug")
import faceswap_func as fs
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
##----------------------------------------------------------------------------------------------##

## Find the face
face_to_add1 = cv2.imread("./img/macron.jpg")
face_to_add2 = cv2.imread("./img/macron.jpg")

face_to_add_gray1, faces1 = get_faces(face_to_add1)
face_to_add_gray2, faces2 = get_faces(face_to_add1)

## Get landmarks/Face features
for face in faces1:
        landmarks_FTA_arr1 = get_landmarks(face_to_add_gray1, face)
landmarks_FTA1 = np.int32(landmarks_FTA_arr1.flatten(order='K'))

for face in faces2:
        landmarks_FTA_arr2 = get_landmarks(face_to_add_gray1, face)
landmarks_FTA2 = np.int32(landmarks_FTA_arr2.flatten(order='K'))

for i in range(len(landmarks_FTA2)):
    print(landmarks_FTA1[i]-landmarks_FTA2[i])