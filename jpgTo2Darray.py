from PIL import Image
import numpy as np
import os

im = Image.open("ayaka.jpg")
col,row =  im.size
data = np.zeros((row*col, 5))
R = np.zeros((row,col)).astype(int)
G = np.zeros((row,col))
B = np.zeros((row,col))
pixels = im.load()

os.remove('R.txt')
os.remove('G.txt')
os.remove('B.txt')
f1 = open("R.txt","a")
f2 = open("G.txt","a")
f3 = open("B.txt","a")

for i in range(row):
    for j in range(col):
        r,g,b =  pixels[i,j]
        data[i*col + j,:] = r,g,b,i,j
        #f1.write("{0:b} ".format(r))
        #f2.write("{0:b} ".format(g))
        #f3.write("{0:b} ".format(b))
        f1.write("%d "%r)
        f2.write("%d "%g)
        f3.write("%d "%b)
        R[i][j] = r
        G[i][j] = g
        B[i][j] = b
    f1.write("\n")
    f2.write("\n")
    f3.write("\n")
f1.close()
f2.close()
f3.close()
'''
for row in :
    for ele in row:
        print("{0:b}".format(ele))
        f1.write("{0:b}".format(ele))
        #f.write("")
    f.write("\n")
f.close()'''

