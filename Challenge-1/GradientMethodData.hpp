#include "GradientMethodUtils.hpp"

namespace pacs{
    struct GradientMethodData
    {
        Function f;
        Gradient grad;
        Vector xInit;
        Scalar epsS;
        Scalar epsR;
        Scalar alpha0;
        unsigned maxIt;
        char strategy; //'a' - Armijo rule, 'e' - Exponential Decay, 'i' - Inverse Decay
    };
    
}