#include "motion_segmentation.hpp"
#include <iostream>

void estimate_motion_vectors(cv::InputArray _img, cv::InputArray _next_img, int block_size,
                             cv::OutputArray _motion_vector_map, cv::InputArray _mask)
{
  cv::Mat img = _img.getMat();
  cv::Mat next_img = _next_img.getMat();
  cv::Mat mask = _mask.getMat();
  // Images must be the same
  CV_Assert(img.type() == next_img.type() && img.size() == next_img.size());
  _motion_vector_map.create(mask.size(), CV_32FC2);
  cv::Mat motion_vector_map = _motion_vector_map.getMat();
  // Mat with Vec2f as element (two floats, one for angle and one for length)
  motion_vector_map = cv::Mat::zeros(motion_vector_map.size(), CV_32FC2);
  // The block that we will search on next_frame
  cv::Rect searched_block_coords;
  cv::Mat searched_block;
  searched_block_coords.width = block_size;
  searched_block_coords.height = block_size;
  // Current block on next frame, that we are comparing with searched_block
  cv::Rect found_block_coords;
  cv::Mat found_block;
  found_block_coords.width = block_size;
  found_block_coords.height = block_size;
  // Area where we are looking for our bock
  cv::Rect search_area_coords;
  cv::Mat search_area;
  if(block_size % 2) {
    searched_area.width_coords = block_size * 2 + 1;
    searched_area.height_coords = block_size * 2 + 1;
  } else {
    searched_area.width_coords = block_size * 2;
    searched_area.height_coords = block_size * 2;
  }
  int block_in_area_delta = (searched_area_coords.width - block_size) / 2;
  // Loop over all image (mask)
  for(int y = 1; y < (mask.cols - 1); y++)
    for(int x = 1; x < (mask.rows - 1); x++)
    {
      searched_block_coords.x = x * block_size;
      searched_block_coords.y = y * block_size;
      // Loop over search area
      for(int area_y = 0; area_y < searched_area_coords.height; area_y++)
        for(int area_x = 0; area_x < searched_area_coords.width; area_x++)
        {
        }
    }
}
