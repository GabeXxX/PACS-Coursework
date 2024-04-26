#include <functional>

// Define useful type and function for the Gradient Method

namespace pacs{
    using Scalar = double;
    using Vector = std::vector<Scalar>;
    using Function = std::function<Scalar(Vector)>;
    using Gradient = std::vector<Function>;

    // Compute the Euclidean norm of a vector
    Scalar euclideanNorm(const Vector &v);

    // Perform the operation alpha * grad(f(x)) where alpha is a Scalar and grad(f(x)) is a vector
    Vector scalarGradProd(const Scalar &s, const Gradient &v,const Vector &x);

    // Perform the subtraction between two vector
    Vector subVector(const Vector &v1, const Vector &v2);

    // compute a Gradient vector using finite difference
    Gradient finiteDiff(Function f, unsigned size);
    
}