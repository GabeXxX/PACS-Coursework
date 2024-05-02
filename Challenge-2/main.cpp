#include "Matrix.hpp"
#include <iostream>
#include <fstream>

using namespace algebra;

int main()
{

    Matrix<double, StorageOrder::COLMAJOR> test_matrix(4, 3);
    //std::cout<< (test_matrix(1,1) = 2)<<std::endl;
    //test_matrix.print();
    //test_matrix.compress(); //ok 
    //test_matrix(1,1) = 2; //ok
    // test_matrix.compress(); // ok
 
    // test_matrix(1,1) = 2; //ok
    // test_matrix.compress();
    //test_matrix.print(); // ok

    //test_matrix.uncompress(); //ok
    //std::string file_path = "./assets/lnsp_131.mtx";
    //test_matrix.read_from_file(file_path);  // ok
    //test_matrix.print();
    //test_matrix.compress();
    //test_matrix.print();

    //TODO: test vector multiplication 
    //std::vector<double> unary_vector(131, 1);

    //std::vector<double> result = test_matrix.operator*(unary_vector);

    /*for (size_t i = 0; i < 131; i++)
    {
        std::cout<<result[i]<<std::endl;
    }*/
    test_matrix(1,0) = 2;
    test_matrix(0,1) = 2;
    test_matrix(3,1) = 2;
    test_matrix.compress(); //ok
    test_matrix.print();

    std::vector<double> unary_vector(3, 1);

    std::vector<double> result = test_matrix.operator*(unary_vector);
    for (size_t i = 0; i < 4; i++)
    {
        std::cout<<result[i]<<std::endl;
    }
    
    

    
    

}