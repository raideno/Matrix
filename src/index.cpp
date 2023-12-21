#include <locale.h>

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <inttypes.h>

#include "lib/pl/pl.cpp"
#include "lib/matrix/matrix.cpp"

#include "lib/matrix/matrix-int-value.hpp"

#define THICKNESS 1

#define M 3
#define N 2

int main()
{
    setlocale(LC_CTYPE, "");

    printf("[program]: start\n");

    MatrixClass<MatrixIntValue>::srand(time(NULL));

    PL<MatrixIntValue> *pl = PL<MatrixIntValue>::create_pl_random(M, N);

    printf("[program]: pl-created\n");

    pl->print();

    pl->destroy();

    printf("[program]: end\n");

    return EXIT_SUCCESS;
}