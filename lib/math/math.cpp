#include <stdio.h>

#include "../matrix/matrix.hpp"

#include "math.hpp"

/*
    matrix_2 have to be equal or less in size than matrix_1
    convolution with a matrix (matrix_2)
    it only work for 3x3 matrices
*/
MatrixClass *matrix_convolution(MatrixClass *matrix_1, MatrixClass *kernel)
{
    auto [n_1, m_1] = matrix_1->size();

    MatrixClass *result = NULL;

    if (!kernel->have_center())
        return NULL;

    result = MatrixClass::create_matrix(n_1, m_1);
    kernel = kernel->flip();

    result->map(
        NORMAL, [matrix_1, kernel](std::size_t i, std::size_t j, float value) -> float
        { return kernel->reduce(
              NORMAL, [matrix_1, target_i = i, target_j = j](float acumulator, std::size_t i, std::size_t j, float value) -> float
              { return acumulator + (matrix_1->get(i - 1 + target_i, j - 1 + target_j, 0) * value); },
              0); },
        true);

    kernel->destroy();

    return result;
}

MatrixClass *list_convolution(MatrixClass *matrix_1, MatrixClass *kernel)
{
    return NULL;
}