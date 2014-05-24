#include "ublas_hotelling_t2.hpp"

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Main

#include <boost/test/included/unit_test.hpp>



BOOST_AUTO_TEST_CASE( one_sample1 )
{
  //http://www.real-statistics.com/multivariate-statistics/hotellings-t-square-statistic/one-sample-hotellings-t-square/
  //example1 data

  double mean1_array[]={5.6 , 7.4 , 5.08 , 5.04 , 12.88};
  double mean2_array[]={7   , 8   , 5    , 7    , 9};
  int n1=25;
  double cov1_array[][5]={
    {4.3333333 , 0.916667 , 1.533333 , -0.81667 , -4.46667},
    {0.916667  , 2.916667 , 2.425    , 0.025    , -2.40833},
    {1.533333  , 2.425    , 7.41     , -1.21167 , -2.57333},
    {-0.81667  , 0.025    , -1.21167 , 4.706667 , 1.963333},
    {-4.46667  , -2.40833 , -2.57333, 1.963333 , 20.86}
  };


  boost::numeric::ublas::vector<double >  mean1(5);
  std::copy( & mean1_array[0], & mean1_array[5],mean1.begin());
  boost::numeric::ublas::vector<double >  mean2(5);
  std::copy( & mean2_array[0], & mean2_array[5],mean2.begin());

  boost::numeric::ublas::matrix<double>  cov1(5,5);
  for (unsigned i = 0; i < cov1.size1 (); ++ i) 
        for (unsigned j = 0; j < cov1.size2 (); ++ j)
	  cov1(i,j)= cov1_array[i][j];

  BOOST_CHECK_EQUAL( 
		    hotelling_t2_1test(mean1,mean2,cov1,n1)
		    ,0.00015510119169827297);

}


BOOST_AUTO_TEST_CASE( two_sample )
{
// http://www.real-statistics.com/multivariate-statistics/hotellings-t-square-statistic/hotellings-t-square-independent-samples/
// example2 data

  double drug_mean_array[]={38.69 , 68.45 ,  18.3};
  double placebo_mean_array[]={39.44444 ,  62.27778 , 20.11111};
  unsigned n_drug=20,n_placebo=18;//,n_dim=3;  
 
  double drug_cov_array[][3]={
    {1.256737 , -9.94263 ,  3.266316},
    {-9.94263 , 169.3132  , -38.3  },
    {3.266316 , -38.3    ,  31.8 }
  };
  double placebo_cov_array[][3]={
    {1.496732 , -10.4307 , 0.647712 },
    {-10.4307 , 175.1536 , -28.5621 },
    {0.647712 , -28.5621 , 24.45752 }
  };

  boost::numeric::ublas::vector<double >  drug_mean(3);
  std::copy( & drug_mean_array[0], & drug_mean_array[3],drug_mean.begin());
  boost::numeric::ublas::vector<double >  placebo_mean(3);
  std::copy( & placebo_mean_array[0], & placebo_mean_array[3],placebo_mean.begin());
  boost::numeric::ublas::matrix<double>  drug_cov(3,3);
  for (unsigned i = 0; i < drug_cov.size1 (); ++ i) 
        for (unsigned j = 0; j < drug_cov.size2 (); ++ j)
	  drug_cov(i,j)= drug_cov_array[i][j];
  boost::numeric::ublas::matrix<double>  placebo_cov(3,3);
  for (unsigned i = 0; i < placebo_cov.size1 (); ++ i) 
        for (unsigned j = 0; j < placebo_cov.size2 (); ++ j)
	  placebo_cov(i,j)= placebo_cov_array[i][j];
  BOOST_CHECK_EQUAL( 
		    hotelling_t2_2test(drug_mean,placebo_mean,drug_cov,placebo_cov,n_drug,n_placebo)
		    ,0.29169109743628624);

}


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
