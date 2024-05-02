#include <chrono>
#include <iostream>
#include "Matrix.hpp"

namespace algebra
{

    template <typename T, StorageOrder Order>
    double timing_matrix(Matrix<T, Order> test_matrix)
    {
        size_t N = 100;
        std::vector<double> unary_vector(131, 1);

        // Start measuring time
        auto start = std::chrono::high_resolution_clock::now();

        // Block of code to measure    
        for (size_t i = 0; i < N; i++)
        {
            std::vector<double> result = test_matrix.operator*(unary_vector);
        }

        // Stop measuring time
        auto end = std::chrono::high_resolution_clock::now();

        // Calculate duration
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start) / 100;

        // Output the execution time
        std::cout << "Execution time: " << duration.count() << " nanoseconds" << std::endl;

        return duration.count();
    }
}