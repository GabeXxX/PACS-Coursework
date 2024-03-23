#include "GradientMethod.hpp"
#include "GradientMethodUtils.hpp"
#include <iostream>

namespace pacs{

    bool verifyCondition(const Vector & oldX, const Vector & newX, const GradientMethodData & data)
    {
        Vector tmp = subVector(newX, oldX);
        if(euclideanNorm(tmp) < data.epsS){
            return false;
        }
        if(std::abs(data.f({newX}) - data.f({oldX})) < data.epsR){
            return false;
        }
        
        return true;
    };

    Scalar armijoRule(const Vector & oldX, const GradientMethodData & data,const unsigned & iter){

        Scalar delta = 0.05;
        Function f = data.f;
        Scalar alpha0 = data.alpha0;

        Vector tmp0 = scalarGradProd(alpha0, data.grad, oldX);
        Vector tmp = subVector(oldX, tmp0);

        Scalar one = 1.0;
        Vector gradEval = scalarGradProd(one, data.grad, oldX);

        Scalar norm = euclideanNorm(gradEval);
        Scalar cond = delta*alpha0*std::pow(norm, 2);
        
        while ( (f({oldX}) - f({tmp})) < cond)
        {
            alpha0 = alpha0/2;

            tmp0 = scalarGradProd(alpha0, data.grad, oldX);
            tmp = subVector(oldX, tmp0);

            gradEval = scalarGradProd(one, data.grad, oldX);
            norm = euclideanNorm(gradEval);
            cond = delta*alpha0*std::pow(norm, 2);

        }

        return alpha0;
    }

    Scalar exponentialRule(const Vector & oldX, const GradientMethodData & data,const unsigned & iter)
    {
        Scalar mu = 5;
        Scalar alpha = data.alpha0*std::exp(-(mu*iter));
        return alpha;
    }

    Scalar inverseRule(const Vector & oldX, const GradientMethodData & data,const unsigned & iter)
    {
        Scalar mu = 0.01;
        Scalar alpha = data.alpha0/(1 + mu*iter);
        return alpha;
    }

    std::function<Scalar(Vector, GradientMethodData, unsigned iter)> selectStrategy(const char &strategy){
        
        std::function<Scalar(Vector, GradientMethodData, unsigned iter)> updateAlpha;
        
        switch (strategy)
        {
        case 'a':
            std::cout<<"Armijo rule selected"<<std::endl;
            updateAlpha = armijoRule;
            break;
        case 'e':
            std::cout<<"Exponential rule selected"<<std::endl;
            updateAlpha = exponentialRule;
            break;
        case 'i':
            std::cout<<"Inverse rule selected"<<std::endl;
            updateAlpha = inverseRule;
            break;
        
        default:
            std::cout<<"Error: select the correct rule"<<std::endl;
            updateAlpha = nullptr;
            break;
        }

        return updateAlpha;
    }

    Vector gradientMethod(const GradientMethodData & data) {
        Vector oldX = data.xInit;
        Vector tmp = scalarGradProd(data.alpha0, data.grad, oldX);
        Vector newX = subVector(oldX, tmp);

        std::function<Scalar(Vector, GradientMethodData, unsigned iter)> updateAlpha = selectStrategy(data.strategy);
        
        if(updateAlpha != nullptr){
            
            unsigned iter = 0;

            while (verifyCondition(oldX, newX, data) && iter < data.maxIt)
            {   
                oldX = newX;
                Scalar newAlpha = updateAlpha(oldX, data, iter);    
                Vector tmp = scalarGradProd(newAlpha, data.grad, oldX);
                newX = subVector(oldX, tmp);

                std::cout<<newAlpha<<std::endl;

                ++iter;
            }   
        }

        return newX;
              
    }

}