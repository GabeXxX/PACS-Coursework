#include "GradientMethodUtils.hpp"
#include "GradientMethodData.hpp"

namespace pacs{
    Vector gradientMethod(const GradientMethodData & data);

    // Select the strategy to compute the alpha parameter in the gradient method using function wrapper
    // The selection is made at runtime but before the enter of the for loop in the gradientMethod function
    std::function<Scalar(Vector, GradientMethodData, unsigned iter)> selectStrategy(const char &strategy);

    // control of the step length and on the residual for the Gradient method
    bool verifyCondition(const Vector & oldX, const Vector & newX, const GradientMethodData & data);

    // Different strategy for the computation of the alpha parameter
    Scalar armijoRule(const Vector & oldX, const GradientMethodData & data,const unsigned & iter);

    Scalar exponentialRule(const Vector & oldX, const GradientMethodData & data,const unsigned & iter);

    Scalar inverseRule(const Vector & oldX, const GradientMethodData & data,const unsigned & iter);

}
