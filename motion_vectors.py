import numpy as np
import cv2
import sys
import math

frames_amount = 2

video = cv2.VideoCapture( sys.argv[1] )
frame_list = []

success, frame = video.read()

while not success or len( frame_list ) != frames_amount:
  success, frame = video.read()
  if success:
    frame_list.append( cv2.cvtColor( frame, cv2.COLOR_BGR2GRAY ) )

frame_0 = frame_list[0]
frame_1 = frame_list[1]

def mse2D( block0, block1 ):
  mse = sum( sum( ( block0 - block1 ) ** 2 ) ) / ( block0.shape[0] ** 2 )
  return mse

def findBlockInArea( area, block ):
  area_size_y  = area.shape[0]
  area_size_x  = area.shape[1]
  block_size_y = block.shape[0]
  block_size_x = block.shape[1]
  min_cost = math.inf
  for y in range( 0, area_size_y - block_size_y + 1 ):
    for x in range( 0, area_size_x - block_size_x + 1 ):
      area_block = area[y : y + block_size_y, x : x + block_size_x]
      cost = mse2D( area_block.astype( np.float64 ), block.astype( np.float64 ) )
      if( cost < min_cost ):
        min_cost = cost
        min_y = y
        min_x = x
  print( min_y, min_x )
  return( min_y, min_x )

def findMotionVector( area0, area1, bs ):
  cv2.imshow('area0', area0)
  cv2.imshow('area1', area1)
  area0_size_y = area0.shape[0]
  area0_size_x = area0.shape[1]
  block_corner_y = ( area0_size_y - bs ) // 2
  block_corner_x = ( area0_size_x - bs ) // 2
  block = area0[block_corner_y : block_corner_y + bs, block_corner_x : block_corner_x + bs]
  (y,x) = findBlockInArea( area1, block )
  rel_y = block_corner_y - y
  rel_x = x - block_corner_x
  print( rel_x, rel_y )
  v_abs = math.sqrt( ( rel_y ** 2 + rel_x ** 2 ) )
  tang = rel_y / rel_x
  if( rel_x > 0 ):
    v_dir = math.atan( tang )
  else:
    v_dir = math.atan( tang )
  return( v_abs, v_dir )

(y,x) = findMotionVector( frame_0[240:430,150:340], frame_1[240:430,150:340], 31 )

cv2.waitKey( 0 )
cv2.destroyAllWindows()
print( y,x )
exit()

cv2.imshow( 'frame0', frame_list[0] )
cv2.imshow( 'frame119', frame_list[119] )
cv2.waitKey( 0 )
cv2.destroyAllWindows()
