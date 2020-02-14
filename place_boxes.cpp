#include "motion_segmentation.hpp"

void place_boxes(cv::InputArray _src, cv::InputArray _boxes_map, int block_size,
                 cv::OutputArray _dst, cv::Vec3b color)
{
  cv::Mat src = _src.getMat();
  cv::Mat boxes_map = _boxes_map.getMat();
  if(src.channels() == 1)
    cv::cvtColor(src, src, cv::COLOR_GRAY2BGR);
  _dst.create(src.size(), src.type());
  cv::Mat dst = _dst.getMat();
  src.copyTo(dst);
  for(int y = 0; y < boxes_map.rows; y++)
    for( int x = 0; x < boxes_map.cols; x++)
      if( boxes_map.at<uchar>(y,x) > 0 )
        cv::rectangle(dst, cv::Point(x * block_size, y * block_size),
                      cv::Point((x + 1) * block_size, (y + 1) * block_size), color);
}

