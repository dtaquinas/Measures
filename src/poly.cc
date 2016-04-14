#include <eigen3/Eigen/Core>

template<class coeff_t>
class Polynomial<coeff_t> {
  private:
  std::vector<coeff_t> coeff;
  public:
  Polynomial<coeff_t>();
  coeff_t operator()( coeff_t z )
  {
    coeff_t result = coeff.last();
    std::vector<coeff_t>::reverse_iterator rit = coeff.rbegin();
    rit++;
    for (; rit != this.coeff.end(); ++rit)
      {
        result = result * z + *rit;
      }
    return result;
  }
  coeff_t roots();
  long set_coeffs( std::vector<coeff_t> rhs )
  {
    std::vector<coeff_t>::reverse_iterator rit = rhs.rbegin();   //Reverse iterate to trim any zeroes, i.e. leading coefficent must be nonzero.
    while (*rit == 0 && (rit + 1) != rhs.rend())
    {
      rhs.pop_back()
      rit++;
    }
    coeff = rhs;
  }
  long degree() {return coeff.size()}
  Polynomial<coeff_t>& operator=(const Polynomial<coeff_t> &rhs);
  Polynomial<coeff_t> operator+(const Polynomial<coeff_t> &other);
  Polynomial<coeff_t> operator*(const Polynomial<coeff_t> &other);
  Polynomial<coeff_t> operator-(const Polynomial<coeff_t> &other);
  Polynomial<coeff_t> operator+=(const Polynomial<coeff_t> &other);
  Polynomial<coeff_t> operator*=(const Polynomial<coeff_t> &other);
  //Polynomial<coeff_t> operator*=(const coeff_t &scalar);
  Polynomial<coeff_t> operator-=(const Polynomial<coeff_t> &other);
  //Polynomial<coeff_t> operator*(const coeff_t &scalar);
  
};

template<class coeff_t>
Polynomial<coeff_t>& operator=(const Polynomial<coeff_t> &rhs)
{
  if(this == &rhs)
    return *this;
  coeff.resize(rhs.size());
  set_coeffs(rhs); 
}

// Type-specific stuff: constructors and (some) arithmetic
// Constructors need to be type-specific because there are different types of zeros, and this ain't Julia

Polynomial<double>::Polynomial()
{
  coeff = std::vector<double>{0.};
}

Polynomial<long>::Polynomial()
{
  coeff = std::vector<long>{0};
}

Polynomial<complex<double> >::Polynomial()
{
  coeff = std::vector<complex<double >>{0.};
}

template<class coeff_t>
Polynomial<coeff_t> & Polynomial<coeff_t>::operator+=( const Polynomial<coeff_t> &rhs )
{
  if (rhs.degree() > degree())
  {
    coeff.resize(rhs.degree());
  }
  for (int i = 0; i <= rhs.degree(); i++)
  {
    coeff[i] += rhs.coeff[i];
  }
  return *this;
}

template<class coeff_t>
Polynomial<coeff_t> & Polynomial<coeff_t>::operator-=( const Polynomial<coeff_t> &rhs )
{
if (rhs.degree() > degree())
  { 
    coeff.resize(rhs.degree());
  } 
  for (int i = 0; i <= rhs.degree(); i++)
  {
    coeff[i] -= rhs.coeff[i];
  }  
  return *this;
}

template<class coeff_t>
Polynomial<coeff_t> & Polynomial<coeff_t>::operator*=( const Polynomial<coeff_t> &rhs )
{
  long new_degree = degree() + rhs.degree();
  coeff.resize(new_degree);
  std::vector<coeff_t> tempc(new_degree);
  int i;
  int j;
  for (i = 0; i <= new_degree; i++)
  {
    for (j = 0; j <= i; j++)
    {
      if (j <= degree() && (i - j) <= rhs.degree())
        tempc[i] += coeff[j] * rhs.coeff[i - j];
    }
  }
  set_coeffs(tempc);
  return *this;
}

template<class coeff_t>
Polynomial<coeff_t> Polynomial<coeff_t>::operator+(const Polynomial<coeff_t> &rhs)
{
  Polynomial<coeff_t> result = *this;
  result += rhs;
  return result;
}

template<class coeff_t>
Polynomial<coeff_t> Polynomial<coeff_t>::operator-
{
  Polynomial<coeff_t> result = *this;
  result -= rhs;
  return result;
}

template<class coeff_t>
Polynomial<coeff_t> Polynomial<coeff_t>::operator*
{
  Polynomial<coeff_t> result = *this;
  result *= rhs;
  return result;
}

