/*
 * Function calculates multiscale morphological gradient
 * over entire image. It is usefull to extract high
 * frequency information
 */


#include "motion_segmentation.hpp"
#include <opencv2/core.hpp>

void multiscale_morph_grad (cv::Mat src, cv::Mat &dst, std::vector<int> sizes)
{
  src.convertTo(src, CV_16S);
  dst = cv::Mat::zeros(src.size(), CV_16S);
  // Two kernels, because we will erode with previous size kernel
  cv::Mat kernel;
  cv::Mat previous_kernel;
  cv::Mat dilatated_img;
  cv::Mat eroded_img;
  // Dilatation - erosion
  cv::Mat morph_grad;
  for(int i = 1; i < sizes.size(); i++)
  {
    // Only square kernels are supported
    kernel = cv::Mat::ones(cv::Size(sizes[i], sizes[i]), CV_8U);
    previous_kernel = cv::Mat::ones(cv::Size(sizes[i - 1], sizes[i - 1]), CV_8U);
    cv::dilate(src, dilatated_img, kernel);
    cv::erode(src, eroded_img, kernel);
    cv::subtract(dilatated_img, eroded_img, morph_grad);
    cv::erode(morph_grad, morph_grad, previous_kernel);
    cv::add(morph_grad, dst, dst);
  }
  // Find mean of accumulated values
  cv::divide(dst, cv::Scalar(sizes.size() - 1), dst);
  dst.convertTo(dst, CV_8U);
}
