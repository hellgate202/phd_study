/*
 * Function place color boxes on image accorging to map
 */

#include "motion_segmentation.hpp"
#include <opencv2/core.hpp>

void place_boxes(cv::Mat src, cv::Mat boxes_map, int block_size,
                 cv::Mat &dst, cv::Vec3b color)
{
  // If it is grayscale we transform it, so boxes will be nice and colorful
  if(src.channels() == 1)
    cv::cvtColor(src, src, cv::COLOR_GRAY2BGR);
  src.copyTo(dst);
  for(int y = 0; y < boxes_map.rows; y++)
    for( int x = 0; x < boxes_map.cols; x++)
      if( boxes_map.at<uchar>(y,x) > 0 )
        cv::rectangle(dst, cv::Point(x * block_size, y * block_size),
                      cv::Point((x + 1) * block_size, (y + 1) * block_size), color);
}

