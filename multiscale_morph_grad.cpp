#include "motion_segmentation.hpp"

void multiscale_morph_grad (cv::InputArray _src, cv::OutputArray _dst, std::vector<int> sizes)
{
  cv::Mat src = _src.getMat();
  src.convertTo(src, CV_16S);
  _dst.create(src.size(), src.type());
  cv::Mat dst = _dst.getMat();
  cv::Mat kernel;
  cv::Mat previous_kernel;
  cv::Mat dilatated_img;
  cv::Mat eroded_img;
  cv::Mat morph_grad;
  cv::Mat accum = cv::Mat::zeros(src.size(), CV_16S);
  for(int i = 1; i < sizes.size(); i++)
  {
    kernel = cv::Mat::ones(cv::Size(sizes[i], sizes[i]), CV_8U);
    previous_kernel = cv::Mat::ones(cv::Size(sizes[i - 1], sizes[i - 1]), CV_8U);
    cv::dilate(src, dilatated_img, kernel);
    cv::erode(src, eroded_img, kernel);
    cv::subtract(dilatated_img, eroded_img, morph_grad);
    cv::erode(morph_grad, morph_grad, previous_kernel);
    cv::add(morph_grad, accum, accum);
  }
  cv::divide(accum, cv::Scalar(sizes.size() - 1), accum);
  accum.convertTo(accum, CV_8U);
  accum.copyTo(_dst);
}
