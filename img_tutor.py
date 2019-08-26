import numpy as np
import cv2
import sys

texture_bs = 5
texture_thr = 100

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
  return( s_img.astype( np.uint8 ) ) 

    
m_img = multiscaleMorphGrad( y_img, [3,5,7,9,11] )

bs_delta = texture_bs // 2
y_blocks_amount = y_size // texture_bs
x_blocks_amount = x_size // texture_bs
for y in range( bs_delta, y_blocks_amount, texture_bs ):
  for x in range( bs_delta, x_blocks_amount, texture_bs ):
    block = m_img[x - bs_delta : x + bs_delta + 1, y - bs_delta : y + bs_delta + 1]
    texture_value = sum( sum( block ) )
    print( block.dtype )


'''
cv2.imshow( 'm_img', m_img.astype( np.uint8 ) )

cv2.waitKey( 0 )
cv2.destroyAllWindows( )
'''
