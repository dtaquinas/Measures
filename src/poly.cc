#include <iostream>
#include <complex>
#include <vector>


// Important note: coefficients go in order of increasing degree: the first element of coeff is the constant, etc.
template<typename coeff_t>
class Polynomial {
  private:
  std::vector<coeff_t> coeff;
  public:
  Polynomial();
  coeff_t operator()( coeff_t z ) 
  {
    coeff_t result = coeff.back();
    typename std::vector<coeff_t>::reverse_iterator rit = coeff.rbegin();
    rit++;
    for (; rit != coeff.rend(); ++rit)
      {
        result = result * z + *rit;
      }
    return result;
  }
  //coeff_t roots();
  int set_coeffs( std::vector<coeff_t> rhs )
  {
    typename std::vector<coeff_t>::reverse_iterator rit = rhs.rbegin();   //Reverse iterate to trim any zeroes, i.e. leading coefficent must be nonzero.
    while (*rit == 0 && (rit + 1) != rhs.rend())
    {
      rhs.pop_back();
      rit++;
    }
    coeff = rhs;
    return 0;
  }
  int degree() const {return coeff.size() - 1;}

  // OVERLOAD ALL THE OPERATORS

  Polynomial<coeff_t>& operator=(const Polynomial<coeff_t> &rhs)
  {
    if(this == &rhs)
      return *this;
    //int newsize;
    //newsize = rhs.degree() + 1;
    //coeff.resize(newsize);
    set_coeffs(rhs.coeff);
  }

  Polynomial<coeff_t> & operator+=( const Polynomial<coeff_t> &rhs )
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

  Polynomial<coeff_t> & operator-=( const Polynomial<coeff_t> &rhs )
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

  Polynomial<coeff_t> & operator*=( const Polynomial<coeff_t> &rhs )
  {
    int new_degree = degree() + rhs.degree();
    std::vector<coeff_t> tempc(new_degree + 1);
    int i;
    int j;
    for (i = 0; i <= new_degree + 1; i++)
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


  Polynomial<coeff_t> & operator*=( const coeff_t &rhs )
  {
  std::vector<coeff_t> result = coeff;
    typename std::vector<coeff_t>::reverse_iterator rit = coeff.rbegin();
    rit++;
    for (; rit != coeff.rend(); ++rit)
      {
        result = result * rhs;
      }
    set_coeffs(result);  
  }

  Polynomial<coeff_t> operator+(const Polynomial<coeff_t> &rhs)
  {
    Polynomial<coeff_t> result = *this;
    result += rhs;
    return result;
  }

  Polynomial<coeff_t> operator-(const Polynomial<coeff_t> &rhs)
  {
    Polynomial<coeff_t> result = *this;
    result -= rhs;
    return result;
  }

  Polynomial<coeff_t> operator*(const Polynomial<coeff_t> &rhs)
  {
    Polynomial<coeff_t> result = *this;
    result *= rhs;
    return result;
  }

  Polynomial<coeff_t> operator*(const coeff_t &rhs)
  {
    Polynomial<coeff_t> result = *this;
    result *= rhs;
    return result;
  }


};

template<>
Polynomial<double>::Polynomial()
{
  coeff = std::vector<double>{0.};
}

template<>
Polynomial<complex>::Polynomial()
{
  coeff = std::vector<complex>{0.};
}

template<>
Polynomial<complex> conj(const Polynomial<complex> &p)
{
  Polynomial<complex> q;
  std::vector<complex> qc;
  typename std::vector<complex>::iterator it = p.coeff.begin();
  for (; it != coeff.end(); ++it)
  {
    qc.push_back(*it);
  }
  return q;
}

