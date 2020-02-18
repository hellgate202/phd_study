/*
 * Function takes two images (subsequent frames)
 * and find all moving block between them
 */

#include "motion_segmentation.hpp"
#include "math.h"
#include <opencv2/core.hpp>

void find_motion_blocks(cv::Mat img, cv::Mat next_img, int block_size, 
                        cv::Mat &motion_map)
{
  motion_map = cv::Mat::zeros(img.rows / block_size, img.cols / block_size, CV_8U);
  cv::Mat interframe_difference;
  img.convertTo(img, CV_16S);
  next_img.convertTo(next_img, CV_16S);
  // Finding absolute interframe difference
  cv::subtract(next_img, img, interframe_difference);
  interframe_difference = cv::abs(interframe_difference);
  // Average motion on frames
  double mean_motion = cv::mean(interframe_difference)[0];
  cv::Mat mean_delta;
  // The same as interframe difference, but we subtract average motion
  // from every pixel
  cv::subtract(interframe_difference, mean_motion, mean_delta);
  // Square of mean_delta
  cv::pow(mean_delta, 2, mean_delta);
  // Movement deviation
  double rsmd_motion = std::sqrt(cv::mean(mean_delta)[0]);
  double mean_in_block;
  cv::Rect roi;
  roi.width = block_size;
  roi.height = block_size;
  cv::Mat roi_block;
  for(int y = 0; y <= (img.rows - block_size); y += block_size)
    for(int x = 0; x <= (img.cols - block_size); x += block_size)
    {
      roi.x = x;
      roi.y = y;
      roi_block = interframe_difference(roi);
      mean_in_block = cv::mean(roi_block)[0];
      // If motion is higher than RSMD and mean than we treat it as movement block
      // 10 is used to filter blocks on static images
      if(mean_in_block > (rsmd_motion + mean_motion) && mean_in_block > 10)
        motion_map.at<uchar>(y / block_size, x / block_size) = 1;
    }
}
