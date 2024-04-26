#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include "GradientMethodUtils.hpp"
#include "GradientMethod.hpp"

using namespace pacs;

int main()
{
  
  // Define function and gradient
  Function fun = [](Vector x) -> Scalar {return x[0]*x[1] + 4*std::pow(x[0],4) + std::pow(x[1],2) + 3*x[0];};
  
  Gradient funGrad = {
    [](Vector x){return x[1] + 16*std::pow(x[0], 3) + 3;},
    [](Vector x){return x[0] + 2*x[1];} 
                    };

  // Alternatively you can compute the gradient autoatically using finite difference
  Gradient diff = finiteDiff(fun, 2);

  // Define data for the Gradient method
  GradientMethodData data = {
    fun,
    diff,
    {0,0},
    std::pow(10, -6),
    std::pow(10, -6),
    0.1,  
    1000, 
    'a'  // select the update rule for alpha - see GradientMethodData.hpp for detail
  };

  // Compute the minimum using Gradient methid
  Vector min  = gradientMethod(data);

  // Print the minimum
  std::cout<<"MIN:"<<std::endl;
    for(auto i : min){
        std::cout<<i<<std::endl;
  }

}