#include "Test.hpp"

using namespace algebra;

int main()
{
    // Creating the matrix by row and column
    std::string file_path = "./assets/lnsp_131.mtx";
    Matrix<double, StorageOrder::ROWMAJOR> test_matrix_row(1, 1);
    test_matrix_row.read_from_file(file_path);
    Matrix<double, StorageOrder::COLMAJOR> test_matrix_col(1, 1);
    test_matrix_col.read_from_file(file_path);

    // Timiming the matrix vector multiplication for non compressed matrix
    std::cout << "Matrix Row-major not compressed:"<<std::endl;
    timing_matrix(test_matrix_row);
    std::cout << "Matrix Column-major not compressed:"<<std::endl;
    timing_matrix(test_matrix_col);

    // Timiming the matrix vector multiplication for compressed matrix
    test_matrix_row.compress();
    test_matrix_col.compress();
    std::cout << "Matrix Row-major compressed:"<<std::endl;
    timing_matrix(test_matrix_col);
    std::cout << "Matrix Column-major compressed:"<<std::endl;
    timing_matrix(test_matrix_col);
}