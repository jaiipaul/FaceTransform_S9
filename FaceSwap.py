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

## FUNCTIONS -----------------------------------------------------------------------------##
def img_array_to_list(array):
    size = array.shape
    list = []
    for y in range(size[0]):
        for x in range(size[1]):
            for c in range(size[2]):
                list.append(array[y,x,c])
    return list

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
    return landmarks_point
##----------------------------------------------------------------------------------------------##
def extract_index(nparray):
    index = None 
    for num in nparray[0]:
        index = num
        break
    return index 
##----------------------------------------------------------------------------------------------##
## FORM TRIANGLE FROM A LANDMARKS DISTRIBUTION
def find_triangles(landmarks_point):
    points = np.array(landmarks_point, dtype=np.int32)
    hull=cv2.convexHull(points)
    cv2.fillConvexPoly(mask,hull,0)
      
    #face_image_1 = cv2.bitwise_and(face_to_add, face_to_add, mask=mask)
    rect = cv2.boundingRect(hull)
    (x,y,w,h) = rect
    #Delaunay triangulation
              
    subdiv = cv2.Subdiv2D(rect)
    subdiv.insert(landmarks_point)
    
    triangles = subdiv.getTriangleList()
    triangles = np.array(triangles, dtype=np.int32)
    indexes_triangles = []
    print(len(triangles))
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
##----------------------------------------------------------------------------------------------##
## Extract Triangle region
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
    
    cv2.fillConvexPoly(cropped_mask,points, 255)
    cropped_triangle = cv2.bitwise_and(cropped_triangle, cropped_triangle, mask=cropped_mask)
    return points, cropped_triangle, x, y, w, h


##-------------------------------------------------------------------------------------------------##
##FRACE SWAPPING ALGORITHM
cap = cv2.VideoCapture(0, cv2.CAP_ANY) # ouvrir la caméra 
_, temp = cap.read()
width_CAM = len(temp[1,:,1])
height_CAM = len(temp[:,1,1])

## FIRST FACE TREATEMENT // THE ALGORITHM WILL PUT THIS FACE ON THE ONE CAPTURE BY THE CAMERA
## Find the face
face_to_add = cv2.imread("./img/singe.jpg")

face_to_add_gray, faces = get_faces(face_to_add)
mask = np.zeros_like(face_to_add_gray)
## Get landmarks/Face features and form triangle from these, delaunay triangulation
for face in faces:
        landmarks_point = get_landmarks(face_to_add_gray, face)
        indexes_triangles = find_triangles(landmarks_point)     

## WEBCAM FACE TREATEMENT // PASTE THE FIRST FACE ON THE FACES CAPTURED BY THE CAMERA
while cap.isOpened():    # tant que la caméa est ouverte  
    ##Find faces
    _, camera_img = cap.read()
    
    camera_img_gray, camera_faces = get_faces(camera_img)
    camera_img_new_face = np.zeros_like(camera_img) 

    if (0 < len(camera_faces)):
        for face in camera_faces:
            landmarks_point2 = get_landmarks(camera_img_gray, face)
        #triangulisation of the second face, from the first face
            points2 = np.array(landmarks_point2, dtype=np.int32)
            hull_camera = cv2.convexHull(points2)

            for triangle_index in indexes_triangles:
                # Extract triangle on the first face
                points_triangle1, cropped_triangle1, x1, y1, w1, h1 = crop_triangle(face_to_add, landmarks_point, triangle_index)
                
                #extract triangle formed by the same points on face captured by camera
                points_triangle2, cropped_triangle2, x2, y2, w2, h2 = crop_triangle(camera_img, landmarks_point2, triangle_index)
                
                #warp triangles 
                points_triangle1 = np.float32(points_triangle1)
                points_triangle2 = np.float32(points_triangle2)
                M = cv2.getAffineTransform(points_triangle1, points_triangle2)
                warped_triangle = cv2.warpAffine(cropped_triangle1, M, (w2,h2))
                
                #reconstruct destination face  
                triangle_area = camera_img_new_face[y2:y2+h2, x2:x2+w2]
                triangle_area = cv2.add(triangle_area, warped_triangle)
                camera_img_new_face[y2:y2+h2, x2:x2+w2] = triangle_area 
         
           
    
    #face swapped (putting 1st face in the second )
    #camera_img_new_face_gray = cv2.cvtColor(camera_img_new_face, cv2.COLOR_BGR2GRAY)
    #_,background =  cv2.threshold(camera_img_new_face_gray, 1,255,cv2.THRESH_BINARY_INV)
    #background = cv2.bitwise_and(camera_img, camera_img, mask = background )
    #result = cv2.add(background, camera_img_new_face)
    #result2 = cv2.flip(result, 1)

            Dst_face_mask = np.zeros_like(camera_img_gray)
            Dst_head_mask = cv2.fillConvexPoly(Dst_face_mask, hull_camera, 255)
            Dst_face_mask = cv2.bitwise_not(Dst_head_mask)
    
    #maskage
            img2Noface = cv2.bitwise_and(camera_img, camera_img, mask=Dst_face_mask)
            imgOut = cv2.add(img2Noface, camera_img_new_face)
    
            (x, y, w, h) = cv2.boundingRect(hull_camera)
            center_face2 = (int((x + x + w) / 2), int((y + y + h) / 2))
    
    # Pour supprimer les lignes 
    # On copie le contenu de imgOut dans imgDst 
        seamlessclone = cv2.seamlessClone(imgOut, camera_img, Dst_head_mask, center_face2, cv2.NORMAL_CLONE)
        result = cv2.flip(seamlessclone, 1)
        cv2.imshow("FaceSwap", result)
    else:
        result = cv2.flip(camera_img, 1)
        cv2.imshow("FaceSwap", result)
    if cv2.waitKey(1) == ord('q'):
        cv2.destroyAllWindows()
        break      

cap.release()
cv2.destroyAllWindows()