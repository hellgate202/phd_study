#include "motion_segmentation.hpp"

void find_motion_blocks(cv::InputArray _img, cv::InputArray _next_img, int block_size, 
                        cv::OutputArray _motion_map)
{
  cv::Mat img = _img.getMat();
  cv::Mat next_img = _next_img.getMat();
  // Images must be the same
  CV_Assert(img.type() == next_img.type() && img.size() == next_img.size());
  //_motion_map.create(img.size(), img.type());
  _motion_map.create(img.rows / block_size, img.cols / block_size, img.type());
  cv::Mat motion_map = _motion_map.getMat();
  motion_map = cv::Mat::zeros(img.rows / block_size, img.cols / block_size, CV_8U);
  int img_width = img.cols;
  int img_height = img.rows;
  int px_amount = img_width * img_height;
  cv::Mat interframe_difference;
  img.convertTo(img, CV_16S);
  next_img.convertTo(next_img, CV_16S);
  cv::subtract(next_img, img, interframe_difference);
  interframe_difference = cv::abs(interframe_difference);
  // Average motion on frames
  double mean_motion = cv::sum(interframe_difference)[0] / px_amount;
  cv::Mat mean_delta;
  // The same as interframe difference, but we subtract average motion
  // from every pixel
  cv::subtract(interframe_difference, mean_motion, mean_delta);
  double rsmd_motion;
  // Square of mean_delta
  cv::pow(mean_delta, 2, mean_delta);
  rsmd_motion = std::sqrt(cv::sum(mean_delta)[0] / px_amount);
  double mean_in_block;
  cv::Rect roi;
  cv::Mat roi_block;
  for(int y = 0; y <= (img_height - block_size); y += block_size)
    for(int x = 0; x <= (img_width - block_size); x += block_size)
    {
      roi.x = x;
      roi.y = y;
      roi.width = block_size;
      roi.height = block_size;
      roi_block = interframe_difference(roi);
      mean_in_block = cv::sum(roi_block)[0] / std::pow(block_size, 2);
      // If motion is higher than RSMD and mean than we treat it as movement block
      if(mean_in_block > (rsmd_motion + mean_motion) && mean_in_block > 10)
        motion_map.at<uchar>(y / block_size, x / block_size) = 1;
    }
}
