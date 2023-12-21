#ifndef __MATH_HPP__
#define __MATH_HPP__

#include "../util/util.hpp"
#include "../matrix/matrix.hpp"
#include "../matrix/matrix-value.hpp"

template <typename T, typename = std::enable_if_t<std::is_base_of<MatrixValue, T>::value>>

MatrixClass<T> *convolution(MatrixClass<T> *entries, MatrixClass<T> *kernel);
template <typename T, typename = std::enable_if_t<std::is_base_of<MatrixValue, T>::value>>

MatrixClass<T> *vector_convolution(MatrixClass<T> *array, MatrixClass<T> *kernel);
template <typename T, typename = std::enable_if_t<std::is_base_of<MatrixValue, T>::value>>

MatrixClass<T> *matrix_convolution(MatrixClass<T> *matrix, MatrixClass<T> *kernel);

#endif