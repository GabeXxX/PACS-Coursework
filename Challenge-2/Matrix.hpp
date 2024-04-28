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
        T &operator()(std::size_t i, std::size_t j, const T &value) const
        {
            if constexpr (i >= n_rows || j >= n_columns)
            {
                throw std::out_of_range("Index out of range");
            }
            if (!is_compressed)
            {
                if (order_ == StorageOrder::RowMajor)
                {   
                    //TODO: check is using find is efficent in this context
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
                //TODO: adjust indexing for vector type compressed data
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
        void compress() {
            if (!compressed) {
                if (order_ == StorageOrder::RowMajor) {
                    // Compress to CSR format
                    // Implementation...
                } else {
                    // Compress to CSC format
                    // Implementation...
                }
                compressed = true;
            }
        }
    };

}