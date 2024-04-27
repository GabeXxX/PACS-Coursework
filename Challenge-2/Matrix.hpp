#include <map>
#include <array>
#include <vector>

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
            Matrix(std::size_t nrows, std::size_t ncolumns) : n_rows{nrows}, n_columns{ncolumns} {
                compressed_data.resize(n_rows*n_columns);
            };

            /*!
            * Method to resize the matrix
            * @param nrows Number of rows
            * @param ncolumns Number of columns
            */
             void resize(std::size_t nrows, std::size_t ncolumns){
                n_rows = nrows;
                n_columns = ncolumns;
                compressed_data.resize(n_rows*n_columns);
            }


    };
    

}