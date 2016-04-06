template<typename coeff_t>
class Polynomial {
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
};


