#ifndef FIND_MOTION_BLOCKS
#define FIND_MOTION_BLOCKS

#include <opencv2/opencv.hpp>

using namespace cv;

void find_motion_blocks(InputArray _img, InputArray _next_img, int block_size, 
                        OutputArray _motion_map);

#endif
