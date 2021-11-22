import sys
sys.path.insert(0, "./cpp/build/Debug")
import faceswap_func as fs
import numpy as np
fs.hello()
fs.addition(3, 6)
tab = np.ndarray(10, dtype=np.int)
fs.fillTab(tab, 10)
for i in range(10):
    print(tab[i])
