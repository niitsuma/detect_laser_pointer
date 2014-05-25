#include "cv_hotelling_t2.hpp"

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Main

#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE( one_sample1 )
{
  // http://www.real-statistics.com/multivariate-statistics/hotellings-t-square-statistic/one-sample-hotellings-t-square/
  //example1 data


  int n1=25;
  cv::Mat_<double> mean1= (cv::Mat_<double>(5,1) 
			   << 5.6 , 7.4 , 5.08 , 5.04 , 12.88);
  cv::Mat_<double> mean2= (cv::Mat_<double>(5,1) 
			   << 7   , 8   , 5    , 7    , 9);
  cv::Mat_<double> cov1
    = (cv::Mat_<double>(5,5) 
       << 
    4.3333333 , 0.916667 , 1.533333 , -0.81667 , -4.46667,
    0.916667  , 2.916667 , 2.425    , 0.025    , -2.40833,
    1.533333  , 2.425    , 7.41     , -1.21167 , -2.57333,
    -0.81667  , 0.025    , -1.21167 , 4.706667 , 1.963333,
    -4.46667  , -2.40833 , -2.57333, 1.963333 , 20.86
       );
  //std::cout << mean1 << std::endl;
  //std::cout << cov1 << std::endl;
  BOOST_CHECK_EQUAL( 
		    hotelling1_t2_score(mean1,mean2,cov1,n1)
		    ,52.672408623864229);
  BOOST_CHECK_EQUAL( 
		    hotelling1_f_score(mean1,mean2,cov1,n1)
		    ,8.7787347706440375);
  BOOST_CHECK_EQUAL( 
		    hotelling1_t2_test(mean1,mean2,cov1,n1)
		    ,0.99984489880830174);

  //test cv::calcCovarMatrix
  cv::Mat_<double> vals1 
    = 
    (cv::Mat_<double>(25,1) 
     <<
     6,6,5,10,7,6,5,3,8,8,5,8,5,4,2,7,4,5,7,1,5,4,7,4,8);
  cv::Mat_<double> cov, mu;
  cv::calcCovarMatrix(vals1,cov, mu, CV_COVAR_NORMAL | CV_COVAR_ROWS| CV_COVAR_SCALE);
  //std::cout << mu << std::endl;
  //std::cout << cov << std::endl;
  //std::cout << cov*25.0/24.0 << std::endl;
  //std::cout << cov(0,0)*n1/(n1-1) << std::endl;

  BOOST_CHECK_EQUAL( 
		    cov(0,0)*n1/(n1-1)
		    ,
		    cov1(0,0)
		     );

}


BOOST_AUTO_TEST_CASE( two_sample )
{
// http://www.real-statistics.com/multivariate-statistics/hotellings-t-square-statistic/hotellings-t-square-independent-samples/
// example2 data

  unsigned n_drug=20,n_placebo=18;
  cv::Mat_<double> mean_drug
    = (cv::Mat_<double>(3,1) 
       << 38.69 , 68.45 ,  18.3);
  cv::Mat_<double> mean_placebo
    = (cv::Mat_<double>(3,1) 
       << 39.44444 ,  62.27778 , 20.11111);

  cv::Mat_<double> cov_drug
    = (cv::Mat_<double>(3,3) 
       <<    
       1.256737 , -9.94263 ,  3.266316,
       -9.94263 , 169.3132  , -38.3  ,
	3.266316 , -38.3    ,  31.8 );
  cv::Mat_<double> cov_placebo
    = (cv::Mat_<double>(3,3) 
       << 
       1.496732 , -10.4307 , 0.647712 ,
       -10.4307 , 175.1536 , -28.5621 ,
       0.647712 , -28.5621 , 24.45752 );
  BOOST_CHECK_EQUAL
    (
     hotelling2_t2_score(mean_drug,mean_placebo,cov_drug,cov_placebo,n_drug,n_placebo)
     ,4.1160139772437194
     );

  BOOST_CHECK_EQUAL
    (
     hotelling2_f_score(mean_drug,mean_placebo,cov_drug,cov_placebo,n_drug,n_placebo)
     ,1.2957821780211711);

  BOOST_CHECK_EQUAL
    (
     round
     (10000000000000 *
      hotelling2_t2_test(mean_drug,mean_placebo,cov_drug,cov_placebo,n_drug,n_placebo))
     ,
     round
     (10000000000000 *
      0.70830890256371382
      ));

}


#if 0
BOOST_AUTO_TEST_CASE( one_sample2 )
{

//http://faculty.chicagobooth.edu/ruey.tsay/teaching/ama/sp2012/lec2-12.pdf
//http://faculty.chicagobooth.edu/ruey.tsay/teaching/ama/sp2014/m-ba4c9807.txt
//example 1 data

  double mean1_array[]={-0.26294605, 0.61075048, 0.67154781, 0.02679363};
  double mean2_array[]={0,0,0,0};
  int n1=121;
  double cov1_array[][4]={
    {127.011719, 28.776911, 8.046536, 50.919252},
    {28.776911, 80.328617, 8.385634, 13.216236},
    {8.046536, 8.385634, 40.750353, -8.710395},
    {50.919252, 13.216236, -8.710395, 122.712745}
  };

  boost::numeric::ublas::vector<double >  mean1(4);
  std::copy( & mean1_array[0], & mean1_array[3],mean1.begin());
  boost::numeric::ublas::vector<double >  mean2(4);
  std::copy( & mean2_array[0], & mean2_array[3],mean2.begin());

  boost::numeric::ublas::matrix<double>  cov1(4,4);
  for (unsigned i = 0; i < cov1.size1 (); ++ i) 
        for (unsigned j = 0; j < cov1.size2 (); ++ j)
	  cov1(i,j)= cov1_array[i][j];
 
  BOOST_CHECK_EQUAL( 
		    hotelling_t2_1test(mean1,mean2,cov1,n1)
		    , 0.2621048702199083);
}
#endif
