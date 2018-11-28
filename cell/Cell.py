
# coding: utf-8

# In[1]:


import numpy as np
import cv2
import scipy.spatial


# In[2]:


i = 12
if (i < 10):
    s = 'Normalized/000' + str(i)
else:
    s = 'Normalized/00' + str(i)
im = cv2.imread(s + '.jpg')
imgray = cv2.cvtColor(im,cv2.COLOR_BGR2GRAY)
ret,thresh = cv2.threshold(imgray,55,255,0)
#im2, contours, hierarchy = cv2.findContours(thresh,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)
connectivity = 6
# Perform the operation
output = cv2.connectedComponentsWithStats(thresh, connectivity, cv2.CV_32S)
# Get the results
# The first cell is the number of labels
num_labels = output[0]
# The second cell is the label matrix
labels = output[1]
# The third cell is the stat matrix
stats = output[2]
# The fourth cell is the centroid matrix
centroids = output[3]

counter = 0
lst = []
newcentroid = []
points = []
for i in range(len(stats)):
    if (stats[i][4] > 1200):
        counter += 1
        lst.append(i)
#print(counter)
#print(lst)
#print(centroids)
for i in lst[1:]:
    cv2.rectangle(im,(stats[i][0],stats[i][1]),(stats[i][0] + stats[i][2],stats[i][1] + stats[i][3]),(0,255,0),3)
    #print(centroids[i])
# for i in range(1,len(lst)):
#     newcentroid.append([])
#     for j in lst[1:]:
#         if (centroids[lst[i]][0] != centroids[j][0] and centroids[lst[i]][1] != centroids[j][1]):
#             newcentroid[i-1].append(scipy.spatial.distance.euclidean(centroids[lst[i]], centroids[j]))

# for i in range(1,len(lst)):
#     for j in range(1,len(lst)):
#         if (i != j and min(newcentroid[i-1]) == min(newcentroid[j - 1])):
#             #print(i,j)
#             if [i,j] and [j,i] not in points:
#                 points.append([i,j])
#                 break
# c = 0
# for i in range(1, len(lst)):
#     for j in range(1,len(lst)):
#         if (i != j and min(newcentroid[i-1]) != min(newcentroid[j - 1])):
#             c += 1
#     if c == (len(lst) - 2):
#         points.append([i])
#     c = 0
# newcentroid = []
# for i in points:
#     length = len(i)
#     x = 0
#     y = 0
#     for j in range(length):
#         x += centroids[lst[i[j]]][0]
#         y += centroids[lst[i[j]]][1]
#     newcentroid.append([x/length,y/length])
file = open(s + '.txt','w') 
#for i in newcentroid:
for i in range(1, len(lst)):
    cv2.circle(im,(int(centroids[lst[i]][0]),int(centroids[lst[i]][1])), 2, (0,0,255), -1)
    file.write(str(centroids[lst[i]][0]))
    file.write(",")
    file.write(str(centroids[lst[i]][1]))
    file.write('\n')
file.close()


# In[3]:


# im2, contours, hierarchy = cv2.findContours(thresh,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)
# count = 0
# newCon = []
# for i in range(len(contours)):
#     if (len(contours[i]) > 160):
#         count += 1
#         newCon.append(contours[i])
# print(count)
# print(type(contours))
# cv2.drawContours(im, newCon, -1, (0,255,0), 3)


# In[ ]:


cv2.imshow('image', thresh)
cv2.imshow('test', im)
cv2.waitKey(0);
cv2.destroyAllWindows()

