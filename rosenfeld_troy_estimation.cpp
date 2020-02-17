#include "motion_segmentation.hpp"

void rosenfeld_troy_estimation(cv::InputArray _img, int block_size, cv::OutputArray _rf_map)
{
  cv::Mat img = _img.getMat();
  if(img.channels() != 1)
    cv::cvtColor(img, img, cv::COLOR_BGR2GRAY);
  _rf_map.create(img.rows / block_size, img.cols / block_size, CV_8U);
  cv::Mat rf_map = _rf_map.getMat();
  cv::Mat hf_map;
  multiscale_morph_grad(img, hf_map, {1,3,5,7});
  cv::Rect roi;
  cv::Mat roi_block;
  roi.width = block_size;
  roi.height = block_size;
  int rf_in_block;
  for(int y = 0; y <= (img.rows - block_size); y += block_size)
    for( int x = 0; x <= (img.cols - block_size); x += block_size)
    {
      roi.x = x;
      roi.y = y;
      img(roi).copyTo(roi_block);
      rf_in_block = cv::sum(roi_block)[0] / std::pow(block_size, 2);
      rf_map.at<uchar>(y / block_size, x / block_size) = rf_in_block;
    }
}
