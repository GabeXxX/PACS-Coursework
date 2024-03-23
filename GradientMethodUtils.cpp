#include "GradientMethodUtils.hpp"

namespace pacs{

    Scalar euclideanNorm(const Vector &v){
        Scalar sum = 0;
        for(auto x : v){
            sum += std::pow(x, 2);
        }

        return std::sqrt(sum);
    };

    Vector scalarGradProd(const Scalar &s, const Gradient &v,const Vector &x)
    {
        Vector newVec(v.size());

        for(int i = 0; i < v.size(); ++i){
            newVec[i] = s*v[i]({x});
        }
        

        return newVec;
    };


    Vector subVector(const Vector &v1, const Vector &v2){
        Vector newVec(v1.size());

        for(int i = 0; i < v1.size(); ++i){
            newVec[i] = v1[i] - v2[i];
        }
        

        return newVec;

    };

    Gradient finiteDiff(Function f, unsigned size) {
        Gradient grad;

        const double h = 1e-5;

        for (unsigned i = 0; i < size; ++i) {

            Function partial_derivative = [f, h, i](Vector x) -> Scalar {
                Vector x_plus = x;
                x_plus[i] += h;
                return (f(x_plus) - f(x)) / (h);
            };

            grad.push_back(partial_derivative);
        }

        return grad;
    }
}