/*
 * Function calculates rosenfeld-troy measure
 * over image's blocks using multiscale morphological
 * gradient
 */

#include "motion_segmentation.hpp"
#include <opencv2/core.hpp>

void rosenfeld_troy_measure(cv::Mat img, int block_size, cv::Mat &rf_map)
{
  // If it is colored - make it grayscale
  if(img.channels() != 1)
    cv::cvtColor(img, img, cv::COLOR_BGR2GRAY);
  rf_map = cv::Mat::zeros(img.rows / block_size, img.cols / block_size, CV_8U);
  cv::Mat hf_map;
  // Usefull to extract high frequency information
  multiscale_morph_grad(img, hf_map, {1,3,5,7});
  cv::Rect roi;
  cv::Mat roi_block;
  roi.width = block_size;
  roi.height = block_size;
  int rf_in_block;
  // Scan blocks of image and calculating mean value in it
  for(int y = 0; y <= (img.rows - block_size); y += block_size)
    for( int x = 0; x <= (img.cols - block_size); x += block_size)
    {
      roi.x = x;
      roi.y = y;
      hf_map(roi).copyTo(roi_block);
      rf_map.at<uchar>(y / block_size, x / block_size) = cv::mean(roi_block)[0];
    }
}
