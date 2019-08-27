import numpy as np
import cv2
import sys
import math
import matplotlib.pyplot as plt

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
  center_corner_y = ( area_size_y - block_size_y ) // 2
  center_corner_x = ( area_size_x - block_size_x ) // 2 
  area_block = area[center_corner_y : center_corner_y + block_size_y, center_corner_x : center_corner_x + block_size_x]
  min_cost = mse2D( area_block.astype( np.float64 ), block.astype( np.float64 ) )
  min_y = center_corner_y
  min_x = center_corner_x
  for y in range( 0, area_size_y - block_size_y + 1 ):
    for x in range( 0, area_size_x - block_size_x + 1 ):
      area_block = area[y : y + block_size_y, x : x + block_size_x]
      cost = mse2D( area_block.astype( np.float64 ), block.astype( np.float64 ) )
      if cost + 500 < min_cost:
        min_cost = cost
        min_y = y
        min_x = x
  return( min_y, min_x )

def findMotionVector( area0, area1, bs ):
  area0_size_y = area0.shape[0]
  area0_size_x = area0.shape[1]
  block_corner_y = ( area0_size_y - bs ) // 2
  block_corner_x = ( area0_size_x - bs ) // 2
  block = area0[block_corner_y : block_corner_y + bs, block_corner_x : block_corner_x + bs]
  (y,x) = findBlockInArea( area1, block )
  rel_y = block_corner_y - y
  rel_x = x - block_corner_x
  v_abs = math.sqrt( ( rel_y ** 2 + rel_x ** 2 ) )
  if rel_y > 0 and rel_x >= 0:
    if rel_x == 0:
      v_dir = math.pi / 2
    else:
      v_dir = math.atan( rel_y / rel_x )
  elif rel_y >= 0 and rel_x < 0:
    if rel_y == 0:
      v_dir = math.pi
    else:
      v_dir = math.atan( rel_x / rel_y ) + math.pi / 2
  elif rel_y < 0 and rel_x <= 0:
    if rel_x == 0:
      v_dir = math.pi * 3 / 2
    else:
      v_dir = math.atan( rel_y / rel_x ) + math.pi
  elif rel_x > 0 and rel_y <= 0:
    if rel_y == 0:
      v_dir = 0
    else:
      v_dir = math.atan( rel_x / rel_y ) + 3 * math.pi / 2
  else:
    v_dir = 0
  v_dir = v_dir * 180 / math.pi
  return( v_abs, v_dir )

def motionDetection ( img0, img1, bs ):
  img_size_y = img0.shape[0]
  img_size_x = img0.shape[1]
  line_list = []
  frame_list = []
  bs_amount_y = img_size_y // bs
  bs_amount_x = img_size_x // bs
  scan_y = bs_amount_y * bs
  scan_x = bs_amount_x * bs
  for y in range( bs, scan_y, bs ):
    for x in range( bs, scan_x, bs ):
      area0 = img0[y - bs : y + 2 * bs, x - bs : x + 2 * bs]
      area1 = img1[y - bs : y + 2 * bs, x - bs : x + 2 * bs]
      (v_abs,v_dir) = findMotionVector( area0, area1, bs )
      line_list.append( (v_abs,v_dir) )
    frame_list.append( line_list )
    line_list = []
  return frame_list

bs = 5

img0 = frame_0
img1 = frame_1

frame_list = motionDetection( frame_1, frame_0, bs )
for y, line in enumerate( frame_list ):
  for x, block in enumerate( line ):
    if block[0] > 0:
      rec_corner_0 = (( x + 1 ) * bs,( y + 1 ) * bs)
      rec_corner_1 = (( x + 2 ) * bs,( y + 2 ) * bs)
      cv2.rectangle( img0, rec_corner_0, rec_corner_1, (0,0,0), 1 )
      cv2.rectangle( img1, rec_corner_0, rec_corner_1, (0,0,0), 1 )

plt.imshow(img0, cmap='gray')
plt.show()
