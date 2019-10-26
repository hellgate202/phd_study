clc;
close all;
clearvars;
MOTION_BLOCK_SIZE = 11;
DETAIL_THRESHOLD  = 100;
orig_frame_0 = imread( "./frame_0.png" );
orig_frame_1 = imread( "./frame_1.png" );
frame_0 = rgb2gray( orig_frame_0 );
frame_0 = imresize( frame_0, ceil( size( frame_0 ) ./ MOTION_BLOCK_SIZE ) .* MOTION_BLOCK_SIZE );
orig_frame_0 = imresize( orig_frame_0, size( frame_0 ) );
frame_1 = rgb2gray( orig_frame_1 );
orig_frame_1 = imresize( orig_frame_1, size( frame_1 ) );
frame_1 = imresize( frame_1, ceil( size( frame_1 ) ./ MOTION_BLOCK_SIZE ) .* MOTION_BLOCK_SIZE );
frame_0 = multiscaleMorphGrad( frame_0, [3, 5, 7] );
det_blocks = findDetailedBlocks( frame_0, MOTION_BLOCK_SIZE, DETAIL_THRESHOLD );
frame_0 = placeBoxes( orig_frame_0, det_blocks, [255,0,0] );
imshow( frame_0 );

