# PACS Challange 1
---- 
## Content
This repo contains source code and descriptions for the first challenge of the PACS (Advanced Programming for Scientific Computing) course at Politecnico di Milano.

Code is organized in the following files:
- `GradientMethod.hpp and GradientMethod.cpp` contains the declaration and definition of the actual gradient method for the minimization of the multivariate function.
- `GradientMethodData.hpp` contains the definition of the data structure aggregating all the parameter needed for the gradient method to work.
- `GradientMethodUtils.hpp and GradientMethodUtils.cpp` contains the declaration and definition of some useful types and function utilized by the gradient method. 

## Code
In `GradientMethodUtils.hpp` are defined the following types:

```cpp
using Scalar = double;
using Vector = std::vector<Scalar>;
using Function = std::function<Scalar(Vector)>;
using Gradient = std::vector<Function>;
```

In particular multivariate polynomial functions are represented by the `Function` type (thus using a `std::vector<double>` for representing the variables). Gradient of such functions are stored as a vector of `Function`. 

`GradientMethodUtils` contains also the function
`Gradient finiteDiff(Function f, unsigned size);` to compute the Gradient with a finite difference scheme in the case we do not want to input the derivative by hand. 

Using *function wrapper* allow to interchange the way in which we input derivative without altering the underlying code for the function optimization

```cpp
// Define function and gradient

Function fun = [](Vector x) -> Scalar {return x[0]*x[1] + 4*std::pow(x[0],4) + std::pow(x[1],2) + 3*x[0];};

Gradient funGrad = {
	[](Vector x){return x[1] + 16*std::pow(x[0], 3) + 3;},
	[](Vector x){return x[0] + 2*x[1];}
};

// Alternatively you can compute the gradient autoatically using finite difference
Gradient diff = finiteDiff(fun, 2);

// Define data for the Gradient method
GradientMethodData data = {
	fun,
	diff, // or funGrad
    ...
};
```

`GradientMethod` contains the actual gradient optimization code. To allow for the selection of different strategies for the computation of $\alpha_k$ I exploit again *function wrapper*: `selectStrategy` select at runtime the right strategy inputted by the user prior of entering in the iterative part of the optimization method:

```cpp
Vector gradientMethod(GradientMethodData data){
	...
	std::function<Scalar(Vector, GradientMethodData, unsigned iter)> updateAlpha = selectStrategy(data.strategy);

	if(updateAlpha != nullptr){
		...
		while (verifyCondition(oldX, newX, data) && iter < data.maxIt)
		{
			...
			Scalar newAlpha = updateAlpha(oldX, data, iter);
			...
		
```