#include <stdio.h>
#include <math.h>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

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
  // Black block for visualization
  Mat empty_block = Mat::zeros(block_size, block_size, CV_8U);
  // Mat with (value - mean)
  Mat mean_delta;
  double mean_of_block;
  double mean_motion;
  double rsmd_motion;
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
  frame_width = video_file.get(CAP_PROP_FRAME_WIDTH);
  frame_height = video_file.get(CAP_PROP_FRAME_HEIGHT);
  if(frame_height % block_size)
    frame_height = (frame_height / block_size + 1) * block_size;
  if(frame_width % block_size)
    frame_width = (frame_width / block_size + 1 ) * block_size;
  Size frame_size(frame_width, frame_height);
  double px_amount = frame_height * frame_width;
  // Getting first frame
  video_file.read(frame);
  cvtColor(frame, frame, COLOR_BGR2GRAY);
  resize(frame, frame, frame_size);
  if(frame.empty())
    return -1;
  // Getting second frame;
  video_file.read(next_frame);
  cvtColor(next_frame, next_frame, COLOR_BGR2GRAY);
  resize(next_frame, next_frame, frame_size);
  if(next_frame.empty())
    return -1;
  for(;;)
  {
    // Subtracting frame from next_frame
    // It will be casted to CV_8U
    subtract(next_frame, frame, interframe_difference);
    // RSMD calculation
    mean_motion = sum(interframe_difference)[0] / px_amount;
    interframe_difference.convertTo(interframe_difference, CV_16S);
    subtract(interframe_difference, mean_motion, mean_delta);
    pow(mean_delta, 2, mean_delta);
    rsmd_motion = sqrt(sum(mean_delta)[0] / px_amount);
    // Scan blocks
    for( int y = 0; y < frame_height; y += block_size )
      for( int x = 0; x < frame_width; x += block_size )
      {
        roi.x = x;
        roi.y = y;
        roi.width = block_size;
        roi.height = block_size;
        block = interframe_difference(roi);
        mean_of_block = sum( block )[0] / pow(block_size, 2);
        // If motion is higher than RSMD than we treat it as movement block
        if(mean_of_block < rsmd_motion || mean_of_block < 10)
          empty_block.copyTo(frame(roi));
      }
    imshow("Video", frame);
    if (waitKey(5) >= 0)
      break;
    next_frame.copyTo(frame);
    video_file.read(next_frame);
    resize(next_frame, next_frame, frame_size);
    cvtColor(next_frame, next_frame, COLOR_BGR2GRAY);
    if(next_frame.empty())
      return 0;
  }
  return 0;
}
