#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/legacy/legacy.hpp> 
#include <iostream>
#include <stdio.h>
#include <vector> 
#include <algorithm>
#include <functional>

#include "cv_hotelling_t2.hpp"



#define MAX_N_FEATURE_POINT 50
#define TH_T2_SCORE1 0.99

//manual sample laser HSV color 40 poind  
int n_hsv_laser=40;
cv::Mat_<double> list_hsv_laser_default
=
  (cv::Mat_<double>(n_hsv_laser,3)
  <<
    55.6, 72.84, 208.56
     ,54.68, 71.04000000000001, 206.56
     ,57.12, 83.60000000000001, 210.28
     ,55.06122448979591, 90.14285714285714, 185.2857142857143
     ,56.04, 74.52, 208.48
     ,55.28571428571428, 77.67346938775509, 180.0204081632653
     ,56.76, 73.60000000000001, 207.2
     ,56.8, 74.23999999999999, 203.56
     ,58.36, 73.76000000000001, 208.16
     ,55.72, 72.44, 200.6
     ,57.4, 74.76000000000001, 204.92
     ,56.88, 75.16, 204.88
     ,58.76, 75.16, 210.84
     ,54.28, 83.2, 208.88
     ,50.22448979591837, 83.3469387755102, 182.6734693877551
     ,51, 75.16, 204.8
     ,54.16, 75.28, 207.04
     ,54.48, 75.72, 211.6
     ,54.6, 65.16, 205.72
     ,52.61224489795918, 77.93877551020408, 182.2857142857143
     ,51.76, 70.60000000000001, 206.04
     ,60.8, 65.68000000000001, 209.88
     ,57.24, 54, 209.84
     ,56.6, 52.04, 209.76
     ,61.12, 55.28, 211.08
     ,56.76, 48, 212.36
     ,56.65306122448979, 60.06122448979591, 193.0204081632653
     ,56.44, 63.92, 217.64
     ,54.30612244897959, 67.48979591836734, 193.8979591836735
     ,55.92, 57.96, 210.12
     ,50.99999999999999, 79.24489795918366, 184.8775510204082
     ,54.30612244897959, 67.48979591836734, 193.8979591836735
     ,55.92, 57.96, 210.12
     ,53.44, 62.92, 205.24
     ,53.52, 62.36, 209.08
     ,54.08, 73.44, 207.24
     ,51.04, 70, 205.52
     ,52.08, 72.64, 211.16
     ,49.68, 70.2, 211.48
     ,54.8, 65.56, 207.36

     );


// feature point response comparison, for list sorting
bool compare_response(cv::KeyPoint first, cv::KeyPoint second)
{
if (first.response/first.size > second.response/second.size) return true;
 else return false;
}


using namespace cv;

template<typename T>
class laser_detector
{
public:

  bool detected; 
  double detected_t2_score1;
  double detected_t2_score2;
  cv::Point detected_point;
  int detected_size;
  

  cv::Mat_<double> mean_hsv_laser;
  cv::Mat_<double> cov_hsv_laser;
  int n_hsv_laser;
  cv::Mat_<double> list_hsv_laser ;
  void initialize() 
  {
    Mat mu_tmp;
    n_hsv_laser=list_hsv_laser.size[0];
    cv::calcCovarMatrix(list_hsv_laser, cov_hsv_laser, mu_tmp, CV_COVAR_NORMAL | CV_COVAR_ROWS | CV_COVAR_SCALE);
    cov_hsv_laser *= ( (double) n_hsv_laser )/( (double) n_hsv_laser -1 );
    mean_hsv_laser=mu_tmp.t();
    std::cout << n_hsv_laser <<  std::endl;
    std::cout << mean_hsv_laser <<  std::endl;
    std::cout << cov_hsv_laser <<  std::endl;
  }

  laser_detector(cv::Mat_<double> & list_hsv_laser ) 
    : list_hsv_laser(list_hsv_laser)
    , mean_hsv_laser(3,1),cov_hsv_laser(3,3)
  {
    initialize(); 
  }

  laser_detector( ) 
    : list_hsv_laser(list_hsv_laser_default) 
    , mean_hsv_laser(3,1),cov_hsv_laser(3,3)
  {
    initialize(); 
  }


  bool detect(cv::Mat image)
  {
    detected=false;
    detected_t2_score2= 1000;
    Mat hsvimg;
    cv::cvtColor(image,hsvimg,CV_BGR2HSV);
    vector<Mat> channels;
    cv::split(hsvimg, channels);

    // //SIFT
    cv::SiftFeatureDetector detector;
    cv::SiftDescriptorExtractor extractor;    
    // //SURF
    //cv::SurfFeatureDetector detector(1000);
    //cv::SurfFeatureDetector detector(10);
    //cv::SurfDescriptorExtractor extractor;

    int i_channel=2;
    std::vector<cv::KeyPoint> keypoints1;
    detector.detect(channels[i_channel], keypoints1);

    std::sort(keypoints1.begin(),keypoints1.end(),compare_response);
    unsigned int i;

    for(i = 0; i < keypoints1.size(); i++ )
      {

	int ks=keypoints1[i].size;
	int kx=keypoints1[i].pt.x;
	int ky=keypoints1[i].pt.y;
	if(i < MAX_N_FEATURE_POINT 
	   //&& ks > 1 
	   //&& ks < 4
	   && ks < 5
	   &&  kx+ks<image.cols &&  ky+ks <image.rows 
	   &&  kx-ks >0 &&  ky-ks >0 
	   )
	  {
	    cv::Rect roi(kx-ks, ky-ks, ks*2+1, ks*2+1);

	    int n_sub_rows=(ks*2+1)* (ks*2+1);
	    Mat_<double> sub_rows(n_sub_rows,3);

	    for(i_channel=0;i_channel<3;i_channel++){
	      cv::Mat_<uchar>  channel_roi(  channels[i_channel], roi);
	      cv::Mat_<uchar>::iterator itf = channel_roi.begin();
	      int k=0;
	      for (; itf != channel_roi.end(); ++itf,++k )
		{
		  sub_rows(k,i_channel)= *itf;;
		}}
	    Mat cov, mu;
	    cv::calcCovarMatrix(sub_rows, cov, mu, CV_COVAR_NORMAL | CV_COVAR_ROWS| CV_COVAR_SCALE);
	    cov *= ( (double) n_sub_rows )/( (double) n_sub_rows -1 );

	    Mat_<double> mu_ = mu.t();
	    double t2_score1 = hotelling1_t2_test<double>(mu_,mean_hsv_laser,cov,n_sub_rows);
	    double t2_score2 = hotelling2_t2_test<double>(mu_,mean_hsv_laser,cov,cov_hsv_laser,n_sub_rows,n_hsv_laser);


	    if(
	       t2_score1 < TH_T2_SCORE1
	       && 
	       t2_score1 >0
	       && 
	       t2_score2 >0
	       )
	      {
		detected=true;
		//if(detected){
		  if(t2_score2 < detected_t2_score2 ){
		    detected_t2_score1=t2_score1 ;
		    detected_t2_score2=t2_score2;
		    detected_point=keypoints1[i].pt;
		    detected_size=keypoints1[i].size;;
		  }
		// }else{
		//   detected=true;
		//   t2_score1 = detected_t2_score1;
		//   t2_score2 = detected_t2_score2;
		//   detected_point=keypoints1[i].pt;
		//   detected_size=keypoints1[i].size;;
		//}
		std::cout << t2_score2 << " " << t2_score1 ;
		std::cout <<  roi ;
		std::cout << mu << std::endl;
		//cv::circle(image,keypoints1[i].pt,ks*5,CV_RGB(255,0,0),3,8,0);
	      }
	  }
      }
    //std::cout << std::endl;
    return detected;
  }
};







#if 0

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

#endif
