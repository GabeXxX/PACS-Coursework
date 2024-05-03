#include <map>
#include <array>
#include <vector>
#include <iostream>
#include <fstream>

namespace algebra
{

    /*!
     * Specify the matrix ordering
     */
    enum class StorageOrder
    {
        ROWMAJOR,
        COLMAJOR
    };

    /*!
     *   @tparam T tyep of the element in the matrix
     *   @tparam StorageOrder storage order of the matrix
     */
    template <typename T, StorageOrder Order>
    class Matrix
    {

    private:
        std::map<std::array<std::size_t, 2>, T> uncompressed_data;
        std::vector<T> compressed_data;
        std::vector<std::size_t> offsets_vector;
        std::vector<std::size_t> indices_vector;
        std::size_t n_rows = 0;
        std::size_t n_columns = 0;
        bool compressed = false;
        T sparse_element = 0;

    public:
        /*!
         * Constructor that takes the size of the matrix
         * @param nrows Number of rows
         * @param ncolumns Number of columns
         */
        Matrix(std::size_t nrows, std::size_t ncolumns) : n_rows{nrows}, n_columns{ncolumns} {};

        /*!
         * Method to resize the matrix
         * @param nrows Number of rows
         * @param ncolumns Number of columns
         */
        void resize(std::size_t nrows, std::size_t ncolumns);

        /*!
         * Method to access element in the matrix
         * @param i Row index
         * @param j Column index
         * @return std::out_of_range if indexes are out of range
         */
        const T &operator()(std::size_t i, std::size_t j) const;

        /*!
         * Method to set/add element in the matrix
         * @param i Row index
         * @param j Column index
         * @return std::out_of_range if indexes are out of range
         */
        T &operator()(std::size_t i, std::size_t j);

        /*!
         * Compress the matrix storage
         */
        void compress();

        /*!
         * Uncompress the matrix storage
         */
        void uncompress();

        /*!
         * Check if the matrix is compressed
         */
        bool is_compressed()
        {
            return compressed;
        }

        /*!
         * Matrix-vector multiplication operatoration
         * @param v vector to permform the matrix-vector moltiplication
         * @return a vector containing the result of the operation
         */
        std::vector<T> operator*(const std::vector<T> &v) const;

        /*!
         * Print the matrix
         */
        void print() const
        {
            for (std::size_t i = 0; i < n_rows; ++i)
            {
                for (std::size_t j = 0; j < n_columns; ++j)
                {
                    std::cout << operator()(i, j) << " ";
                }

                std::cout << "\n";
            }
        }

        /*!
         * Read matrix a file formatted in matrix market format
         */
        void read_from_file(std::string &file_path);
    };

    /*
     * ***************************************************************************
     * Definitions
     * ***************************************************************************
     */
    template <typename T, StorageOrder Order>
    void Matrix<T, Order>::resize(std::size_t nrows, std::size_t ncolumns)
    {
        n_rows = nrows;
        n_columns = ncolumns;
    };

    template <typename T, StorageOrder Order>
    const T &Matrix<T, Order>::operator()(std::size_t i, std::size_t j) const
    {
        if (i >= n_rows || j >= n_columns)
        {
            throw std::out_of_range("Index out of range");
        }
        if (!compressed)
        {
            auto it = uncompressed_data.find({i, j});
            return (it != uncompressed_data.end()) ? it->second : sparse_element;
        }
        else
        {
            if (Order == StorageOrder::ROWMAJOR)
            {
                // Localizing the row
                std::size_t row_start = offsets_vector[i];
                std::size_t row_end = offsets_vector[i + 1];

                // Scan the row's columns 
                for (std::size_t k = row_start; k < row_end; ++k)
                {   
                    // Return the element if the right index is found     
                    if (indices_vector[k] == j)
                    {
                        return compressed_data[k];
                    }
                }

                // return 0 if the element is not found
                return sparse_element;
            }
            else
            {
                // Localizing the column 
                std::size_t col_start = offsets_vector[j];
                std::size_t col_end = offsets_vector[j + 1];

                // Scan the column's rows 
                for (std::size_t k = col_start; k < col_end; ++k)
                {
                    // Return the element if the right index is found 
                    if (indices_vector[k] == i)
                    {
                        return compressed_data[k];
                    }
                }

                // return 0 if the element is not found
                return sparse_element;
            }
        }
    };

    template <typename T, StorageOrder Order>
    T &Matrix<T, Order>::operator()(std::size_t i, std::size_t j)
    {
        if (i >= n_rows || j >= n_columns)
        {
            throw std::out_of_range("Index out of range");
        }
        if (compressed)
        {
            throw std::runtime_error("Cannot insert elements in compressed state");
        }
        else
        {
            return uncompressed_data[{i, j}];
        }
    };

    template <typename T, StorageOrder Order>
    void Matrix<T, Order>::compress()
    {
        if (!compressed)
        {
            offsets_vector.resize(n_rows + 1);

            if (Order == StorageOrder::ROWMAJOR)
            {
                // Compressed Sparse Row
                for (const auto &elem : uncompressed_data)
                {
                    std::size_t i = elem.first[0];
                    std::size_t j = elem.first[1];

                    // Populate data vector
                    compressed_data.push_back(elem.second);

                    // Populate column indices vector
                    indices_vector.push_back(j);

                    // Counting the number of non-zero elements encountered in each row of the matrix
                    offsets_vector[i + 1]++;
                }

                // Accumulates the counts from the previous rows, effectively transforming the counts into the starting indexes for each row in the compressed format
                for (std::size_t i = 1; i <= n_rows; ++i)
                {
                    // Populating starting row offset vector
                    offsets_vector[i] += offsets_vector[i - 1];
                }
            }
            else
            {
                // Compressed Sparse Column
                for (const auto &elem : uncompressed_data)
                {
                    std::size_t i = elem.first[0];
                    std::size_t j = elem.first[1];

                    // Populate data vector
                    compressed_data.push_back(elem.second);

                    // Populate row indices vector
                    indices_vector.push_back(i);

                    // Counting the number of non-zero elements encountered in each column of the matrix
                    offsets_vector[j + 1]++;
                }

                // Accumulates the counts from the previous columns, effectively transforming the counts into the starting indexes for each column in the compressed format
                for (std::size_t j = 1; j <= n_columns; ++j)
                {
                    // Populating starting column offset vector
                    offsets_vector[j] += offsets_vector[j - 1];
                }
            }
            compressed = true;
            uncompressed_data.clear();
        }
        else
        {
            std::cout << "Matrix already compressed" << std::endl;
        }
    }

    template <typename T, StorageOrder Order>
    void Matrix<T, Order>::uncompress()
    {

        if (Order == StorageOrder::ROWMAJOR)
        {
            for (std::size_t i = 0; i < n_rows; ++i)
            {
                for (std::size_t k = offsets_vector[i]; k < offsets_vector[i + 1]; ++k)
                {
                    uncompressed_data[{i, indices_vector[k]}] = compressed_data[k];
                }
            }

            compressed = false;
            compressed_data.clear();
            offsets_vector.clear();
            indices_vector.clear();
        }
        else
        {
            std::cout << "Matrix is not compressed" << std::endl;
        }
    }

    template <typename T, StorageOrder Order>
    std::vector<T> Matrix<T, Order>::operator*(const std::vector<T> &v) const
    {
        if (v.size() != n_columns)
        {
            throw std::runtime_error("Non comforming size for the input vector");
        }

        std::vector<T> result(n_rows, 0); // Initialize result vector

        if (compressed)
        {
            // Compressed state
            if (Order == StorageOrder::ROWMAJOR)
            {
                // Row-wise multiplication (CSR format)
                for (std::size_t i = 0; i < n_rows; ++i)
                {
                    for (std::size_t k = offsets_vector[i]; k < offsets_vector[i + 1]; ++k)
                    {
                        result[i] += compressed_data[k] * v[indices_vector[k]];
                    }
                }
            }
            else
            {
                // Column-wise multiplication (CSC format)
                for (std::size_t j = 0; j < n_columns; ++j)
                {
                    for (std::size_t k = offsets_vector[j]; k < offsets_vector[j + 1]; ++k)
                    {
                        result[indices_vector[k]] += compressed_data[k] * v[j];
                    }
                }
            }
        }
        else
        {
            // Uncompressed state
            for (const auto &elem : uncompressed_data)
            {
                std::size_t i = elem.first[0];
                std::size_t j = elem.first[1];

                result[i] += elem.second * v[j];
            }
        }

        return result;
    };

    template <typename T, StorageOrder Order>
    void Matrix<T, Order>::read_from_file(std::string &file_path)
    {
        std::fstream myfile(file_path);
        std::size_t i, j, n_lines;
        double value;

        if (myfile.is_open())
        {
            // Ignore comments headers
            while (myfile.peek() == '%')
            {
                myfile.ignore(2048, '\n');
            }

            // Read number of rows and columns
            myfile >> n_rows >> n_columns >> n_lines;
            resize(n_rows, n_columns);

            // fill the matrix with data
            for (int l = 0; l < n_lines; l++)
            {
                myfile >> i >> j >> value;
                this->operator()(i - 1, j - 1) = value;
            }
        }

        myfile.close();
    };
}