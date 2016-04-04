// Measures for orthogonal polynomials on the real line and the unit circle
// 


class MeasureRL
{

  public:
  double integrate()

};


class MeasureUC
{

  public:
  double integrate()

};

class PolynomialRL
{
  private:
  MeasureRL mu;

  public:
  complex eval(complex z);
  double norm();
  
};

class PolynomialUC
{
  private:
  MeasureUC mu;
  
  public:
  complex eval(complex z);
  double norm();

};


