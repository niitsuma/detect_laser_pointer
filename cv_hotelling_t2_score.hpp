#ifndef CV_HOTELLING_T2_SCORE_HPP
#define CV_HOTELLING_T2_SCORE_HPP

#include <opencv2/opencv.hpp>
#include "hotelling_t2_common.hpp"

//  One Sample Hotelling’s T-square
template<class T>
T hotelling1_t2_score(
  cv::Mat_<T> mean1,  cv::Mat_<T> mean2,  cv::Mat_<T> cov1,  unsigned n1){
  //unsigned k=mean1.size[0];  
  cv::Mat_<T > mean_diff=mean1-mean2;
  T t2_score= mean_diff.dot(cov1.inv() * mean_diff)*n1;
  return t2_score;}

template<class T>
T hotelling1_f_score(
  cv::Mat_<T> mean1,  cv::Mat_<T> mean2,  cv::Mat_<T> cov1,  unsigned n){
  unsigned k=mean1.size[0];  
  T t2_score=hotelling1_t2_score(mean1, mean2,cov1, n);
  T f_score =hotelling_t2_score_to_f_score(t2_score,  k,  n);
  return f_score;}


//Hotelling’s T-square Test for Two Independent Samples
template<class T>
T hotelling2_t2_score(
  cv::Mat_<T> mean1,  cv::Mat_<T> mean2,  cv::Mat_<T> cov1,  cv::Mat_<T> cov2,   unsigned n1,  unsigned n2){
  //unsigned k=mean1.size[0]; 
  //int n = n1 + n2 -1;
  cv::Mat_<T> mean_diff=mean1-mean2;
#if 1
  cv::Mat_<T> 
    pooled_cov
    =(cov1*(n1-1)+cov2*(n2-1))*(((1.0/(n1+n2-2)) * (1.0/n1+1.0/n2 )));
#else
  cv::Mat_<T> 
    pooled_cov=(cov1*(n1-1)+cov2*(n2-1))*1.0/(n1+n2-2) ;
  pooled_cov *= (1.0/n1+1.0/n2 );
#endif
  T t2_score=mean_diff.dot(pooled_cov.inv() * mean_diff);
  return t2_score;
}


template<class T>
T hotelling2_f_score(
  cv::Mat_<T> mean1,  cv::Mat_<T> mean2,  cv::Mat_<T> cov1,  cv::Mat_<T> cov2,   unsigned n1,  unsigned n2){
  unsigned k=mean1.size[0]; 
  int n = n1 + n2 -1;
  T t2_score=hotelling2_t2_score(mean1, mean2,cov1,cov2, n1,n2);
  T f_score =hotelling_t2_score_to_f_score(t2_score,  k,  n);
  return f_score;}




#endif // CV_HOTELLING_T2_SCORE_HPP
