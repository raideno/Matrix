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

#define THICKNESS 1

#define NUMBER_OF_CONSTRAINTS 3
#define NUMBER_OF_VARIABLES 2

int main()
{
    setlocale(LC_CTYPE, "");

    printf("[program]: start\n");

    // unsigned int seed = time(NULL);
    unsigned int seed = 1703272015;

    printf("[program]: seed = %d\n", seed);

    // TODO: create same function in PL and SLE classes that'll make a call to the MatrixClass::srand function at their turn
    MatrixClass<MatrixIntValue>::srand(seed);

    PL<MatrixIntValue> *pl = PL<MatrixIntValue>::create_random_pl(NUMBER_OF_CONSTRAINTS, NUMBER_OF_VARIABLES);

    printf("[program]: pl-created\n");

    pl->print();

    for (size_t i = 0; i < NUMBER_OF_CONSTRAINTS; i++)
    {
        // pl->convert_contraint_type_to(i, pl->get_constraint_type(i) * -1);
        pl->convert_contraint_type_to(i, 0);
    }

    printf("\n\n");

    pl->print();

    pl->destroy();

    printf("[program]: end\n");

    return EXIT_SUCCESS;
}