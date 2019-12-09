clc;
close all;
clearvars;
VIDEO_FILE = "./video_example.mp4";
FRAMES_AMOUNT = 30;
MOTION_BLOCK_SIZE = 15;
MOTION_THRESHOLD  = 5;
DETAIL_THRESHOLD  = 80;
VECTOR_GROUP_SEARCH_AREA = 5;
VECTOR_TOLLERANCE = pi / 2;
v = VideoReader( VIDEO_FILE );
frames = read( v, [1 FRAMES_AMOUNT + 1] );
output_video = [];
for f = 1 : 1 : FRAMES_AMOUNT 
  orig_frame_0 = frames(:,:,:,f);
  orig_frame_1 = frames(:,:,:,f + 1);
  frame_0 = rgb2gray( orig_frame_0 );
  frame_0 = imresize( frame_0, ceil( size( frame_0 ) ./ MOTION_BLOCK_SIZE ) .* MOTION_BLOCK_SIZE );
  orig_frame_0 = imresize( orig_frame_0, size( frame_0 ) );
  frame_1 = rgb2gray( orig_frame_1 );
  frame_1 = imresize( frame_1, ceil( size( frame_1 ) ./ MOTION_BLOCK_SIZE ) .* MOTION_BLOCK_SIZE );
  orig_frame_1 = imresize( orig_frame_1, size( frame_1 ) );
  det_img = multiscaleMorphGrad( frame_0, [3, 5, 7] );
  det_blocks = findDetailedBlocks( det_img, MOTION_BLOCK_SIZE, DETAIL_THRESHOLD );
  [vec_length, vec_orient, vec_map] = findMotionVectors( frame_0, frame_1, det_blocks, MOTION_THRESHOLD );
  [vec_groups, groups_amount] = findVectorGroups( vec_orient, VECTOR_TOLLERANCE, VECTOR_GROUP_SEARCH_AREA );
  all_class_img = markAllClasses( orig_frame_0, groups_amount, vec_groups, 'elipse', MOTION_BLOCK_SIZE );
  all_class_img = placeBoxes( all_class_img, vec_map, [0,255,0] );
  output_video = cat( 4, output_video, all_class_img );
  fprintf( "%0d / %0d frames done\n", f, FRAMES_AMOUNT );
end
