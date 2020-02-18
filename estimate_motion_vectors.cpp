/*
 * Function calculates motion vectors
 * on blocks that a matched as moving in mask
 * It will return a Mat with Vec3f where 
 * [0] - presence of vector, [1] - length, [2] - angle
 */

#include "motion_segmentation.hpp"
#include <opencv2/core.hpp>
#include <math.h>

void estimate_motion_vectors(cv::Mat img, cv::Mat next_img, int block_size,
                             cv::Mat mask, cv::Mat &motion_vector_map)
{
  // Making them large and signed, so we can compute MSE
  img.convertTo(img, CV_16S);
  next_img.convertTo(next_img, CV_16S);
  // Mat with Vec3f as element (three floats, one for precense, one for angle and one for length)
  motion_vector_map = cv::Mat::zeros(img.rows / block_size, img.cols / block_size, CV_32FC3);
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
  // Mat for calculation MSE
  cv::Mat mse_calc_mat;
  // Area where we are looking for our bock
  cv::Rect search_area_coords;
  cv::Mat search_area;
  // We need equal border around searched block, so search
  // area size depends on oddness
  if(block_size % 2) {
    search_area_coords.width = block_size * 2 + 1;
    search_area_coords.height = block_size * 2 + 1;
  } else {
    search_area_coords.width = block_size * 2;
    search_area_coords.height = block_size * 2;
  }
  // Interval between block in the center of area and border of area
  int block_in_area_delta = (search_area_coords.width - block_size) / 2;
  double mse;
  double vector_length;
  double vector_angle;
  // Point where motion vector will point
  // Matrix with all mse between searched block and all found blocks
  cv::Mat mse_in_area(search_area_coords.height - found_block_coords.height + 1,
                      search_area_coords.width - found_block_coords.width + 1, CV_64F);
  // Relative position of motion vector point
  cv::Point min_mse_pos;
  // Loop over all image (mask)
  for(int y = 1; y < (mask.rows - 1); y++)
    for(int x = 1; x < (mask.cols - 1); x++)
      if( mask.at<uchar>(y,x) )
      {
        // Preparing block in current frame
        searched_block_coords.x = x * block_size;
        searched_block_coords.y = y * block_size;
        searched_block = img(searched_block_coords);
        // Preparing area in next frame
        search_area_coords.x = searched_block_coords.x - block_in_area_delta;
        search_area_coords.y = searched_block_coords.y - block_in_area_delta;
        search_area = next_img(search_area_coords);
        // Loop over search area
        for(int area_y = 0; area_y < (search_area_coords.height - found_block_coords.height + 1); area_y++)
          for(int area_x = 0; area_x < (search_area_coords.width - found_block_coords.width + 1); area_x++)
          {
            found_block_coords.y = area_y;
            found_block_coords.x = area_x;
            found_block = search_area(found_block_coords);
            // MSE calculation
            cv::subtract(searched_block, found_block, mse_calc_mat);
            cv::pow(mse_calc_mat, 2, mse_calc_mat);
            mse_in_area.at<double>(area_y,area_x) = cv::mean(mse_calc_mat)[0];
          }
        // Looking for the position of the minumum mse
        cv::minMaxLoc(mse_in_area, NULL, NULL, &min_mse_pos, NULL);
        // Shifting position to center instead of corner
        min_mse_pos.y -= (search_area_coords.height - found_block_coords.height) / 2;
        min_mse_pos.x -= (search_area_coords.width - found_block_coords.width) / 2;
        vector_length = std::sqrt(std::pow(min_mse_pos.y, 2) + std::pow(min_mse_pos.x, 2));
        vector_angle = std::atan2(min_mse_pos.y, min_mse_pos.x);
        motion_vector_map.at<cv::Vec3f>(y,x) = cv::Vec3f(1, vector_length, vector_angle);
      } 
}
