#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "sle.hpp"
#include "util.hpp"
#include "matrix.hpp"

std::size_t order = 3;
float vector_array[] = {9, 1, 0};
float matrix_array[] = {1, 1, 2, 2, 4, -3, 3, 6, -5};

int main()
{
    MatrixClass::srand(45);
    MatrixClass::set_debug_options(Debug::ALL);

    std::cout << "start-program" << std::endl;

    MatrixClass *matrix = MatrixClass::matrix_from_array(NORMAL, matrix_array, order, order)->set_name("matrix-original");
    MatrixClass *vector = MatrixClass::matrix_from_array(NORMAL, vector_array, order, 1)->set_name("vector-original");

    Sle *system = new Sle(matrix, vector);

    // auto [l, u] = MatrixClass::lu_decomposition(system->matrix);

    system->gauss_jordan(true)->solve_upper_triangular()->set_name("solutions")->print();
    printf("\n");

    system->print();

    delete system;
    delete matrix;
    delete vector;

    std::cout << "end-program" << std::endl;

    return 0;
}