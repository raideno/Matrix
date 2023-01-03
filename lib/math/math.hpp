#ifndef __MATH_HPP__
#define __MATH_HPP__

#include "../util/util.hpp"
#include "../matrix/matrix.hpp"

template <typename T>
MatrixClass<T> *convolution(MatrixClass<T> *entries, MatrixClass<T> *kernel);
template <typename T>
MatrixClass<T> *vector_convolution(MatrixClass<T> *array, MatrixClass<T> *kernel);
template <typename T>
MatrixClass<T> *matrix_convolution(MatrixClass<T> *matrix, MatrixClass<T> *kernel);

#endif