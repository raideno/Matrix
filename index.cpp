#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "lib/sle.hpp"
#include "lib/util.hpp"
#include "lib/matrix.hpp"

int main()
{
    Sle::srand(time(NULL));
    Sle::set_debug_options(SystemDebug::SYSTEM_NONE);

    MatrixClass::srand(time(NULL));
    MatrixClass::set_debug_options(MatrixDebug::MATRIX_NONE);

    srand(time(NULL));

    printf("[start]\n");

    MatrixClass *matrix = MatrixClass::create_matrix_random_int(NORMAL, 3, 3);

    matrix->print();

    matrix->destroy();

    return 0;
}