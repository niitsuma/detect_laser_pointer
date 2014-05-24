#ifndef CV_HOTELLING_T2_HPP
#define CV_HOTELLING_T2_HPP

#include "cv_hotelling_t2_score.hpp"
#include "hotelling_t2_common.hpp"
#include "fisher_f_cdf.hpp"

//  One Sample Hotelling’s T-square
template<class T>
T hotelling1_t2_test(
  cv::Mat_<T> mean1,  cv::Mat_<T> mean2,  cv::Mat_<T> cov1,  unsigned n){
  unsigned k=mean1.size[0];
  T t2_score=hotelling1_t2_score(mean1, mean2,cov1, n);
  T f_score =hotelling_t2_score_to_f_score(t2_score,  k,  n);
  return fisher_f_score_to_cdf_complement(f_score,  k, n);
}


//Hotelling’s T-square Test for Two Independent Samples
template<class T>
T hotelling2_t2_test(
  cv::Mat_<T> mean1,  cv::Mat_<T> mean2,  cv::Mat_<T> cov1,  cv::Mat_<T> cov2,   unsigned n1,  unsigned n2){
  unsigned k=mean1.size[0]; 
  int n = n1 + n2 -1;
  T t2_score=hotelling2_t2_score(mean1, mean2,cov1,cov2, n1,n2);
  T f_score =hotelling_t2_score_to_f_score(t2_score,  k,  n);
  return fisher_f_score_to_cdf_complement(f_score,  k, n);
}

#endif // CV_HOTELLING_T2_HPP

