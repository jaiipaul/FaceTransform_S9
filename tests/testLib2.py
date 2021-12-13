import sys
sys.path.insert(0, "../lib/cuda/Debug")
import faceswap_func_cuda as fs
import matplotlib.pyplot as plt
import numpy as np
import dlib 
import cv2 


face_to_add1_arr = cv2.imread("../img/macron.jpg")
face_to_add_gray1 = cv2.cvtColor(face_to_add1_arr, cv2.COLOR_BGR2GRAY)

width_FTA = np.int32(len(face_to_add1_arr[1,:,1]))
height_FTA = np.int32(len(face_to_add1_arr[:,1,1]))

FTA = face_to_add_gray1.flatten(order='C')
FTA_sqr = fs.CUDA_Sqr(FTA, width_FTA, height_FTA)

cv2.imwrite("FTA.jpg", FTA_sqr)