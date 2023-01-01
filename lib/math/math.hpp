#ifndef __MATH_HPP__
#define __MATH_HPP__

#include "../util/util.hpp"
#include "../matrix/matrix.hpp"

MatrixClass *convolution(MatrixClass *entries, MatrixClass *kernel);
MatrixClass *vector_convolution(MatrixClass *array, MatrixClass *kernel);
MatrixClass *matrix_convolution(MatrixClass *matrix, MatrixClass *kernel);

#endif