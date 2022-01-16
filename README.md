# FaceSwapS9
FaceSwap app using python/c++/cuda


## Setup de l'environnement
1) Installer VS Community 2019 et Python 3.9
2) Ouvrir powershell et taper : 

     ```Powershell 
     cmake -version
     ```
     
    si erreur installer [CMake](https://cmake.org/download/) et ajouter le repertoire bin aux variables d'environnement

3) Installer dlib et openCV :

    ```Powershell
    pip install dlib
    ```

    ```Powershell
    pip install opencv-python
    ```

4) Pour la librairie CUDA, installer le [CUDA Toolkit](https://developer.nvidia.com/cuda-toolkit)

## Compilation librairie python :
1) Se placer dans le dossier `cpp_cuda/` et créer un dossier `build/`, puis lancer les config cmake :

    ```Powershell
    cd cpp_cuda && mkdir build && cd build
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
import faceswap_func as fs 
```

