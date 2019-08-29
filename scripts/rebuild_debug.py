import os
import sys

if sys.platform=="linux2" or sys.platform=="linux":
    os.system("make clean")
    os.system("cmake .")
    os.system("cmake --build .")
else:
    print("Your System is: " + sys.platform)