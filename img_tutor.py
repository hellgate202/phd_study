import numpy as np
import cv2
import sys

threshold = 90
blocksize = 17

img = cv2.imread( sys.argv[1], 1 )
y_img = cv2.cvtColor( img, cv2.COLOR_BGR2GRAY )
y_size = y_img.shape[0]
x_size = y_img.shape[1]

def multiscaleMorphGrad( img, k_list ):
  m_img_list = []
  for i in range( len( k_list ) ):
    kernel = cv2.getStructuringElement( cv2.MORPH_RECT, (k_list[i],k_list[i]) )
    m_img = cv2.morphologyEx( img, cv2.MORPH_GRADIENT, kernel )
    m_img_list.append( m_img.astype( np.uint16 ) )
  for i in range( len( k_list ) ):
    s_img = sum( m_img_list ) / len( m_img_list )
  return s_img 

    
m_img = multiscaleMorphGrad( y_img, [3,5,7] )

def findHighFreqBlocks( img, bs, threshold ):
  px_in_block = bs ** 2
  bs_delta = bs // 2
  y_size = img.shape[0]
  x_size = img.shape[1]
  y_blocks_amount = y_size // bs
  x_blocks_amount = x_size // bs
  y_scan = y_blocks_amount * bs
  x_scan = x_blocks_amount * bs
  o_img = img
  for y in range( bs_delta, y_scan, bs ):
    for x in range( bs_delta, x_scan, bs ):
      block = m_img[y - bs_delta : y + bs_delta + 1, x - bs_delta : x + bs_delta + 1]
      texture_value = sum( sum( block ) ) / px_in_block
      if( texture_value > threshold ):
        cv2.rectangle( o_img, (x + bs_delta,y - bs_delta), (x - bs_delta,y + bs_delta), (255,255,255), 1)
  return o_img

m_img = findHighFreqBlocks( m_img, blocksize, threshold )

cv2.imshow( 'm_img', m_img.astype( np.uint8 ) )
cv2.waitKey( 0 )
cv2.destroyAllWindows( )
