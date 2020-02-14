#include "motion_segmentation.hpp"
#include <math.h>

void visualize_motion_vectors(cv::InputArray _background, int block_size, 
                              cv::InputArray _motion_vector_map, cv::OutputArray _visualized_img,
                              cv::Vec3b color)
{
  cv::Mat motion_vector_map = _motion_vector_map.getMat();
  cv::Mat background = _background.getMat();
  if(background.channels() == 1)
    cv::cvtColor(background, background, cv::COLOR_GRAY2BGR);
  _visualized_img.create(background.size(), background.type());
  cv::Mat visualized_img = _visualized_img.getMat();
  background.copyTo(visualized_img);
  cv::Point vector_center;
  cv::Point vector_end;
  for(int y = 0; y < motion_vector_map.rows; y++)
    for(int x = 0; x < motion_vector_map.cols; x++)
      if(motion_vector_map.at<cv::Vec3f>(y,x)[0])
      {
        vector_center.y = y * block_size + block_size / 2;
        vector_center.x = x * block_size + block_size / 2;
        circle(visualized_img, vector_center, 1, color, -1);
        vector_end.y = motion_vector_map.at<cv::Vec3f>(y,x)[1] * 
                       std::sin(motion_vector_map.at<cv::Vec3f>(y,x)[2]) + vector_center.y;
        vector_end.x = motion_vector_map.at<cv::Vec3f>(y,x)[1] * 
                       std::cos(motion_vector_map.at<cv::Vec3f>(y,x)[2]) + vector_center.x;
        cv::line(visualized_img, vector_center, vector_end, color);
      }
  visualized_img.copyTo(_visualized_img);
}
