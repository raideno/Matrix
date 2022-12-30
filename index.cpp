#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "sle.hpp"
#include "util.hpp"
#include "matrix.hpp"

int main()
{
    Sle::srand(time(NULL));
    Sle::set_debug_options(SystemDebug::SYSTEM_NONE);

    MatrixClass::srand(time(NULL));
    MatrixClass::set_debug_options(MatrixDebug::MATRIX_NONE);

    srand(time(NULL));

    std::cout << "start-program" << std::endl;

    MatrixClass *vector = MatrixClass::create_matrix_random_float(NORMAL, 1, 5)->set_name("random-vector");

    vector->print();

    float medium = vector->reduce(
        NORMAL, [size = (vector->size().first * vector->size().second)](float acumulator, size_t i, size_t j, float value) -> float
        { return acumulator + value / size; },
        0);

    printf("medium=%f\n", medium);

    vector->destroy();

    return 0;
}