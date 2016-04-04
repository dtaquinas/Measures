#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <boost/multiprecision/mpfr.hpp>
#include <boost/multiprecision/cpp_dec_float.hpp>
#include <boost/math/special_functions/bessel.hpp>
#define PREC 300    //change this to set precision

using namespace boost::multiprecision;

typedef number<cpp_dec_float<PREC> > big_float;

big_float dpii_iter( big_float current, big_float previous, big_float alpha, big_float beta, big_float gamma, int n )
{
  return (current * (alpha * n + beta)) / (1 - current * current) - previous; 
}

big_float initval( big_float lambda )
{
  using namespace boost::math;
  return cyl_bessel_i(1, lambda) / cyl_bessel_i(0, lambda);
}

big_float jac_offdiag( big_float a, big_float b, big_float c )
{
  return (1 - a)*(1 - b * b)*(1 + c);
}

big_float jac_diag( big_float a, big_float b, big_float c )
{
  return (1 - b)*a - (1 + b)*c;
}

int compute_verb ( std::vector<big_float > &verblunsky, big_float lambda )
{
  std::cout << "Computing verblunsky coefficients: initializing..." << std::endl;
  big_float coeff = -1*(2 / lambda);
  verblunsky[0] = initval( lambda );
  verblunsky[1] = dpii_iter( verblunsky[0], (big_float) -1, coeff, coeff, (big_float) 0, 0 );
  int kmax = verblunsky.size() - 1;
  std::cout << "Computing verblunsky coefficients: iterating..." << std::endl;
  for (int k = 2; k <= kmax; k++)
  {
    verblunsky[k] = dpii_iter( verblunsky[k-1], verblunsky[k-2], coeff, coeff, (big_float) 0, (k-1) );
  }
  std::cout << "Done computing verblunsky coefficients." << std::endl;
  return 0;
}

int compute_jac ( std::vector<big_float > &diag, std::vector<big_float > &offdiag, std::vector<big_float > &verblunsky )
{
  std::cout << "Computing jacobi coefficients: initializing..." << std::endl;
  int kmax = offdiag.size() - 1;
  diag[0] = jac_diag( verblunsky[1], verblunsky[0], (big_float) -1 );
  offdiag[0] = 2 * verblunsky[0];
  std::cout << "Computing jacobi coefficients: iterating..." << std::endl;
  for (int k = 1; k <= kmax; k++)
  {
    diag[k] = jac_diag( verblunsky[2*k], verblunsky[2*k - 1], verblunsky[2*k - 2] );
    offdiag[k] = jac_offdiag( verblunsky[2*k + 1], verblunsky[2*k], verblunsky[2*k - 1] );
  }
  std::cout << "Done computing jacobi coefficients." << std::endl;
  return 0;
}

int writeout_csv( std::string fname, int out_digits, std::vector<big_float> verblunsky, std::vector<big_float> diag, std::vector<big_float> offdiag )
{
  std::cout << "Writing to file..." << std::endl;
  int terms = (int) verblunsky.size() / 2 - 1;
  std::ofstream outfile{fname};
  outfile << "Index alpha b square(a) diagnostic(1-sq(a))" << std::endl;
  outfile << std::setprecision(out_digits);
//  outfile << std::setprecision(std::numeric_limits<big_float>::maxdigits10);
  for (int k = 0; k < terms; k++)
  {
    big_float diagnostic;
    diagnostic = log(abs(1. - offdiag[k]));
    outfile << k << " " << verblunsky[k] << " " << diag[k] << " " << offdiag[k] << " " << diagnostic << std::endl;
  }
  outfile.close();
  return 0;
}

int get_input( int &digits, int &terms, double &lambda, std::string &fname )
{
  std::string s;
  while (true) 
  {
    std::cout << "Enter number of digits to output: " << std::endl;
    std::getline(std::cin, s);
    std::stringstream instr(s);
    if (instr >> digits)
      break;
    std::cout << "Input should be an integer: " << std::endl;
  }
  while (true) 
  {
    std::cout << "Enter number of terms to compute: " << std::endl;
    std::getline(std::cin, s);
    std::stringstream instr(s);
    if (instr >> terms)
      break;
    std::cout << "Input should be an integer: " << std::endl;
  }
  while (true) 
  {
    std::cout << "Enter value of lambda to use: " << std::endl;
    std::getline(std::cin, s);
    std::stringstream instr(s);
    if (instr >> lambda)
      break;
    std::cout << "Input should be a float: " << std::endl;
  }
  std::cout << "Enter output filename: " << std::endl;
  std::getline(std::cin, fname);
}

int main(int argc, char *argv[])
{

  int digits;
  int terms;
  double lambda_in;
  std::string fname;

  get_input( digits, terms, lambda_in, fname );
//  big_float::default_precision(digits);
  big_float lambda(lambda_in);

  int jterms = (terms / 2) - 1;
  std::vector<big_float > verblunsky(terms);
  std::vector<big_float > diag(jterms);
  std::vector<big_float > offdiag(jterms); 

  compute_verb (verblunsky, lambda);
  compute_jac (diag, offdiag, verblunsky);
  writeout_csv( fname, digits, verblunsky, diag, offdiag );
  return 0;
}




