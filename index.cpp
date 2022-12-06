#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "sle.hpp"
#include "util.hpp"
#include "matrix.hpp"

float matrix_array[] = {1, 6, 9, 2, 1, 2, 3, 6, 9};
float vector_array[] = {1, 2, 3};

int main()
{
    MatrixClass::srand(time(NULL));
    MatrixClass::set_debug_options(MISC);

    Sle system = Sle::create_power_system(3);

    std::cout << system << std::endl;

    system.gauss();

    MatrixClass solutions = system.solve_upper_triangular().set_name("solutions");

    std::cout << system << std::endl;

    std::cout << solutions << std::endl;

    std::cout << "program-end" << std::endl;

    /*
    std::cout << matrix << std::endl;

    auto [lower, upper] = matrix.lu_decomposition();

    std::cout << lower << std::endl;
    std::cout << upper << std::endl;
    */
    return 0;
}