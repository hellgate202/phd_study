#include "find_motion_blocks.hpp"

void find_motion_blocks(InputArray _img, InputArray _next_img, int block_size, 
                        OutputArray _motion_map)
{
  Mat img = _img.getMat();
  Mat next_img = _next_img.getMat();
  // Images must be the same
  CV_Assert(img.type() == next_img.type() && img.size() == next_img.size());
  _motion_map.create(img.size(), img.type());
  Mat motion_map = _motion_map.getMat();
  img.copyTo(motion_map);
  int img_width = img.cols;
  int img_height = img.rows;
  int px_amount = img_width * img_height;
  Mat interframe_difference;
  subtract(next_img, img, interframe_difference);
  // Making interframe difference signed
  interframe_difference.convertTo(interframe_difference, CV_16S);
  // Average motion on frames
  double mean_motion = sum(interframe_difference)[0] / px_amount;
  Mat mean_delta;
  // The same as interframe difference, but we subtract average motion
  // from every pixel
  subtract(interframe_difference, mean_motion, mean_delta);
  double rsmd_motion;
  // Square of mean_delta
  pow(mean_delta, 2, mean_delta);
  rsmd_motion = sqrt(sum(mean_delta)[0] / px_amount);
  double mean_in_block;
  Rect roi;
  Mat roi_block;
  // Black block for visualization
  Mat empty_block = Mat::zeros(block_size, block_size, CV_8U);
  for(int y = 0; y <= (img_height - block_size - 1); y += block_size)
    for(int x = 0; x <= (img_width - block_size - 1); x += block_size)
    {
      roi.x = x;
      roi.y = y;
      roi.width = block_size;
      roi.height = block_size;
      roi_block = interframe_difference(roi);
      mean_in_block = sum(roi_block)[0] / pow(block_size, 2);
      // If motion is higher than RSMD than we treat it as movement block
      if(mean_in_block < (rsmd_motion + mean_motion) || mean_in_block < 10)
        empty_block.copyTo(motion_map(roi));
    }
}

