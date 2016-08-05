#pragma once

#include <vector>
#include <cassert>

#include "alns/beta_distribution.hpp"

namespace alns{

  struct BernoulliThompsonSampler
  {
  public:
    explicit BernoulliThompsonSampler(std::size_t n,
				      double initial_succ=0.5,
				      double initial_fail=0.5,
				      double decay_rate=0.99,
				      double min_score=1.0e-3
				      )
      : succs(n, initial_succ), fails(n, initial_fail), decay_rate(decay_rate), min_score(min_score)
    {
      assert(initial_succ > 0);
      assert(initial_fail > 0);	
    }

    explicit BernoulliThompsonSampler(std::vector<double> initial_succs,
				      std::vector<double> initial_fails,
				      double decay_rate=0.99,
				      double min_score=1.0e-3
				      )
      : succs(initial_succs), fails(initial_fails), decay_rate(decay_rate), min_score(min_score)
    {
      assert(initial_succs.size() == initial_fails.size());
      for(std::size_t i=0; i<initial_succs.size(); ++i){
	assert(0 < initial_succs[i]);
	assert(0 < initial_fails[i]);
      }
    }
      
  public:

    inline std::size_t size() const
    { return succs.size(); }
      
    template <class RandomGenerator>
    inline std::size_t operator()(RandomGenerator& gen)
    {
      std::size_t result = std::numeric_limits<std::size_t>::max();
      double theta_max = -1;
      beta_distribution<double> beta;
      for(std::size_t i=0, I=this->size(); i < I; ++i){
	double theta = beta(gen, std::make_pair(succs[i], fails[i]));
	if(theta > theta_max){
	  result = i;
	  theta_max = theta;
	}
      }
      return result;
    }

    void feedback(std::size_t i, double delta_succ, double delta_fail)
    {
      assert(i < this->size());
      assert(delta_succ >= 0);
      assert(delta_fail >= 0);
      this->succs[i] += delta_succ;
      this->fails[i] += delta_fail;
    }

    void decay()
    {
      for(std::size_t i=0, I=this->size(); i < I; ++i){
	this->succs[i] *= this->decay_rate;
	if(this->succs[i] < min_score){
	  this->succs[i] = min_score;
	}
	this->fails[i] *= this->decay_rate;
	if(this->fails[i] < min_score){
	  this->fails[i] = min_score;
	}
      }
    }
      
  public:
    std::vector<double> succs; // array of scores related to success
    std::vector<double> fails; // array of scores related to failure
    double decay_rate; // succs and fails are multiplied by decay_rate
    double min_score;
  }; // end of BernoulliThompsonSampler

} // end of alns
