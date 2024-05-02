#include "Matrix.hpp"
#include <iostream>
#include <fstream>

using namespace algebra;

int main()
{

    Matrix<double, StorageOrder::ROWMAJOR> test_matrix(4, 3);
    //std::cout<< (test_matrix(1,1) = 2)<<std::endl;
    //test_matrix.print();
    //test_matrix.compress(); //ok 
    //test_matrix(1,1) = 2; //ok
    // test_matrix.compress(); // ok
 
    // test_matrix(1,1) = 2; //ok
    // test_matrix.compress();
    //test_matrix.print(); // ok

    //test_matrix.uncompress(); //ok
    std::string file_path = "./assets/lnsp_131.mtx";
    test_matrix.read_from_file(file_path);  // ok
    //test_matrix.print();
    //test_matrix.compress();
    //test_matrix.print();

    //test vector multiplication 
    std::vector<double> unary_vector(131, 1);

    std::vector<double> result = test_matrix.operator*(unary_vector);

    for (size_t i = 0; i < 131; i++)
    {
        std::cout<<result[i]<<std::endl;
    } 


    //TODO: test with chrono utility and activate optimization in compiler
    
    

    
    

}