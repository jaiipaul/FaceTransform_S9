# FaceSwapS9
FaceSwap app using python/c++/cuda


# Setup
1) Installer VS Community 2019 et Python
2) Ouvrir powershell et taper : cmake, si erreur 
   Installer CMake ( https://cmake.org/download/ ) et ajouter le repertoire bin aux variables d'environnement

3) Installer dlib et openCV :
    > pip install dlib
    > pip install openCV

Compilation librairie cpp/python ou cuda/python
4) aller dans le dossier cpp et creer un dossier build, puis lancer les config cmake :
    > cd cpp (ou cuda) && mkdir build && cd build
    > cmake ..

Solution 1 :
5) Dans build tous la solution pour la librairie cpp à été créée, pour compiler ouvrir la solution .sln dans visual studio 2019
6) Generer

Solution 2 :
5) executer dans build: 
    cmake --build . --config Release --target ALL_BUILD -j 10 --