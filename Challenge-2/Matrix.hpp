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

    public:
        /*!
         * Constructor that takes the size of the matrix
         * @param nrows Number of rows
         * @param ncolumns Number of columns
         */
        Matrix(std::size_t nrows, std::size_t ncolumns) : n_rows{nrows}, n_columns{ncolumns}
        {
            compressed_data.resize(n_rows * n_columns);
        };

        /*!
         * Method to resize the matrix
         * @param nrows Number of rows
         * @param ncolumns Number of columns
         */
        void resize(std::size_t nrows, std::size_t ncolumns)
        {
            n_rows = nrows;
            n_columns = ncolumns;
            compressed_data.resize(n_rows * n_columns);
        };

        /*!
         * Method to access element in the matrix
         * @param i Row index
         * @param j Column index
         * @return std::out_of_range if the element is not present or indexes are out of range
         */
        T& operator()(std::size_t i, std::size_t j, const T& value) const
        {
            if constexpr(i >= n_rows || j >= n_columns){
                throw std::out_of_range("Index out of range");
            }
            if constexpr (Order == StorageOrder::ROWMAJOR)
            {
                auto it = uncompressed_data.find({i, j});
                if (it != uncompressed_data.end())
                {
                    return it->second;
                }
                else
                {
                    throw std::out_of_range("Element not found");
                }
            }
            else
            {
                auto it = uncompressed_data.find({j, i}); // For column-major ordering
                if (it != uncompressed_data.end())
                {
                    return it->second;
                }
                else
                {
                    throw std::out_of_range("Element not found");
                }
            }
        };

        /*!
         * Method to insert element in  the matrix
         * @param i Row index
         * @param j Column index
         * For now if an element is already present is simply overwritten
         */
        void operator()(std::size_t i, std::size_t j, const T& value)
        {
            if constexpr(i >= n_rows || j >= n_columns){
                throw std::out_of_range("Index out of range");
            }
            if constexpr (Order == StorageOrder::ROWMAJOR)
            {
                uncompressed_data[{i, j}] = value;
            }
            else
            {
                uncompressed_data[{j, i}] = value;
            }
        }
    };

}