#include <map>
#include <array>
#include <vector>

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
        std::vector<std::size_t> compressed_row_indexes;
        std::vector<std::size_t> compressed_column_indexes;
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

            if constexpr (is_compressed)
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
                if (order_ == StorageOrder::RowMajor)
                {
                    // TODO: check is using find is efficent in this context
                    auto it = uncompressed_data.find({i, j});
                    return (it != uncompressed_data.end()) ? it->second : 0;
                }
                else
                {
                    auto it = uncompressed_data.find({j, i}); // For column-major ordering
                    return (it != uncompressed_data.end()) ? it->second : 0;
                }
            }
            else
            {
                // Compressed matrix
                // TODO: adjust indexing for vector type compressed data
                if (order_ == StorageOrder::RowMajor)
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
                if (order == StorageOrder::ROWMAJOR)
                    data_[{i, j}] = value;
                else
                    data_[{j, i}] = value; // For column-major ordering
            }
        };

        /*!
         * Compress the matrix storage
         */
        void compress()
        {
            if (!compressed)
            {
                if (order_ == StorageOrder::RowMajor)
                {
                    if (order_ == StorageOrder::RowMajor)
                    {
                        // Initialize vectors for CSR format
                        std::vector<T> values;
                        std::vector<std::size_t> inner_indexes(n_rows + 1, 0);
                        std::vector<std::size_t> outer_indices(n_columns, 0);

                        // Traverse the matrix data and populate vectors
                        for (const auto &elem : data_)
                        {
                            std::size_t i = elem.first[0]; // Row index
                            std::size_t j = elem.first[1]; // Column index

                            // Store value and column index
                            values.push_back(elem.second);
                            outer_indices.push_back(j);

                            // Increment inner index for the current row
                            inner_indexes[i + 1]++;
                        }

                        // Cumulative sum to obtain inner indexes
                        for (std::size_t i = 1; i <= n_rows; ++i)
                        {
                            inner_indexes[i] += inner_indexes[i - 1];
                        }

                        // Update internal storage
                        compressed_data = std::make_tuple(values, inner_indexes, outer_indices);
                    }
                    else
                    {
                        // Compress to CSC format
                        // Implementation...
                    }
                    compressed = true;
                }
            }
        };
    }