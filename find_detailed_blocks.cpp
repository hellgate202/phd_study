/*
 * Function gets image and block size
 * and writes back a new Mat with 1s
 * for blocks where details are present and
 * 0 otherwise
 */

#include "motion_segmentation.hpp"
#include <opencv2/core.hpp>

void find_detailed_blocks(cv::Mat img, int block_size, cv::Mat &det_map)
{
  cv::Mat rf_map;
  rosenfeld_troy_measure(img, block_size, rf_map);
  // Commented mode calculation, because mean works better
  // May be, i'm doing smth wrong
  double rf_mean = cv::mean(rf_map)[0];
  det_map = cv::Mat::zeros(rf_map.size(), CV_8U);
  // For now, if current block has higher details than mean - it
  // is treated as detailed block
  for(int y = 0; y < det_map.rows; y++)
    for(int x = 0; x < det_map.cols; x++)
      if(rf_map.at<uchar>(y,x) > rf_mean)
        det_map.at<uchar>(y,x) = 1;
      else
        det_map.at<uchar>(y,x) = 0;
}
