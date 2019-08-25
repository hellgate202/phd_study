import numpy as np
import cv2
import sys

img = cv2.imread( sys.argv[1], 1 )
n_img = np.zeros( (img.shape[0], img.shape[1]) )

for y in range( img.shape[0] ):
  for x in range( img.shape[1] ):
    y_val = img.item(y,x,0)*0.114+img.item(y,x,1)*0.587+img.item(y,x,2)*0.299
    n_img.itemset((y,x),y_val)
n_img = n_img.astype( np.uint8 )

cv2.imshow( 'image', n_img )
cv2.waitKey( 0 )
cv2.destroyWindow( 'image' )

