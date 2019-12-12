clc;
close all;
clearvars;
VIDEO_FILE               = "./video_example.mp4";
FIRST_FRAME              = 1;
FRAMES_AMOUNT            = 360;
MOTION_BLOCK_SIZE        = 15;
MOTION_THRESHOLD         = 11;
MSE_THRESHOLD            = 35;
DETAIL_THRESHOLD         = 30;
VECTOR_GROUP_SEARCH_AREA = 3;
FRAME_GAP                = 1;
VECTOR_TOLLERANCE        = pi;
v = VideoReader( VIDEO_FILE );
frames = read( v, [FIRST_FRAME FIRST_FRAME + FRAMES_AMOUNT + FRAME_GAP] );
output_size = ceil( size( frames(:,:,1,1) ) ./ MOTION_BLOCK_SIZE ) .* MOTION_BLOCK_SIZE;
output_video = zeros( output_size(1), output_size(2), 3, FRAMES_AMOUNT, 'uint8' );
for f = 1 : 1 : FRAMES_AMOUNT 
  orig_frame_0 = frames(:,:,:,f);
  orig_frame_1 = frames(:,:,:,f + FRAME_GAP);
  frame_0 = rgb2gray( orig_frame_0 );
  frame_0 = imresize( frame_0, ceil( size( frame_0 ) ./ MOTION_BLOCK_SIZE ) .* MOTION_BLOCK_SIZE );
  orig_frame_0 = imresize( orig_frame_0, size( frame_0 ) );
  frame_1 = rgb2gray( orig_frame_1 );
  frame_1 = imresize( frame_1, ceil( size( frame_1 ) ./ MOTION_BLOCK_SIZE ) .* MOTION_BLOCK_SIZE );
  orig_frame_1 = imresize( orig_frame_1, size( frame_1 ) );
  det_img = multiscaleMorphGrad( frame_0, [3, 5, 7] );
  det_blocks = findDetailedBlocks( det_img, MOTION_BLOCK_SIZE, DETAIL_THRESHOLD );
  [vec_length, vec_orient, vec_map] = findMotionVectors( frame_0, frame_1, det_blocks, MOTION_THRESHOLD, MSE_THRESHOLD );
  [vec_groups, groups_amount] = findVectorGroups( vec_orient, VECTOR_TOLLERANCE, VECTOR_GROUP_SEARCH_AREA );
  all_class_img = markAllClasses( orig_frame_0, groups_amount, vec_groups, 'elipse', MOTION_BLOCK_SIZE );
  all_class_img = placeBoxes( all_class_img, vec_map, [0,255,0] );
  output_video(:,:,:,f) = all_class_img;
  fprintf( "%0d / %0d frames done\n", f, FRAMES_AMOUNT );
end
