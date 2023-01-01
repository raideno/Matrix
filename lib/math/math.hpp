#ifndef __MATH_HPP__
#define __MATH_HPP__

#include "../util/util.hpp"
#include "../matrix/matrix.hpp"

MatrixClass *convolution(MatrixClass *matrix_1, MatrixClass *kernel);
MatrixClass *list_convolution(MatrixClass *matrix_1, MatrixClass *kernel);
MatrixClass *matrix_convolution(MatrixClass *matrix_1, MatrixClass *kernel);

#endif