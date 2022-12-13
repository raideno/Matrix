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

    MatrixClass *matrix = MatrixClass::create_matrix_random_int(MatrixType::NORMAL, 3, 3)->set_name("matrix");
    MatrixClass *vector = MatrixClass::create_matrix_random_int(MatrixType::NORMAL, 3, 1)->set_name("vector");

    Sle *system = Sle::creaate_system(matrix, vector)->set_name("system");

    system->print();

    matrix->print();

    matrix->destroy();
    vector->destroy();
    system->destroy();

    std::cout << "end-program" << std::endl;

    return 0;
}