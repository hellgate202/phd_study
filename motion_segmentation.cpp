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
  // Frame that will be processed
  cv::Mat frame;
  // Frame that will be saved in color for visualizing
  cv::Mat frame_color;
  cv::Mat next_frame;
  cv::Mat next_frame_color;
  // Map with marked detailed blocks
  cv::Mat det_map;
  // Map with marked motion blocks
  cv::Mat motion_blocks;
  // Image with visualized motion blocks
  cv::Mat boxes_on_motion;
  // Motion vectors map
  cv::Mat motion_vectors;
  // Image with visualized motion vectors
  cv::Mat visualized_motion_vectors;
  // ROI for block processing
  int fps;
  int frame_period;
  int block_size = std::stoi(argv[2]);
  cv::VideoCapture video_file;
  video_file.open(argv[1]);
  fps = video_file.get(cv::CAP_PROP_FPS);
  // In miliseconds
  frame_period = 1000 / fps;
  video_file.read(frame);
  // Save for visualization
  frame.copyTo(frame_color);
  // Working only with grayscale
  cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);
  // If failed to read first frame
  if(frame.empty())
    return -1;
  video_file.read(next_frame);
  next_frame.copyTo(next_frame_color);
  cv::cvtColor(next_frame, next_frame, cv::COLOR_BGR2GRAY);
  // If failed to read second frame
  if(next_frame.empty())
    return -1;
  while( 1 )
  {
    // We will look for motion vectors only in detailed blocks
    find_detailed_blocks(frame, block_size, det_map);
    // Looking for all moving blocks (not motion vectos)
    find_motion_blocks(frame, next_frame, block_size, motion_blocks);
    // Placing boxes on moving blocks for visualization purposes
    place_boxes(frame_color, motion_blocks, block_size, frame_color, cv::Vec3b(255,0,0));
    // Continue working only with blocks that are both containing movement and are detailed
    cv::bitwise_and(det_map, motion_blocks, motion_blocks);
    // Placing boxes on blocks with motion vectors being estimated for visualization purposes
    place_boxes(frame_color, motion_blocks, block_size, boxes_on_motion, cv::Vec3b(0,255,0));
    // Calculating motion vectors
    estimate_motion_vectors(frame, next_frame, block_size, motion_blocks, motion_vectors);
    // Visualizing motion vectors
    visualize_motion_vectors(boxes_on_motion, block_size, motion_vectors, visualized_motion_vectors);
    // Showing processed frame
    cv::imshow("Video", visualized_motion_vectors);
    // Pause between imshows, / 2 need to fps be adequate, don't know why
    if( cv::waitKey(frame_period / 2) >= 0 )
      break;
    // Switching frames from next to current
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
