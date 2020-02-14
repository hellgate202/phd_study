#include <stdio.h>
#include <iostream>
#include <math.h>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "motion_segmentation.hpp"

int main(int argc, char** argv) {
  if (argc != 3)
  {
    std::cout << "Usage: DisplayVideo <Video_Path> <Block_size>\n" << std::endl;
    return -1;
  }
  cv::Mat frame;
  cv::Mat frame_color;
  cv::Mat next_frame;
  cv::Mat next_frame_color;
  cv::Mat interframe_difference;
  cv::Mat block;
  // ROI for block processing
  int fps;
  int frame_period;
  int block_size = std::stoi( argv[2] );
  cv::VideoCapture video_file;
  // Opening a video file
  video_file.open(argv[1]);
  if(!video_file.isOpened()) {
    std::cerr << "ERROR! Unable to open the file\n";
    return -1;
  }
  // Getting video information
  fps = video_file.get(cv::CAP_PROP_FPS);
  frame_period = 1000 / fps;
  // Getting first frame
  video_file.read(frame);
  frame.copyTo(frame_color);
  cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);
  if(frame.empty())
    return -1;
  // Getting second frame;
  video_file.read(next_frame);
  next_frame.copyTo(next_frame_color);
  cv::cvtColor(next_frame, next_frame, cv::COLOR_BGR2GRAY);
  if(next_frame.empty())
    return -1;
  cv::Mat motion_blocks;
  cv::Mat boxes_on_motion;
  cv::Mat motion_vectors;
  cv::Mat visualized_motion_vectors;
  for(;;)
  {
    find_motion_blocks(frame, next_frame, block_size, motion_blocks);
    place_boxes(frame_color, motion_blocks, block_size, boxes_on_motion, cv::Vec3b(0,255,0));
    estimate_motion_vectors(frame, next_frame, block_size, motion_vectors, 
                            motion_blocks);
    visualize_motion_vectors(boxes_on_motion, block_size, motion_vectors, visualized_motion_vectors);
    cv::imshow("Video", visualized_motion_vectors);
    if (cv::waitKey(frame_period / 2) >= 0)
      break;
    next_frame.copyTo(frame);
    next_frame_color.copyTo(frame_color);
    video_file.read(next_frame);
    next_frame.copyTo(next_frame_color);
    cv::cvtColor(next_frame, next_frame, cv::COLOR_BGR2GRAY);
    if(next_frame.empty())
      return 0;
  }
  return 0;
}
