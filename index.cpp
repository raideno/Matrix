#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "util.hpp"
#include "matrix.hpp"

int main()
{
    MatrixClass::srand(time(NULL));
    MatrixClass::set_allocations_debug(false);

    MatrixClass matrix = MatrixClass::create_matrix_random_int(DIAGONAL, 3, 3).set_name("matrix");
    MatrixClass transpose = matrix.transpose();
    MatrixClass bigger = matrix.resize(2, 2).set_name("resized");

    float determinent = matrix.determinent();

    std::cout << "det(matrix)=" << determinent << std::endl;

    std::cout << matrix << std::endl;
    std::cout << transpose << std::endl;
    std::cout << bigger << std::endl;

    return 0;
}