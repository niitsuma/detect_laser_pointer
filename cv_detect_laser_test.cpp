#include "cv_detect_laser.hpp"

#define INPUT_VIDEO "laser.avi"
#define WRITE_VIDEO 0
#define OUTPUT_VIDEO "detected.avi"


    int main( )
    {
      laser_detector<double> detector(list_hsv_laser_default);
  
      std::string video_url_str   = INPUT_VIDEO ; 
      std::cout << video_url_str <<  std::endl;
      cv::VideoCapture capture(video_url_str.c_str());
#if WRITE_VIDEO
      VideoWriter writer;
      double outputFps = capture.get(CV_CAP_PROP_FPS);
      CvSize capture_size =
	cvSize((int)capture.get(CV_CAP_PROP_FRAME_WIDTH),(int)capture.get(CV_CAP_PROP_FRAME_HEIGHT));
      int codec = 
	CV_FOURCC('D','I','V','3'); 
      std::cout << "FPS: " << outputFps << std::endl;
      std::cout << "CODEC: " << codec << std::endl;
      writer.open(
		  //"detected.avi"
		  OUTPUT_VIDEO
		  , codec
		  , outputFps
		  , capture_size);
#endif
      namedWindow( "window1", 1 );  
      while(1)
	{
	  Mat frame,frame1;
	  capture >> frame1;
	  if(frame1.empty()) break;
	  detector.detect(frame1);
	  if(detector.detected){cv::circle(frame1,detector.detected_point,detector.detected_size*5,CV_RGB(255,0,0),3,8,0);}

	  imshow( "window1", frame1 );
#if WRITE_VIDEO
	  writer << frame1 ;
#endif
	  if(waitKey(1) == 'c') break; 
	}
      return 0;
    }


