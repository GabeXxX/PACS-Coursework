#include "Matrix.hpp"
#include <iostream>
#include <fstream>

using namespace algebra;

int main()
{

    Matrix<double, StorageOrder::ROWMAJOR> test_matrix(4, 3);

    std::vector<double> unary_vector(131, 1);

    std::vector<double> result = test_matrix.operator*(unary_vector);

    for (size_t i = 0; i < 131; i++)
    {
        std::cout<<result[i]<<std::endl;
    } 


    //TODO: test with chrono utility and activate optimization in compiler
    
    

    
    

}