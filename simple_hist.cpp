#include "motion_segmentation.hpp"

void simple_hist(cv::Mat src, cv::Mat &dst)
{
  dst = cv::Mat::zeros(1, 256, CV_16U);
  for(int y = 0; y < src.rows; y++)
    for(int x = 0; x < src.cols; x++)
      dst.at<ushort>(src.at<uchar>(y,x)) += 1;
}
