#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <boost/multiprecision/cpp_float_dec.hpp>
#include <boost/math/special_functions/bessel.hpp>

{
using namespace boost::multiprecision;

  cpp_dec_float dpii_iter( cpp_dec_float current, cpp_dec_float previous, cpp_dec_float alpha, cpp_dec_float beta, cpp_dec_float gamma, int n )
  {
    return (current * (alpha * n + beta)) / (1 - current * current) - previous; 
  }

  cpp_dec_float initval( cpp_dec_float lambda )
  {
    using namespace boost::math;
    return cyl_bessel_i(1, lambda) / cyl_bessel_i(0, lambda);
  }

  cpp_dec_float jac_offdiag( cpp_dec_float a, cpp_dec_float b, cpp_dec_float c )
  {
    return (1 - a)*(1 - b * b)*(1 + c);
  }

  cpp_dec_float jac_diag( cpp_dec_float a, cpp_dec_float b, cpp_dec_float c )
  {
    return (1 - b)*a - (1 + b)*c;
  }

  int compute_verb ( std::vector<cpp_dec_float> &verblunsky, cpp_dec_float lambda )
  {
    const cpp_dec_float coeff = -1*(2 / lambda);
    verblunsky[0] = initval( lambda );
    verblunsky[1] = dpii_iter( verblunsky[0], (cpp_dec_float) -1, coeff, coeff, 0 );
    int kmax = verblunsky.size();

    for (int k = 2; k <= kmax; k++)
    {
      verblunsky[k] = dpii_iter( verblunsky[k-1], verblunsky[k-2], coeff, coeff, (cpp_dec_float) 0, (k-1) );
    }
    return 0;
  }

  int compute_jac ( std::vector<cpp_dec_float> &diag, std::vector<cpp_dec_float> &offdiag, const std::vector<cpp_dec_float> &verblunsky )
  {
    int kmax = offdiag.size();
  
    diag[0] = jac_diag( verblunsky[1], verblunsky[0], (cpp_dec_float) -1 );
    offdiag[0] = 2 * verblunsky[0];
    for (int k = 1; k <= kmax; k++)
    {
      diag[k] = jac_diag( verblunsky[2*k], verblunsky[2*k - 1], verblunsky[2*k - 2] );
      offdiag[k] = jac_offdiag( verblunsky[2*k + 1], verblunsky[2*k], verblunsky[2*k - 1] );
    }
  return 0;
  }
  
  int writeout_csv( std::string fname, std::vector<cpp_dec_float> verblunsky, std::vector<cpp_dec_float> diag, std::vector<cpp_dec_float> offdiag )
  {
    std::ofstream outfile;
    outfile.open( fname );
    outfile << "Index alpha b a" << std::endl;
    outfile << std::setprecision(std::numeric_limits<cpp_dec_float_100>::max_digits10);
    for (int k = 0; k < terms; k++)
    {
      outfile << verblunsky[k] << " " << diag[k] << " " << offdiag[k] << std::endl;
    }
    outfile.close();
    return 0;
  }


  int main(int argc, char *argv[])
  {

    int digits;
    int terms;
  
    std::vector<cpp_dec_float<prec>> verblunsky;
    std::vector<cpp_dec_float<prec>> diag;
    std::vector<cpp_dec_float<prec>> offdiag; 
  
  
  
    writeout_csv( fname, verblunsky, diag, offdiag, terms );
    return 0;
  }


}


/*
mpfr_float mod_bessel( int terms, int index, mpfr_float z, mpfr_float eps )
{
  using namespace boost::multiprecision;
  mpfr_float result = 0;
  mpfr_float term = 1;
  mpfr_float s;

  s = (z * z) / 4;

  if index != 

  for (int k = 1; k <= terms; k++)
  {
  term = term * s;
  term = term / k;
  term = term / (k + index);
  result += term;
  }

  if (index != 0)
  {
  result = result * ((0.5 * z) ** index)
  }
}
*/

