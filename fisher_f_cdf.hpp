#ifndef FISHER_F_CDF_HPP
#define FISHER_F_CDF_HPP

#include <boost/math/distributions/fisher_f.hpp>

template<class T>
T fisher_f_score_to_cdf(T f_score, unsigned k, unsigned n){
  boost::math::fisher_f dist(k, n-k);
  T p_score =boost::math::cdf(dist, f_score);
  return p_score;}

template<class T>
T fisher_f_score_to_cdf_complement(T f_score, unsigned k, unsigned n){
  boost::math::fisher_f dist(k, n-k);
  T p_comp =boost::math::cdf(complement(dist, f_score));
  return p_comp;}

#endif // FISHER_F_CDF_HPP

