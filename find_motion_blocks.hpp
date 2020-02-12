#ifndef FIND_MOTION_BLOCKS
#define FIND_MOTION_BLOCKS

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <stdio.h>

void find_motion_blocks(cv::InputArray _img, cv::InputArray _next_img, int block_size, 
                        cv::OutputArray _motion_map);

void place_boxes(cv::InputArray _src, cv::InputArray _boxes_map, int block_size, 
                 cv::OutputArray _dst, cv::Vec3b color = cv::Vec3b(0, 0, 255));

#endif
