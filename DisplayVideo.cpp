#include <stdio.h>
#include <math.h>
#include <opencv2/opencv.hpp>
#include "find_motion_blocks.hpp"

using namespace cv;
using namespace std;

void find_motion_blocks(InputArray _img, InputArray _next_img, int block_size, 
                        OutputArray _motion_map);

int main(int argc, char** argv) {
  if (argc != 3)
  {
    printf("Usage: DisplayVideo <Video_Path> <Block_size>\n");
    return -1;
  }
  Mat frame;
  Mat next_frame;
  Mat interframe_difference;
  Mat block;
  // ROI for block processing
  Rect roi;
  int fps, frame_width, frame_height;
  int frame_period;
  int block_size = stoi( argv[2] );
  VideoCapture video_file;
  // Opening a video file
  video_file.open(argv[1]);
  if(!video_file.isOpened()) {
    cerr << "ERROR! Unable to open the file\n";
    return -1;
  }
  // Getting video information
  fps = video_file.get(CAP_PROP_FPS);
  frame_period = 1000 / fps;
  // Getting first frame
  video_file.read(frame);
  cvtColor(frame, frame, COLOR_BGR2GRAY);
  if(frame.empty())
    return -1;
  // Getting second frame;
  video_file.read(next_frame);
  cvtColor(next_frame, next_frame, COLOR_BGR2GRAY);
  if(next_frame.empty())
    return -1;
  Mat motion_blocks;
  for(;;)
  {
    find_motion_blocks(frame, next_frame, block_size, motion_blocks);
    imshow("Video", motion_blocks);
    if (waitKey(5) >= 0)
      break;
    next_frame.copyTo(frame);
    video_file.read(next_frame);
    cvtColor(next_frame, next_frame, COLOR_BGR2GRAY);
    if(next_frame.empty())
      return 0;
  }
  return 0;
}
