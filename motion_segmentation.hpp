#ifndef FIND_MOTION_BLOCKS
#define FIND_MOTION_BLOCKS

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <stdio.h>
#include <iostream>
#include <vector>

void find_motion_blocks(cv::InputArray _img, cv::InputArray _next_img, int block_size, 
                        cv::OutputArray _motion_map);

void place_boxes(cv::InputArray _src, cv::InputArray _boxes_map, int block_size, 
                 cv::OutputArray _dst, cv::Vec3b color = cv::Vec3b(0, 0, 255));

void estimate_motion_vectors(cv::InputArray _img, cv::InputArray _next_img, int block_size,
                             cv::OutputArray _motion_vector_map, cv::InputArray _mask);

void visualize_motion_vectors(cv::InputArray _background, int block_size, 
                              cv::InputArray _motion_vector_map, cv::OutputArray _visualized_img,
                              cv::Vec3b color = cv::Vec3b(0, 0, 255));

void multiscale_morph_grad (cv::InputArray _src, cv::OutputArray _dst,
                            std::vector<int> sizes = {1,3,5,7});

#endif
