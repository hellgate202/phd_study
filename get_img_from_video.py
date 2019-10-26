import cv2
import sys

frames_amount = 2

video = cv2.VideoCapture( sys.argv[1] )
frame_list = []

success, frame = video.read()

while not success or len( frame_list ) != frames_amount:
  success, frame = video.read()
  if success:
    frame_list.append( frame )

frame_0 = frame_list[0]
frame_1 = frame_list[1]

cv2.imwrite( "frame_0.png", frame_0 );
cv2.imwrite( "frame_1.png", frame_1 );
