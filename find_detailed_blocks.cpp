#include "motion_segmentation.hpp"

void find_detailed_blocks(cv::Mat img, int block_size, cv::Mat &det_map)
{
  cv::Mat rf_map;
  rosenfeld_troy_estimation(img, block_size, rf_map);
  cv::Mat rf_hist;
  cv::Point hist_max;
  //int rf_mode;
  double rf_mean = cv::mean(rf_map)[0];
  //simple_hist(rf_map, rf_hist);
  //cv::minMaxLoc(rf_hist, NULL, NULL, NULL, &hist_max);
  //rf_mode = hist_max.x;
  det_map = cv::Mat::zeros(rf_map.size(), CV_8U);
  for(int y = 0; y < det_map.rows; y++)
    for(int x = 0; x < det_map.cols; x++)
      if(rf_map.at<uchar>(y,x) > rf_mean)
        det_map.at<uchar>(y,x) = 1;
      else
        det_map.at<uchar>(y,x) = 0;
}
