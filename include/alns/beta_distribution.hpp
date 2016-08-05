#pragma once

#include <utility>
#include <limits>
#include <random>
#include <iostream>

namespace alns{

  template <class RealType = double>
  struct beta_distribution
  {
    typedef RealType result_type;
    typedef std::pair<RealType, RealType> param_type;

    beta_distribution(): param_(std::unique_ptr<param_type>(nullptr)) {}
    
    explicit beta_distribution(const param_type& param): param_(std::unique_ptr<param_type>(new param_type(param) )) {} // c++-11 not supported make_unique

    template <class RandomEngine>
    inline result_type operator()(RandomEngine& gen) const
    {
      assert( this->param_ != nullptr );
      return this->operator()(gen, *this->param_);
    }

    template <class RandomEngine>
    inline result_type operator()(RandomEngine& gen, const param_type& param) const;

    inline void reset() {}

    inline const param_type& param() const { return *param_; }

    inline void param(const param_type& param)
    { this->param_ = std::unique_ptr<param_type>(new param_type(param)); } // c++-11 not supported make_unique

    inline const param_type& alpha_beta() const { return *param_; }
    

  public:
    std::unique_ptr<param_type> param_; // define param_ as (unique_)ptr to make it nullable
    
  }; // end of struct beta_distribution


  // --------------------------------------------------------
  
  template <class RealType>
  template <class RandomEngine>
  typename beta_distribution<RealType>::result_type
  beta_distribution<RealType>::operator()
    (RandomEngine& gen, const param_type& param) const    
  {
    RealType alpha = param.first, beta = param.second;

    std::gamma_distribution<RealType> gamma;

    RealType
      y1 = gamma(gen, typename std::gamma_distribution<RealType>::param_type(alpha, 1.0)),
      y2 = gamma(gen, typename std::gamma_distribution<RealType>::param_type(beta, 1.0));

    result_type result = y1 / (y1 + y2);

    // handling the case where gamma_distribution returned 'inf'
    // these case will happen very rarely
    if(y1 == std::numeric_limits<RealType>::infinity()
       && y2 == std::numeric_limits<RealType>::infinity()){
      return alpha / (alpha + beta);
    }else if(y1 == std::numeric_limits<RealType>::infinity()){
      return 1 - std::numeric_limits<RealType>::epsilon();
    }else if(y2 == std::numeric_limits<RealType>::infinity()){
      return std::numeric_limits<RealType>::epsilon();
    }

    // //    if(result < std::numeric_limits<RealType>::epsilon()){
    // if(!(0 < result)){
    //   result += std::numeric_limits<RealType>::epsilon();
    //   assert( result > 0 );
    // }else if(!(result < 1)){
    //   result -= std::numeric_limits<RealType>::epsilon();
    //   assert( result < 1 );      
    // }
    // if( result <= 0 || result >= 1){
    //   std::cout << "@ " << result << std::endl;exit(0);
    // }
    // assert(0 < result && result < 1);
      
    return result;
  }    

  

} // end of namespace stdpp


