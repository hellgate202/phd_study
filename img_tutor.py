import numpy as np
import cv2
import sys

img = cv2.imread( sys.argv[1], 1 )
y_img = cv2.cvtColor( img, cv2.COLOR_BGR2GRAY )

def multiscaleMorphGrad( img, k_list ):
  m_img_list = []
  for i in range( len( k_list ) ):
    kernel = cv2.getStructuringElement( cv2.MORPH_RECT, (k_list[i],k_list[i]) )
    m_img = cv2.morphologyEx( img, cv2.MORPH_GRADIENT, kernel )
    m_img_list.append( m_img.astype( np.uint16 ) )
  for i in range( len( k_list ) ):
    s_img = sum( m_img_list ) / len( m_img_list )
  return( s_img.astype( np.uint8 ) )

    
m_img = multiscaleMorphGrad( y_img, [3,5,7,9,11] )

cv2.imshow( 'm_img', m_img.astype( np.uint8 ) )

cv2.waitKey( 0 )
cv2.destroyAllWindows( )

