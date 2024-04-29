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
        bool is_compressed = false;

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

            if (is_compressed)
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
            if constexpr (i >= n_rows || j >= n_columns)
            {
                throw std::out_of_range("Index out of range");
            }
            if (!is_compressed)
            {
                if (Order == StorageOrder::ROWMAJOR)
                {
                    // TODO: check is using find is efficent in this context
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
            if constexpr (i >= n_rows || j >= n_columns)
            {
                throw std::out_of_range("Index out of range");
            }
            if (is_compressed)
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
        void compress() // TODO: delete the uncompressed map
        {
            if (!is_compressed)
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

                    is_compressed = true;
                }
                else{
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

                    // Accumulates the counts from the previous rows, effectively transforming the counts into the starting indexes for each row in the compressed format
                    for (std::size_t j = 1; j <= n_columns; ++j)
                    {
                        compressed_inner_indexes[j] += compressed_inner_indexes[j - 1];
                    }

                    is_compressed = true; 

                }
            }
            else
            {
                std::cout << "Matrix already compressed" << std::endl;
            }
        }
    };
}