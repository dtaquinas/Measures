#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
//#include <boost/multiprecision/mpfr.hpp>
#include <boost/multiprecision/cpp_dec_float.hpp>
#include <boost/math/special_functions/bessel.hpp>
#define PREC 300    //change this to set precision

using namespace boost::multiprecision;

typedef number<cpp_dec_float<PREC> > big_float;

big_float dpii_iter( big_float current, big_float previous, big_float alpha, big_float beta, big_float gamma, int n )
{
  return (current * (alpha * n + beta)) / (1 - current * current) - previous; 
}

big_float adpii_iter( big_float x, big_float y, big_float alpha )
{
  return (x * alpha) / (1 - x * x) - y;
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

int compute_verb ( std::vector<big_float > &verblunsky, big_float lambda, bool use_bessel, double init )
{
  std::cout << "Computing verblunsky coefficients: initializing..." << std::endl;
  big_float coeff = -1*(2 / lambda);
  if (use_bessel)
    verblunsky[0] = initval( lambda );
  else
    verblunsky[0] = init;
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

int compute_xy ( std::vector<big_float > &x, std::vector<big_float> &y, big_float lambda, double init_x, double init_y )
{
  std::cout << "Computing verblunsky coefficients: initializing..." << std::endl;
  big_float coeff = -1*(2 / lambda);
  big_float temp = 0;
  x[0] = init_x;
  y[0] = init_y;
  int kmax = x.size() - 1;
  std::cout << "Computing verblunsky coefficients: iterating..." << std::endl;
  for (int k = 1; k <= kmax; k++)
  {
    temp = x[k-1];
    x[k] = adpii_iter(x[k-1], y[k-1], coeff);
    y[k] = temp;
  }
  std::cout << "Done computing verblunsky coefficients." << std::endl;
  return 0;
}

int compute_jac ( std::vector<big_float > &diag, std::vector<big_float > &offdiag, const std::vector<big_float > & verblunsky )
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

int writeout_csv( std::string fname, int out_digits, std::vector<big_float> &verblunsky, std::vector<big_float> &diag, std::vector<big_float> &offdiag )
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

int writeout_xy( std::string fname, int out_digits, std::vector<big_float> &x, std::vector<big_float> &y )
{
  std::cout << "Writing to file..." << std::endl;
  int terms = x.size();
  std::ofstream outfile{fname};
  outfile << "Index x y" << std::endl;
  outfile << std::setprecision(out_digits);
//  outfile << std::setprecision(std::numeric_limits<big_float>::maxdigits10);
  for (int k = 0; k < terms; k++)
  {
    outfile << k << " " << x[k] << " " << y[k] << std::endl;
  }
  outfile.close();
  return 0;
}

int get_input( int &digits, int &terms, double &lambda, std::string &fname, double &init_val, double &init_x, double &init_y, bool &use_bessel )
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
  while (true) 
  {
    std::cout << "Use mod bessel initial condition? (y/n) " << std::endl;
    std::getline(std::cin, s);
    if (s == "y")
    {
      use_bessel = true;
      break;
    }
    if (s == "n")
    {
      use_bessel = false;
      break;
    }
    std::cout << "Input should be (y/n) " << std::endl;
  }
  if (!use_bessel)
  {
    while (true) 
    {
      std::cout << "Enter alternative initial value x: " << std::endl;
      std::getline(std::cin, s);
      std::stringstream instr(s);
      if (instr >> init_x)
        break;
      std::cout << "Input should be a float: " << std::endl;
    }
  }
  if (!use_bessel)
  {
    while (true)
    {
      std::cout << "Enter alternative initial value y: " << std::endl;
      std::getline(std::cin, s);
      std::stringstream instr(s);
      if (instr >> init_y)
        break;
      std::cout << "Input should be a float: " << std::endl;
    }
  }
  std::cout << "Enter output filename: " << std::endl;
  std::getline(std::cin, fname);
}

int main(int argc, char *argv[])
{

  int digits;
  int terms;
  double lambda_in;
  double init_x;
  double init_y;
  std::string fname;
  bool use_bessel;
  double init_val;

  get_input( digits, terms, lambda_in, fname, init_val, init_x, init_y, use_bessel );
//  big_float::default_precision(digits);
  big_float lambda(lambda_in);

  if (use_bessel) {
    int jterms = (terms / 2) - 1;
    std::vector<big_float > verblunsky(terms);
    std::vector<big_float > diag(jterms);
    std::vector<big_float > offdiag(jterms); 
    compute_verb (verblunsky, lambda, use_bessel, init_val );
    compute_jac (diag, offdiag, verblunsky);
    writeout_csv( fname, digits, verblunsky, diag, offdiag );
  }
  if (!use_bessel) {
    std::vector<big_float> x(terms);
    std::vector<big_float> y(terms);
    compute_xy (x, y, lambda, init_x, init_y );
    writeout_xy ( fname, digits, x, y );
  }
  return 0;
}




