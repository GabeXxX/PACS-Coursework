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
        std::vector<std::size_t> compressed_inner_indexes;
        std::vector<std::size_t> compressed_outer_indexes;
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
                std::size_t row_start = compressed_inner_indexes[i];
                std::size_t row_end = compressed_inner_indexes[i + 1];

                for (std::size_t k = row_start; k < row_end; ++k)
                {
                    if (compressed_outer_indexes[k] == j)
                    {
                        return compressed_data[k];
                    }
                }

                return sparse_element;
            }
            else
            {
                std::size_t col_start = compressed_inner_indexes[j];
                std::size_t col_end = compressed_inner_indexes[j + 1];

                for (std::size_t k = col_start; k < col_end; ++k)
                {
                    if (compressed_outer_indexes[k] == i)
                    {
                        return compressed_data[k];
                    }
                }

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
            // std::cout<<uncompressed_data.size()<<std::endl;
            compressed_inner_indexes.resize(n_rows + 1);

            if (Order == StorageOrder::ROWMAJOR)
            {
                // Compressed Sparse Row
                for (const auto &elem : uncompressed_data)
                {
                    std::size_t i = elem.first[0];
                    std::size_t j = elem.first[1];

                    compressed_data.push_back(elem.second);
                    compressed_outer_indexes.push_back(j);

                    // Counting the number of non-zero elements encountered in each row of the matrix
                    compressed_inner_indexes[i + 1]++;
                }

                // Accumulates the counts from the previous rows, effectively transforming the counts into the starting indexes for each row in the compressed format
                for (std::size_t i = 1; i <= n_rows; ++i)
                {
                    compressed_inner_indexes[i] += compressed_inner_indexes[i - 1];
                }
            }
            else
            {
                // Compressed Sparse Column
                for (const auto &elem : uncompressed_data)
                {
                    std::size_t i = elem.first[0];
                    std::size_t j = elem.first[1];

                    compressed_data.push_back(elem.second);
                    compressed_outer_indexes.push_back(i);

                    // Counting the number of non-zero elements encountered in each row of the matrix
                    compressed_inner_indexes[j + 1]++;
                }

                for (std::size_t j = 1; j <= n_columns; ++j)
                {
                    compressed_inner_indexes[j] += compressed_inner_indexes[j - 1];
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
                for (std::size_t k = compressed_inner_indexes[i]; k < compressed_inner_indexes[i + 1]; ++k)
                {
                    uncompressed_data[{i, compressed_outer_indexes[k]}] = compressed_data[k];
                }
            }

            compressed = false;
            compressed_data.clear();
            compressed_inner_indexes.clear();
            compressed_outer_indexes.clear();
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
                    for (std::size_t k = compressed_inner_indexes[i]; k < compressed_inner_indexes[i + 1]; ++k)
                    {
                        result[i] += compressed_data[k] * v[compressed_outer_indexes[k]];
                    }
                }
            }
            else
            {
                // Column-wise multiplication (CSC format)
                for (std::size_t j = 0; j < n_columns; ++j)
                {
                    for (std::size_t k = compressed_inner_indexes[j]; k < compressed_inner_indexes[j + 1]; ++k)
                    {
                        result[compressed_outer_indexes[k]] += compressed_data[k] * v[j];
                    }
                }
            }
        }
        else
        {
            // Uncompressed state
            for (const auto &elem : uncompressed_data)
            {
                std::size_t i = elem.first[0]; // Row index
                std::size_t j = elem.first[1]; // Column index

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