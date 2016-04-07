template<typename coeff_t>
class Polynomial<coeff_t> {
  private:
  std::vector<coeff_t> coefficients;
  public:
  coeff_t operator()( coeff_t z )
  {
    coeff_t result = this.coeff.last();
    std::vector<coeff_t>::reverse_iterator rit;
    rit++;
    for (; rit != this.coefficients.end(); ++rit)
      {
        result = result * z + *rit;
      }
    return result;
  }
  coeff_t roots();
  int set_coeffs( std::vector<coeff_t> coef );
  Polynomial<coeff_t>& operator=(const Polynomial<coeff_t> &rhs);
  Polynomial<coeff_t> operator+(const Polynomial<coeff_t> &p1, const Polynomial<coeff_t> &p2);
  Polynomial<coeff_t> operator*(const Polynomial<coeff_t> &p1, const Polynomial<coeff_t> &p2);
  Polynomial<coeff_t> operator*(coeff_t scalar, const Polynomial<coeff_t> &p);
};


