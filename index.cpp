#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "lib/sle/sle.hpp"
#include "lib/util/util.hpp"
#include "lib/math/math.hpp"
#include "lib/matrix/matrix.hpp"

float arr_1[] = {1, 2, 3};
float arr_2[] = {4, 5, 6};

int main()
{
    printf("[start]\n");

    MatrixClass *array_1 = MatrixClass::matrix_from_array(NORMAL, arr_1, 1, 3);
    MatrixClass *array_2 = MatrixClass::matrix_from_array(NORMAL, arr_2, 1, 3);
    MatrixClass *result = convolution(array_1, array_2)->set_name("convolution");

    array_1->print();
    array_2->print();

    result->print();

    array_1->destroy();
    array_2->destroy();
    result->destroy();

    printf("[end]\n");

    return 0;
}