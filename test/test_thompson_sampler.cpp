#include <vector>
#include <random>
#include <limits>
#include <iostream>

#include "gtest/gtest.h"
#include "alns/thompson_sampler.hpp"

using ::testing::InitGoogleTest;
using ::testing::Test;

template <class T>
std::vector<std::size_t> compute_argmax(const std::vector<T>& vals)
{
  std::vector<std::size_t> result;
  T max_val = std::numeric_limits<T>::min();
  for(std::size_t i=0; i<vals.size(); ++i){
    T val = vals[i];
    if(val > max_val){
      max_val = val;
      result.clear();
      result.push_back(i);
    }else if(val == max_val){
      result.push_back(i);
    }
  }
  return result;
}

template <class RandomGenerator>
void test_sampler(RandomGenerator& gen, const std::vector<double>& thetas, const std::size_t N)
{
  alns::BernoulliThompsonSampler sampler(thetas.size());
  typedef std::uniform_real_distribution<double> uniform_type;
  uniform_type uniform;
  std::vector<std::size_t> argmax_vector = compute_argmax<double>(thetas);
  std::set<std::size_t> argmax(argmax_vector.begin(), argmax_vector.end());
  std::vector<std::size_t> counts(thetas.size(), 0);
  for(std::size_t n = 0; n < N; ++n){
    std::size_t chosen = sampler(gen);
    double theta = thetas[chosen];

    if(uniform(gen, typename uniform_type::param_type(0.0, 1.0)) < theta){
      sampler.feedback(chosen, 1.0, 0.0);
    }else{
      sampler.feedback(chosen, 0.0, 1.0);      
    }

    counts[chosen] += 1;
  }

  for(std::size_t i=0; i<thetas.size(); ++i){
    std::cout << i << "\t" << thetas[i] << "\t" << counts[i] << std::endl;
  }

  std::vector<std::size_t> emp_argmax = compute_argmax<std::size_t>(counts);
  for(std::size_t i=0; i<emp_argmax.size(); ++i){
    ASSERT_TRUE(argmax.find(emp_argmax[i]) != argmax.end());
  }
}

template <class RandomGenerator>
std::vector<double> generate(RandomGenerator& gen, std::size_t n)
{
  std::vector<double> result;
  typedef std::uniform_real_distribution<double> uniform_type;
  uniform_type uniform;
  for(std::size_t i = 0; i < n; ++ i){
    result.push_back(uniform(gen, typename uniform_type::param_type(0.0, 1.0)));
  }
  return result;
}

TEST(BernoulliThompsonSamplerTest, Argmax10_100000){
  std::mt19937 gen(10+100000);
  for(std::size_t i=0;i<5;++i){
    std::vector<double> thetas = generate(gen, 10);    
    test_sampler(gen, thetas, 100000);
  }
}

TEST(BernoulliThompsonSamplerTest, Argmax100_100000){
  std::mt19937 gen(100+100000);
  for(std::size_t i=0;i<5;++i){
    std::vector<double> thetas = generate(gen, 100);
    test_sampler(gen, thetas, 100000);
  }
}

TEST(BernoulliThompsonSamplerTest, Argmax1000_100000){
  std::mt19937 gen(1000+100000);
  for(std::size_t i=0;i<5;++i){
    std::vector<double> thetas = generate(gen, 1000);    
    test_sampler(gen, thetas, 100000);
  }
}
