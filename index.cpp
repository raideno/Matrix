#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "sle.hpp"
#include "util.hpp"
#include "matrix.hpp"

int main()
{
    MatrixClass::srand(time(NULL));
    MatrixClass::set_debug_options(MatrixDebug::MATRIX_ALL);

    std::cout << "start-program" << std::endl;

    MatrixClass *matrix = MatrixClass::create_matrix_random_int(MatrixType::NORMAL, 3, 3)->set_name("matrix");

    matrix->print();

    delete matrix;

    std::cout << "end-program" << std::endl;

    return 0;
}