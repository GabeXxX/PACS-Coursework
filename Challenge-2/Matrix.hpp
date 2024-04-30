#include <map>
#include <array>
#include <vector>
#include <iostream>

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
        void resize(std::size_t nrows, std::size_t ncolumns)
        {
            n_rows = nrows;
            n_columns = ncolumns;

            if (compressed)
            {
                compressed_data.resize(n_rows * n_columns);
            }
        };

        /*!
         * Method to access element in the matrix
         * @param i Row index
         * @param j Column index
         * @return std::out_of_range if indexes are out of range
         */
        const T &operator()(std::size_t i, std::size_t j, const T &value) const
        {
            if (i >= n_rows || j >= n_columns)
            {
                throw std::out_of_range("Index out of range");
            }
            if (!compressed)
            {
                if (Order == StorageOrder::ROWMAJOR)
                {
                    // TODO: check if using find is efficent in this context
                    auto it = uncompressed_data.find({i, j});
                    return (it != uncompressed_data.end()) ? it->second : 0;
                }
                else
                {
                    auto it = uncompressed_data.find({j, i});
                    return (it != uncompressed_data.end()) ? it->second : 0;
                }
            }
            else
            {
                // TODO: adjust indexing for vector type compressed data
                if (Order == StorageOrder::ROWMAJOR)
                {
                    auto it = compressed_data.find({i, j});
                    return (it != compressed_data.end()) ? it->second : 0;
                }
                else
                {
                    auto it = compressed_data.find({j, i}); // For column-major ordering
                    return (it != compressed_data.end()) ? it->second : 0;
                }
            }
        };

        /*!
         * Method to set/add element in the matrix
         * @param i Row index
         * @param j Column index
         * @return std::out_of_range if indexes are out of range
         */
        T &operator()(std::size_t i, std::size_t j, const T &value)
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
                if (Order == StorageOrder::ROWMAJOR)
                    uncompressed_data[{i, j}] = value;
                else
                    uncompressed_data[{j, i}] = value;
            }
        };

        /*!
         * Compress the matrix storage
         */
        void compress()
        {
            if (!compressed)
            {
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

        /*!
         * Uncompress the matrix storage
         */
        void uncompress()
        {
            if (compressed)
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
                }
                else
                {
                    for (std::size_t j = 0; j < n_columns; ++j)
                    {
                        for (std::size_t k = compressed_inner_indexes[j]; k < compressed_inner_indexes[j + 1]; ++k)
                        {
                            uncompressed_data[{j, compressed_outer_indexes[k]}] = compressed_data[k];
                        }
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

        /*!
         * Check if the matrix is compressed
         */
        bool is_compressed()
        {
            return compressed;
        }

        // Matrix-vector multiplication operator TODO: implement as friend method
        std::vector<T> operator*(const std::vector<T> &v) const
        {
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
                if (Order == StorageOrder::ROWMAJOR)
                {
                    // Row-wise multiplication
                    for (const auto &elem : uncompressed_data)
                    {
                        std::size_t i = elem.first[0]; // Row index
                        std::size_t j = elem.first[1]; // Column index

                        result[i] += elem.second * v[j];
                    }
                }
                else
                {
                    // Column-wise multiplication
                    for (const auto &elem : uncompressed_data)
                    {
                        std::size_t i = elem.first[0]; // Row index
                        std::size_t j = elem.first[1]; // Column index

                        result[j] += elem.second * v[i];
                    }
                }
            }

            return result;

        };
    };
}