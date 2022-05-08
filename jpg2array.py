# https://stackoverflow.com/questions/27026866/convert-an-image-to-2d-array-in-python
from PIL import Image
import numpy as np
import os

im = Image.open("ayaka.jpg")
col,row =  im.size
pixels = im.load()

f = open("RGB.txt","w")
f.write("%d %d"%(row,col))

for i in range(row):
    for j in range(col):
        r,g,b =  pixels[i,j]
        f.write("%d %d %d "%(r,g,b))

f.close()


