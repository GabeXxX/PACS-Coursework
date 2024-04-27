#include <map>
#include <array>

namespace algebra{

    /*!
    * Specify the matrix ordering
    */
    enum class StorageOrder{
        ROWMAJOR,
        COLMAJOR
    };

    /*!
    *   @tparam T tyep of the element in the matrix
    *   @tparam StorageOrder storage order of the matrix
    */
    template<typename T, StorageOrder Order>
    class Matrix{

        private:
            std::map<std::array<std::size_t, 2>, T> data;
            std::size_t nRows = 0;
            std::size_t nColumns = 0;

        public:

            /*Constructor that takes the size of the matrix
            * @param nRows Number of rows
            * @param nColumns Number of columns
            */
            Matrix(std::size_t nRows, std::size_t nColumns) : nRows{nRows}, nColumns{nColumns} {};
    };
    

}