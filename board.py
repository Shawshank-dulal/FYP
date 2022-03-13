import cv2
import numpy as np


imPath = r"./test.jpg"  # <----- image path


def imageResize(orgImage, resizeFact):
    dim = (int(orgImage.shape[1]*resizeFact),       
           int(orgImage.shape[0]*resizeFact))
    return cv2.resize(orgImage, dim, cv2.INTER_AREA)


img = imageResize(cv2.imread(imPath), 0.5)
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)


kernel = cv2.getStructuringElement(cv2.MORPH_RECT, (2, 2))                                                                             
morphed = cv2.dilate(gray, kernel, iterations=1)

_,thresh = cv2.threshold(gray,  150, 255, cv2.THRESH_BINARY) 

# find canny edge
edged_wide = cv2.Canny(thresh, 10, 10, apertureSize=3)
edged_wide = cv2.copyMakeBorder(edged_wide, 1,1,1,1, cv2.BORDER_CONSTANT,value=[255,255,255])
# edged_wide = cv2.Canny(edged_wide, 10, 100, apertureSize=3)

lines = cv2.HoughLines(edged_wide, 1, np.pi / 180, 200)

# find Contours
contours, hierarchy = cv2.findContours(
    cv2.inRange(morphed, 12,0), cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)  


cntImg = img.copy()
print(type(lines))
minArea, maxArea = 2000, 4000

valid_cnts = []
for c in contours:
    area = cv2.contourArea(c)
    if area > minArea and area < maxArea:
        valid_cnts.append(c)

        # draw centers 
        M = cv2.moments(c)
        cX = int(M["m10"] / M["m00"])
        cY = int(M["m01"] / M["m00"])
        cv2.circle(cntImg, (cX, cY), 5, (0, 0, 255), -1)


cv2.drawContours(cntImg, valid_cnts, -1, (0, 255, 0), 2)

for line in lines:
    rho,theta=line[0]
    a=np.cos(theta)
    b=np.sin(theta)
    x0=a*rho
    y0=b*rho
    x1=int(x0+1000*(-b))
    y1=int(y0+100*(a))
    x2=int(x0-1000*(-b))
    y2=int(y0-1000*(a))
    cv2.line(img,(x1,y1),(x2,y2),(0,0,255),2)

cv2.imshow('threshold', thresh)
cv2.imshow('morphed', morphed)
cv2.imshow('canny edge', edged_wide)
cv2.imshow('contour', cntImg)
cv2.imshow('image', img)
cv2.waitKey(0)

cv2.destroyAllWindows()