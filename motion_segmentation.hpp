#ifndef FIND_MOTION_BLOCKS
#define FIND_MOTION_BLOCKS

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <math.h>

void find_motion_blocks(cv::Mat img, cv::Mat next_img, int block_size, cv::Mat &motion_map);

void place_boxes(cv::Mat src, cv::Mat boxes_map, int block_size, 
                 cv::Mat &dst, cv::Vec3b color = cv::Vec3b(0, 0, 255));

void estimate_motion_vectors(cv::Mat img, cv::Mat next_img, int block_size,
                             cv::Mat mask, cv::Mat &motion_vector_map);

void visualize_motion_vectors(cv::Mat background, int block_size, 
                              cv::Mat motion_vector_map, cv::Mat &visualized_img,
                              cv::Vec3b color = cv::Vec3b(0, 0, 255));

void multiscale_morph_grad (cv::Mat src, cv::Mat &dst, std::vector<int> sizes = {1,3,5,7});

void rosenfeld_troy_measure(cv::Mat img, int block_size, cv::Mat &rf_map);

void simple_hist(cv::Mat src, cv::Mat &dst);

void find_detailed_blocks(cv::Mat img, int block_size, cv::Mat &det_map);

#endif
