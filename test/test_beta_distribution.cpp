#include <cmath>
#include <algorithm>
#include <vector>

#include "gtest/gtest.h"

#include "alns/beta_distribution.hpp"

using ::testing::InitGoogleTest;
using ::testing::Test;

template <class T>
T compute_mean(const std::vector<T>& vals)
{ return std::accumulate(vals.begin(), vals.end(), 0.0) / vals.size(); }

template <class T>
T compute_stddev(const std::vector<T>& vals)
{
  T m = 0, s2 = 0;
  for(auto v : vals){
    m += v;
    s2 += v*v;
  }
  m /= vals.size();
  s2 /= vals.size();
  return std::sqrt(s2 - m*m);
}

template <class T>
void test_basic_statistics(T alpha, T beta, T eps=0.01)
{
  std::pair<T, T> param;
  param.first = alpha;
  param.second = beta;

  const std::size_t N = 1000000;
  std::mt19937 gen(0);

  // generate
  alns::beta_distribution<T> betadist;
  std::vector<T> vals;
  for(std::size_t n=0; n<N; ++n){
    T x = betadist(gen, param);
    vals.push_back(x);
  }

  // compute empirical mean, stddev
  T emp_mean = compute_mean(vals), emp_stddev = compute_stddev(vals);

  // theoretical mean, stddev
  T mean = alpha / (alpha + beta),
    stddev = std::sqrt(alpha * beta / (alpha+beta) / (alpha+beta) / (alpha + beta + 1));

  ASSERT_LE(std::abs(emp_mean - mean), eps);
  ASSERT_LE(std::abs(emp_stddev - stddev), eps);  
}

template <class T>
void test_operator(T alpha, T beta, T eps=0.01)
{
  std::pair<T, T> param;
  param.first = alpha;
  param.second = beta;

  const std::size_t N = 1000000;
  std::mt19937 gen(0);

  // generate
  alns::beta_distribution<T> betadist1, betadist2(param);
  std::vector<T> vals1, vals2;
  for(std::size_t n=0; n<N; ++n){
    vals1.push_back(betadist1(gen, param));
    vals2.push_back(betadist2(gen));
  }

  // compute empirical mean, stddev
  T emp_mean1 = compute_mean(vals1), emp_stddev1 = compute_stddev(vals1),
    emp_mean2 = compute_mean(vals2), emp_stddev2 = compute_stddev(vals2)
    ;

  ASSERT_LE(std::abs(emp_mean1 - emp_mean2), eps);
  ASSERT_LE(std::abs(emp_stddev1 - emp_stddev2), eps);

  // change settings
  alpha = alpha + 0.2;
  beta = beta + 0.5;
  param = std::make_pair(alpha, beta);
  std::vector<T> vals3;
  for(std::size_t n=0; n<N; ++n){
    vals3.push_back(betadist2(gen, param));
  }

  // compute empirical mean, stddev
  T emp_mean3 = compute_mean(vals3), emp_stddev3 = compute_stddev(vals3);

  // theoretical mean, stddev
  T mean3 = alpha / (alpha + beta),
    stddev3 = std::sqrt(alpha * beta / (alpha+beta) / (alpha+beta) / (alpha + beta + 1));

  ASSERT_LE(std::abs(emp_mean3 - mean3), eps);
  ASSERT_LE(std::abs(emp_stddev3 - stddev3), eps);  
  
}


TEST(FLoatBetaDistributionTest, PredictBasicStatistics_03_03){
  test_basic_statistics<float>(0.3, 0.3);
}

TEST(FLoatBetaDistributionTest, PredictBasicStatistics_03_05){
  test_basic_statistics<float>(0.3, 0.5);
}

TEST(FLoatBetaDistributionTest, PredictBasicStatistics_03_15){
  test_basic_statistics<float>(0.3, 1.5);
}

TEST(FLoatBetaDistributionTest, PredictBasicStatistics_05_03){
  test_basic_statistics<float>(0.5, 0.3);
}

TEST(FLoatBetaDistributionTest, PredictBasicStatistics_15_03){
  test_basic_statistics<float>(1.5, 0.3);
}

TEST(FLoatBetaDistributionTest, PredictBasicStatistics_05_05){
  test_basic_statistics<float>(0.5, 0.5);
}

TEST(FLoatBetaDistributionTest, PredictBasicStatistics_15_05){
  test_basic_statistics<float>(1.5, 0.5);
}

TEST(FLoatBetaDistributionTest, PredictBasicStatistics_05_15){
  test_basic_statistics<float>(0.5, 1.5);
}

TEST(FLoatBetaDistributionTest, PredictBasicStatistics_15_15){
  test_basic_statistics<float>(1.5, 1.5);
}

TEST(FloatBetaDistributionTest, OperatorCall){
  test_operator<float>(0.3, 0.5);
}


// -------

TEST(DoubleBetaDistributionTest, PredictBasicStatistics_03_03){
  test_basic_statistics<double>(0.3, 0.3);
}

TEST(DoubleBetaDistributionTest, PredictBasicStatistics_03_05){
  test_basic_statistics<double>(0.3, 0.5);
}

TEST(DoubleBetaDistributionTest, PredictBasicStatistics_03_15){
  test_basic_statistics<double>(0.3, 1.5);
}

TEST(DoubleBetaDistributionTest, PredictBasicStatistics_05_03){
  test_basic_statistics<double>(0.5, 0.3);
}

TEST(DoubleBetaDistributionTest, PredictBasicStatistics_15_03){
  test_basic_statistics<double>(1.5, 0.3);
}

TEST(DoubleBetaDistributionTest, PredictBasicStatistics_05_05){
  test_basic_statistics<double>(0.5, 0.5);
}

TEST(DoubleBetaDistributionTest, PredictBasicStatistics_15_05){
  test_basic_statistics<double>(1.5, 0.5);
}

TEST(DoubleBetaDistributionTest, PredictBasicStatistics_05_15){
  test_basic_statistics<double>(0.5, 1.5);
}

TEST(DoubleBetaDistributionTest, PredictBasicStatistics_15_15){
  test_basic_statistics<double>(1.5, 1.5);
}

// -------

TEST(DoubleBetaDistributionTest, OperatorCall){
  test_operator<double>(0.3, 0.5);
}

