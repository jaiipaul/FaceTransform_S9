# -*- coding: utf-8 -*-
"""
Created on Sun Oct 24 21:54:45 2021

@author: pcmaroc
"""
import dlib 
import cv2
import numpy as np 

def extract_index(nparray):
    index = None 
    for num in nparray[0]:
        index = num
        break
    return index 
        

def get_landmarks(img_gray, face):
    landmarks=predictor(img_gray, face)
    landmarks_point = []
    for n in range(0, 68):
        x = landmarks.part(n).x
        y = landmarks.part(n).y
        landmarks_point.append((x, y))
    return landmarks_point

def get_faces(img):
    img_gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    faces = detector(img_gray)
    #print("Found ")
    #print(len(faces))
    return img_gray, faces


def find_triangles(landmarks_point):
    points = np.array(landmarks_point, dtype=np.int32)
    hull=cv2.convexHull(points)
    cv2.fillConvexPoly(mask,hull,255)
      
    #face_image_1 = cv2.bitwise_and(face_to_add, face_to_add, mask=mask)
    rect = cv2.boundingRect(hull)
    (x,y,w,h) = rect
    #Delaunay triangulation
              
    subdiv = cv2.Subdiv2D(rect)
    subdiv.insert(landmarks_point)
    
    triangles = subdiv.getTriangleList()
    triangles = np.array(triangles, dtype=np.int32)
    indexes_triangles = []
    #print(len(triangles))
    for t in triangles :
        pt1 = (t[0], t[1])
        pt2 = (t[2], t[3])
        pt3 = (t[4], t[5])
        
        index_pt1 = np.where((points == pt1).all(axis=1))
        index_pt1 = extract_index(index_pt1)
        
        index_pt2 = np.where((points == pt2).all(axis=1))
        index_pt2 = extract_index(index_pt2)
        
        index_pt3 = np.where((points == pt3).all(axis=1))
        index_pt3 = extract_index(index_pt3)
        
        
        if index_pt1 is not None and index_pt2 is not None and index_pt3 is not None:
            triangle = [index_pt1, index_pt2, index_pt3]
            indexes_triangles.append(triangle)
        
    return indexes_triangles


def crop_triangle(img, landmarks_point, triangle_index):
    tr_pt1 = landmarks_point[triangle_index[0]]
    tr_pt2 = landmarks_point[triangle_index[1]]
    tr_pt3 = landmarks_point[triangle_index[2]]
    triangle = np.array([tr_pt1, tr_pt2, tr_pt3], np.int32)
    
    rect = cv2.boundingRect(triangle)
    (x,y,w,h) = rect
    
    cropped_triangle = img[y:y+h, x:x+w]
    cropped_mask = np.zeros((h,w), np.uint8)
    points = np.array( [[tr_pt1[0]-x, tr_pt1[1]-y],
                        [tr_pt2[0]-x, tr_pt2[1]-y],
                        [tr_pt3[0]-x, tr_pt3[1]-y]])
     
    return points, cropped_triangle, cropped_mask, x, y, w, h







imgSrc = cv2.imread("./img/willsmith.jpg")
# Le pentagone dont la forme ressemple à ça 
"""
                    ______________
               /                   \
              \                    \
               \                   /
                \                 / 
                 \               / 
                  \             /
                   \ _________ /

"""



#initilisation de l'objet responsable de la détection du visage  
detector = dlib.get_frontal_face_detector()
#modèle pré-entrainé 
predictor = dlib.shape_predictor("shape_predictor_68_face_landmarks.dat")
#dans le cas d'images statique(!= real time) len(faces)=1 mais en 
# temps réel y en a plusieurs 
imgSrc_gray, faces = get_faces(imgSrc)
mask = np.zeros_like(imgSrc_gray)
for face in faces:
    # on prélève du visage (qui est dans img_gray) les 68 points caractéristiques 
    landmarks_pointSrc = get_landmarks(imgSrc_gray, face)        
    indexes_triangles = find_triangles(landmarks_pointSrc) 

cap = cv2.VideoCapture(0) # ouvrir la caméra 

while cap.isOpened():
    _, imgDst = cap.read()
    new_face = np.zeros_like(imgDst) 
            
                
    imgDst_gray, facesDst = get_faces(imgDst)
    
    if len(facesDst) > 0 :
        for face in facesDst:
            landmarks_pointDst = get_landmarks(imgDst_gray, face)    
            pointsDst = np.array(landmarks_pointDst, np.int32)
            hullDst = cv2.convexHull(pointsDst)


        lines_space_mask = np.zeros_like(imgSrc_gray)
        lines_space_new_face = np.zeros_like(imgDst)         
        #triangulisation of the second face, from the first face 

        for triangle_index in indexes_triangles:
        
            pointsSrc, cropped_triangleSrc,cropped_maskSrc, x,y,w,h = crop_triangle(imgSrc, landmarks_pointSrc, triangle_index)
            lines_space = cv2.bitwise_and(cropped_triangleSrc,cropped_triangleSrc, mask=cropped_maskSrc)

            pointsDst, cropped_triangleDst,cropped_maskDst, x,y,w,h = crop_triangle(imgDst, landmarks_pointDst, triangle_index)
            cv2.fillConvexPoly(cropped_maskDst, pointsDst, 255)


            #warp triangles 
            pointsSrc = np.float32(pointsSrc)
            pointsDst = np.float32(pointsDst)
            #effectuer la transformation affine 
            M = cv2.getAffineTransform(pointsSrc, pointsDst)
            # transformation du triangle source selon warpaffine 
            warped_triangle = cv2.warpAffine(cropped_triangleSrc, M, (w,h))
            #faire passer les triangles du visage1 sur le 2 en utilisantt un mask 
            warped_triangle = cv2.bitwise_and(warped_triangle, warped_triangle, mask=cropped_maskDst)
            # Reconstructing destination face
            new_faceRect = new_face[y: y + h, x: x + w]
            new_faceRect_gray = cv2.cvtColor(new_faceRect, cv2.COLOR_BGR2GRAY)
            _, mask_triangles_designed = cv2.threshold(new_faceRect_gray, 1, 255, cv2.THRESH_BINARY_INV)
            warped_triangle = cv2.bitwise_and(warped_triangle, warped_triangle, mask=mask_triangles_designed)

            new_faceRect = cv2.add(new_faceRect, warped_triangle)
            new_face[y: y + h, x: x + w] = new_faceRect



        Dst_face_mask = np.zeros_like(imgDst_gray)
        Dst_head_mask = cv2.fillConvexPoly(Dst_face_mask, hullDst, 255)
        Dst_face_mask = cv2.bitwise_not(Dst_head_mask)

        #maskage
        img2Noface = cv2.bitwise_and(imgDst, imgDst, mask=Dst_face_mask)
        imgOut = cv2.add(img2Noface, new_face)

        (x, y, w, h) = cv2.boundingRect(hullDst)
        center_face2 = (int((x + x + w) / 2), int((y + y + h) / 2))

        # Pour supprimer les lignes 
        # On copie le contenu de imgOut dans imgDst 
        seamlessclone = cv2.seamlessClone(imgOut, imgDst, Dst_head_mask, center_face2, cv2.NORMAL_CLONE)
        result = cv2.flip(seamlessclone, 1)
        cv2.imshow("FaceSwap", result)
    else:
        result = cv2.flip(imgDst, 1)
        cv2.imshow("FaceSwap", result)
    if cv2.waitKey(1) == ord('q'):
        cv2.destroyAllWindows()
        break    

cap.release()
cv2.destroyAllWindows()












