#include <stdio.h>

#include "../matrix/matrix.hpp"

#include "math.hpp"

/*
    matrix_2 have to be equal or less in size than matrix_1
    convolution with a matrix (matrix_2)
    it only work for 3x3 matrices
*/

MatrixClass *convolution(MatrixClass *entries, MatrixClass *kernel)
{
    auto [n_1, m_1] = entries->size();

    return n_1 > 1 ? matrix_convolution(entries, kernel) : vector_convolution(entries, kernel);
}

MatrixClass *matrix_convolution(MatrixClass *matrix, MatrixClass *kernel)
{

    /*make sure those two are matrices*/

    auto [n_1, m_1] = matrix->size();

    MatrixClass *result = NULL;

    if (!kernel->have_center())
        return NULL;

    result = MatrixClass::create_matrix(n_1, m_1);
    kernel = kernel->flip();

    result->map(
        NORMAL, [matrix, kernel](std::size_t i, std::size_t j, float value) -> float
        { return kernel->reduce(
              NORMAL, [matrix, target_i = i, target_j = j](float acumulator, std::size_t i, std::size_t j, float value) -> float
              { return acumulator + (matrix->get(i - 1 + target_i, j - 1 + target_j, 0) * value); },
              0); },
        true);

    kernel->destroy();

    return result;
}

MatrixClass *vector_convolution(MatrixClass *array, MatrixClass *kernel)
{
    auto [n_1, m_1] = array->size();
    auto [n_2, m_2] = kernel->size();

    MatrixClass *result = NULL;

    /*make sure those two are arrays*/

    result = MatrixClass::create_matrix(1, m_1 + m_2 - 1);
    kernel = kernel->flip();

    result->map(
        NORMAL, [array, kernel](std::size_t i, std::size_t j, float value) -> float
        { return kernel->reduce(
              NORMAL, [array, k = j](float acumulator, std::size_t i, std::size_t j, float value) -> float
              { return acumulator + (array->get(0, j - 2 + k, 0) * value); },
              0); },
        true);

    kernel->destroy();

    return result;
}