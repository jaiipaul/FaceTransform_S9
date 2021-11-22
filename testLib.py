import sys
sys.path.insert(0, "./cpp/build/Debug")
import faceswap_func as fs
import numpy as np
fs.hello()
fs.addition(3, 6)

tab = [10,9,8,1,3,1,4,8,9,0]
fs.fillTab(tab, 10)
for i in range(10):
    print(tab[i])
