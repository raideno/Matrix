#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "sle.hpp"
#include "util.hpp"
#include "matrix.hpp"

int main()
{
    Sle::srand(time(NULL));
    Sle::set_debug_options(SystemDebug::SYSTEM_ALL);

    MatrixClass::srand(time(NULL));
    MatrixClass::set_debug_options(MatrixDebug::MATRIX_ALL);

    std::cout << "start-program" << std::endl;

    float sum = 0;
    MatrixClass *matrix = MatrixClass::create_matrix_random_int(MatrixType::NORMAL, 3, 3)->set_name("matrix");

    matrix->print();

    sum = matrix->reduce(NORMAL, [](float acumulator, size_t i, size_t j, float value) -> float
                         { return acumulator + value; });

    printf("sum: %f\n", sum);

    matrix->destroy();

    std::cout << "end-program" << std::endl;

    return 0;
}