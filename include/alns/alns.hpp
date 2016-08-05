#pragma once

#include "alns/thompson_sampler.hpp"

namespace alns{

  template <class Solution,
	    class RemoverSampler=BernoulliThompsonSampler,
	    class InserterSampler=BernoulliThompsonSampler>
  struct ALNS
  {
    ALNS(const RemoverSampler& remover_sampler,
	 const InserterSampler& inserter_sampler)
      : remover_sampler(remover_sampler), inserter_sampler(inserter_sampler)
    {}

    template <class RandomGenerator>
    inline std::pair<std::size_t, std::size_t> draw()
    { return std::make_pair(this->remover_sampler(gen), this->inserter_sampler(gen)); }
    
    inline void feedback(std::size_t i, double delta_succ, double delta_fail)
    {
      this->remover_sampler.feedback(i,delta_succ, delta_fail);
      this->inserter_sampler.feedback(i,delta_succ, delta_fail);      
    }
    
    inline void decay(std::size_t i, double delta_succ, delta_fail)
    {
      this->remover_sampler.decay();
      this->inserter_sampler.decay();
    }
    
    Sampler remover_sampler;
    Sampler inserter_sampler;
  }; // end of struct ALNS

} // end of namespace alns














