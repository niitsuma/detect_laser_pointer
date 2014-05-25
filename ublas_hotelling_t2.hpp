
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/vector_proxy.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/triangular.hpp>
#include <boost/numeric/ublas/lu.hpp>
#include <boost/numeric/ublas/io.hpp>

#include <boost/math/distributions/fisher_f.hpp>


/* Matrix inversion routine.
 Uses lu_factorize and lu_substitute in uBLAS to invert a matrix */

template<class T>
bool invert_matrix(const boost::numeric::ublas::matrix<T>& input, boost::numeric::ublas::matrix<T>& inverse)
{
	typedef boost::numeric::ublas::permutation_matrix<std::size_t> pmatrix;

	// create a working copy of the input
	boost::numeric::ublas::matrix<T> A(input);

	// create a permutation matrix for the LU-factorization
	pmatrix pm(A.size1());

	// perform LU-factorization
	int res = boost::numeric::ublas::lu_factorize(A, pm);
	if (res != 0)
		return false;

	// create identity matrix of "inverse"
	inverse.assign(boost::numeric::ublas::identity_matrix<T> (A.size1()));

	// backsubstitute to get the inverse
	boost::numeric::ublas::lu_substitute(A, pm, inverse);

	return true;
}


//  One Sample Hotelling’s T-square
template<class T>
T hotelling_t2_1test(
  boost::numeric::ublas::vector<T> mean1,
  boost::numeric::ublas::vector<T> mean2,
  boost::numeric::ublas::matrix<T> cov1,
  unsigned n1
){
  unsigned k=mean1.size();  

  boost::numeric::ublas::vector<T > mean_diff=mean1-mean2;
  boost::numeric::ublas::matrix<T>  cov_inv(cov1);
  invert_matrix(cov1  ,cov_inv);
  T t2_score=
  boost::numeric::ublas::inner_prod(
			      boost::numeric::ublas::prod( cov_inv,mean_diff)
			      ,mean_diff)*n1;
  T f_score= (t2_score*(n1 - k))/(k*(n1-1));

  std::cout << t2_score   << std::endl;
  std::cout << f_score   << std::endl;

  boost::math::fisher_f dist(k, n1-k);
  T p_score =boost::math::cdf(dist, f_score);

  std::cout << p_score << std::endl;
  T p_comp =boost::math::cdf(complement(dist, f_score));
  std::cout << p_comp << std::endl;
  //return p_comp;
  return p_score;
}



//Hotelling’s T-square Test for Two Independent Samples
template<class T>
T hotelling_t2_2test(
  boost::numeric::ublas::vector<T> mean1,
  boost::numeric::ublas::vector<T> mean2,
  boost::numeric::ublas::matrix<T> cov1,
  boost::numeric::ublas::matrix<T> cov2, 
  unsigned n1,
  unsigned n2){
  unsigned k=mean1.size();  
//int n = n1 + n2 -1;
  boost::numeric::ublas::vector<T > mean_diff=mean1-mean2;
  boost::numeric::ublas::matrix<T>  
  pooled_cov=(cov1*(n1-1)+cov2*(n2-1))*1.0/(n1+n2-2) ;
  pooled_cov *= (1.0/n1+1.0/n2 );
  boost::numeric::ublas::matrix<T>  pooled_cov_inv(pooled_cov);
  invert_matrix(   pooled_cov  ,pooled_cov_inv);
//std::cout << mean_diff << std::endl;  
  T t2_score=
  boost::numeric::ublas::inner_prod(
			      boost::numeric::ublas::prod( pooled_cov_inv,mean_diff)
			      ,mean_diff);
  T f_score= (t2_score*(n1 + n2 - 1- k))/(k*(  n1 + n2 - 2));

  std::cout << t2_score   << std::endl;
  std::cout << f_score   << std::endl;

  boost::math::fisher_f dist(k, n1+n2-1-k);
  T p_score =boost::math::cdf(dist, f_score);

  std::cout << p_score << std::endl;
  T p_comp =boost::math::cdf(complement(dist, f_score));
  std::cout << p_comp << std::endl;
  //return p_comp;
  return p_score;
}



