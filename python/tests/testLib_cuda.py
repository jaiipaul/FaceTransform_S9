import sys
sys.path.insert(0, "../../lib/cuda/Release")
import faceswap_func_cuda as fs
import matplotlib.pyplot as plt
import numpy as np
import dlib 
import cv2 


face_to_add1_arr = cv2.imread("../../img/macron.jpg")
face_to_add_gray1 = cv2.cvtColor(face_to_add1_arr, cv2.COLOR_BGR2GRAY)

width_FTA = np.int32(len(face_to_add1_arr[1,:,1]))
height_FTA = np.int32(len(face_to_add1_arr[:,1,1]))

img_Out = np.ndarray((height_FTA, width_FTA), dtype=np.uint8)
FTA = face_to_add_gray1.flatten(order='C')
FTA_sqr = fs.CUDA_Sqr(FTA, width_FTA, height_FTA)

img_Out = np.uint8(np.reshape(FTA_sqr, (height_FTA, width_FTA), order='C'))
cv2.imshow("Result", img_Out)
cv2.waitKey(0)