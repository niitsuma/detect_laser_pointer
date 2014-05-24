#ifndef HOTELLING_T2_COMMON_HPP
#define HOTELLING_T2_COMMON_HPP

template<class T>
T hotelling_t2_score_to_f_score(T t2_score, unsigned k, unsigned n){
  T f_score= (t2_score*(n - k))/(k*(n-1));
  return f_score;}

#endif // HOTELLING_T2_COMMON_HPP

