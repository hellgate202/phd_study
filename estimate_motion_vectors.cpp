#include "motion_segmentation.hpp"
#include <math.h>

void estimate_motion_vectors(cv::InputArray _img, cv::InputArray _next_img, int block_size,
                             cv::OutputArray _motion_vector_map, cv::InputArray _mask)
{
  cv::Mat img = _img.getMat();
  img.convertTo(img, CV_16S);
  cv::Mat next_img = _next_img.getMat();
  next_img.convertTo(next_img, CV_16S);
  cv::Mat mask = _mask.getMat();
  // Images must be the same
  CV_Assert(img.type() == next_img.type() && img.size() == next_img.size());
  _motion_vector_map.create(mask.size(), CV_32FC3);
  cv::Mat motion_vector_map = _motion_vector_map.getMat();
  // Mat with Vec3f as element (three floats, one for precense, one for angle and one for length)
  motion_vector_map = cv::Mat::zeros(motion_vector_map.size(), CV_32FC3);
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
    search_area_coords.width = block_size * 2 + 1;
    search_area_coords.height = block_size * 2 + 1;
  } else {
    search_area_coords.width = block_size * 2;
    search_area_coords.height = block_size * 2;
  }
  int block_in_area_delta = (search_area_coords.width - block_size) / 2;
  double mse;
  double vector_length;
  double vector_angle;
  int motion_vec_y;
  int motion_vec_x;
  cv::Mat mse_in_area(search_area_coords.height - found_block_coords.height + 1,
                      search_area_coords.width - found_block_coords.width + 1, CV_64F);
  cv::Point min_mse_pos;
  // Loop over all image (mask)
  for(int y = 1; y < (mask.rows - 1); y++)
    for(int x = 1; x < (mask.cols - 1); x++)
      if( mask.at<uchar>(y,x) )
      {
        // Preparing block in current frame
        searched_block_coords.x = x * block_size;
        searched_block_coords.y = y * block_size;
        img(searched_block_coords).copyTo(searched_block);
        // Prepating area in next frame
        search_area_coords.x = searched_block_coords.x - block_in_area_delta;
        search_area_coords.y = searched_block_coords.y - block_in_area_delta;
        next_img(search_area_coords).copyTo(search_area);
        // Loop over search area
        for(int area_y = 0; area_y < (search_area_coords.height - found_block_coords.height + 1); area_y++)
          for(int area_x = 0; area_x < (search_area_coords.width - found_block_coords.width + 1); area_x++)
          {
            found_block_coords.y = area_y;
            found_block_coords.x = area_x;
            search_area(found_block_coords).copyTo(found_block);
            cv::subtract(searched_block, found_block, found_block);
            cv::pow(found_block, 2, found_block);
            mse = cv::sum(found_block)[0] / std::pow(block_size, 2);
            mse_in_area.at<double>(area_y,area_x) = mse;
          }
        cv::minMaxLoc(mse_in_area, NULL, NULL, &min_mse_pos, NULL);
        motion_vec_y = min_mse_pos.y - (search_area_coords.height - found_block_coords.height) / 2;
        motion_vec_x = min_mse_pos.x - (search_area_coords.width - found_block_coords.width) / 2;
        vector_length = std::sqrt(std::pow(motion_vec_y, 2) + std::pow(motion_vec_x, 2));
        vector_angle = std::atan2(motion_vec_y, motion_vec_x);
        motion_vector_map.at<cv::Vec3f>(y,x) = cv::Vec3f(1, vector_length, vector_angle);
      } else {
        motion_vector_map.at<cv::Vec3f>(y,x) = cv::Vec3f(0,0,0);
      }
}
