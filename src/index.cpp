#include <locale.h>

#include <time.h>
#include <stdio.h>
#include <cstdlib>
#include <stdlib.h>
#include <iostream>
#include <inttypes.h>

#include "lib/pl/pl.cpp"
#include "lib/matrix/matrix.cpp"

#include "lib/matrix/matrix-int-value.hpp"
#include "lib/matrix/matrix-double-value.hpp"

#define THICKNESS 1

#define NUMBER_OF_CONSTRAINTS 3
#define NUMBER_OF_VARIABLES 2

int main()
{
    setlocale(LC_CTYPE, "");

    printf("[program]: start\n");

    unsigned int seed = time(NULL);
    // unsigned int seed = 1703345585;
    // unsigned int seed = 1703272015;
    // unsigned int seed = 1703325252;

    printf("[program]: seed = %d\n", seed);

    // TODO: create same function in PL and SLE classes that'll make a call to the MatrixClass::srand function at their turn
    MatrixClass<MatrixDoubleValue>::srand(seed);

    MatrixClass<MatrixDoubleValue> *matrix = MatrixClass<MatrixDoubleValue>::create_matrix_random(MatrixType::NORMAL, 2, 2);

    matrix->print();

    printf("[matrix-det]: ");
    matrix->determinent().print();
    printf("\n");

    matrix->inverse()->print()->destroy();

    // matrix->find_eigen_stuff();

    MatrixClass<MatrixDoubleValue>::print_debug_informations();

    matrix->destroy();

    printf("[program]: end\n");

    return EXIT_SUCCESS;
}