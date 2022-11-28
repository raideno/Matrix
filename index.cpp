#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "util.hpp"
#include "matrix.hpp"

int main()
{

    MatrixClass matrix = MatrixClass::create(3, 3);

    matrix.fill(5);

    matrix.print();

    return 0;
}