# Challenge 2
# PACS Challange 2
## Content
This repo contains source code and descriptions for the second challenge of the PACS (Advanced Programming for Scientific Computing) course at Politecnico di Milano.

Code is organized in the following files:
- `Matrix.hpp` contains the declaration and definition of the class implementing the matrix function.  

- `Test.hpp` contains the declaration and definition of the code used to test and chrono the matrix implementation.  

- `assets` folder contain the matrix used for testing  


## Installation
Go to the project directory. Type
```
make
```

## Documentation
The Doxygen generated documentation is available at the following [link](https://gabexxx.github.io/PACS-Coursework/html/classalgebra_1_1_matrix.html).

### A note on the matrix-vector product
Although it is not documented in the code so as not to fill it with comments, the algorithm used to perform the product between matrix and vector is explained in this [link](https://www.netlib.org/utk/people/JackDongarra/etemplates/node382.html).

## Performance
The performance of the matrix-vector product using both uncompressed and compressed representations was measured using the `Chrono` utility, with both rowmajor and columnmajor sorting. 100 trials were performed for each of the possible 4 cases. Below are the average execution times for each case

```
Matrix Row-major not compressed:
Average execution time: 3617 nanoseconds

Matrix Column-major not compressed:
Average execution time: 3627 nanoseconds

Matrix Row-major compressed:
Average execution time: 1089 nanoseconds

Matrix Column-major compressed:
Average execution time: 1100 nanoseconds
```

