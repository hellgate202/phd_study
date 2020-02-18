/*
 * Function draws motion vectors on image
 */

#include "motion_segmentation.hpp"
#include <math.h>

void visualize_motion_vectors(cv::Mat background, int block_size, 
                              cv::Mat motion_vector_map, cv::Mat &visualized_img,
                              cv::Vec3b color)
{
  // Make image colorfull
  if(background.channels() == 1)
    cv::cvtColor(background, background, cv::COLOR_GRAY2BGR);
  background.copyTo(visualized_img);
  cv::Point vector_center;
  cv::Point vector_end;
  for(int y = 0; y < motion_vector_map.rows; y++)
    for(int x = 0; x < motion_vector_map.cols; x++)
      if(motion_vector_map.at<cv::Vec3f>(y,x)[0])
      {
        // Start point of motion vector
        vector_center.y = y * block_size + block_size / 2;
        vector_center.x = x * block_size + block_size / 2;
        // Place a dot in the center
        circle(visualized_img, vector_center, 1, color, -1);
        // Reverse atan2 endpoint ov motion vector
        vector_end.y = motion_vector_map.at<cv::Vec3f>(y,x)[1] * 
                       std::sin(motion_vector_map.at<cv::Vec3f>(y,x)[2]) + vector_center.y;
        vector_end.x = motion_vector_map.at<cv::Vec3f>(y,x)[1] * 
                       std::cos(motion_vector_map.at<cv::Vec3f>(y,x)[2]) + vector_center.x;
        // Draw line
        cv::line(visualized_img, vector_center, vector_end, color);
      }
}
