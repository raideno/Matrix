#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "lib/sle/sle.hpp"
#include "lib/util/util.hpp"
#include "lib/math/math.hpp"
#include "lib/matrix/matrix.hpp"

float array_1[] = {1, 2, 3, 4, 5, 6};
float array_2[] = {0, 0, 0, 0, 0, 0, 0, 1, 0};

int main()
{

    printf("[start]\n");

    MatrixClass *matrix_1 = MatrixClass::matrix_from_array(NORMAL, array_1, 2, 3);
    MatrixClass *matrix_2 = MatrixClass::matrix_from_array(NORMAL, array_2, 3, 3);

    MatrixClass *result = matrix_convolution(matrix_1, matrix_2)->set_name("convolution");

    matrix_1->print();
    matrix_2->print();

    result->print();

    matrix_1->destroy();
    matrix_2->destroy();
    result->destroy();

    printf("[end]\n");

    return 0;
}