#ifndef MPFR_SUPPORT
#define MPFR_SUPPORT

#include <boost/multiprecision/mpfr.hpp>
#include <eigen3/Eigen/Core>
#define PREC 300  //change this to change precision

typedef boost::multiprecision::number<mpfr_float_backend<PREC> > big_float;

namespace Eigen {
  template<> struct NumTraits<big_float> {

  typedef big_float Real;
  typedef big_float NonInteger;
  typedef big_float Nested;

  enum {
    IsComplex = 0,
    IsInteger = 0,
    IsSigned = 1,
    RequireInitialization = 1,
    ReadCost = 1,
    AddCost = 3,
    MulCost = 3
  };
  
  };
}



#endif
