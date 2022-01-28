# FACE TRANSFORMATION S9
Face Transformation app using python/c++/cuda
(building a wrapped c++/cuda library for python for Windows)

## Setup de l'environnement
1) Installer VS Community 2019 et Python 3.9
2) Cloner le repo et initialiser les sous modules

    ```Powershell
    git clone https://github.com/pfriteau/FaceTransform_S9.git

    git submodule init
    git submodule update
    ```

2) Ouvrir powershell et taper : 

     ```Powershell 
     cmake -version
     ```
     
    si erreur installer [CMake](https://cmake.org/download/) et ajouter le repertoire bin aux variables d'environnement

3) Configurer DLib et OpenCV

    Les scripts Python utilisent DLib et OpenCV. 
    
    ### Compiler les libs dans le projet

    Dlib est en sous-module du projet et peut être compiler pour python:

    ```Powershell
    cd ext/dlib
    python setup.py install
    ``` 
    OpenCV est dejà build pour windows dans le projet

    ## OU
     
    Installer dlib et openCV directement localement avec pip:

    ```Powershell
    pip install dlib
    ```

    ```Powershell
    pip install opencv-python
    ```
    

4) Pour la librairie CUDA, installer le [CUDA Toolkit](https://developer.nvidia.com/cuda-toolkit)

## Compilation librairie python :
1) Créer un dossier `build/`, puis lancer les configs cmake :

    ```Powershell
    mkdir build
    cd build
    ```

    ```Powershell
    cmake ..
    ```

### Solution 1 :
2) Dans `build/`, la solution pour la librairie cpp a été créée. Pour compiler ouvrir la solution `.sln` dans Visual Studio 2019.
3) Générer avec la configuration Release.

### Solution 2 : 
2) Exécuter dans `build/` : 

    ```Powershell
    cmake --build . --config Release --target ALL_BUILD -j 10 --
    ```

## Importation des librairies dans les codes python
La librairie est générée dans `lib/Release`.
Le code python se trouvant dans `python/` devra donc commencer par :

```Python
import sys
sys.path.insert(0, "../lib/Release")
import FaceTransform as FT 
```

Si DLib et OpenCV n'ont pas été installées avec pip:
```Python
import sys
#Local to project DLib
sys.path.insert(0, "../ext/dlib/build/lib")
import dlib
#Local to project OpenCV
sys.path.insert(0, "../ext/opencv")
import cv2
```

