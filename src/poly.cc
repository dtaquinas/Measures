
template<typename coeff_t>
class Polynomial {
  private:
  std::vector<coeff_t> coefficients;
  public:
  coeff_t eval( coeff_t );
  coeff_t roots();
  
};




