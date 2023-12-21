#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <inttypes.h>

#include "lib/matrix/matrix.cpp"
#include "lib/matrix/matrix-int-value.hpp"

#define THICKNESS 1

#define N 2
#define M 3

void print_objective_function(MatrixClass<MatrixIntValue> *objective_function_coefficients)
{
    printf("F(");
    for (int i = 0; i < objective_function_coefficients->size().second; i++)
    {
        printf("x%d", i);
        if (i != objective_function_coefficients->size().second - 1)
            printf(", ");
    }
    printf(") = ");

    for (int i = 0; i < objective_function_coefficients->size().second; i++)
    {
        objective_function_coefficients->get(0, i).print();
        printf(" * x%d", i);
        if (i != objective_function_coefficients->size().second - 1)
            printf(" + ");
    };

    printf("\n");

    return;
}

int main()
{
    printf("[start]\n");

    MatrixClass<MatrixIntValue>::srand(time(NULL));

    MatrixIntValue number = MatrixIntValue(10);
    MatrixIntValue one = MatrixIntValue(1);

    MatrixClass<MatrixIntValue> *matrixA = MatrixClass<MatrixIntValue>::multiply_matrix_value(MatrixClass<MatrixIntValue>::create_matrix_random(MatrixType::NORMAL, M, N), number);
    MatrixClass<MatrixIntValue> *matrixC = MatrixClass<MatrixIntValue>::multiply_matrix_value(MatrixClass<MatrixIntValue>::create_matrix_random(MatrixType::NORMAL, 1, N), number);
    MatrixClass<MatrixIntValue> *matrixB = MatrixClass<MatrixIntValue>::multiply_matrix_value(MatrixClass<MatrixIntValue>::create_matrix_random(MatrixType::NORMAL, 1, N), number);

    MatrixClass<MatrixIntValue> *matrixIdentity = MatrixClass<MatrixIntValue>::create_matrix_with(MatrixType::DIAGONAL, M, M, one);

    matrixA->set_name("Matrix-A");
    matrixC->set_name("Matrix-C");
    matrixB->set_name("Matrix-B");

    matrixA->print();
    matrixC->print();
    matrixB->print();

    matrixIdentity->print();

    MatrixClass<MatrixIntValue>::concatenate_horizontal(2, matrixA, matrixIdentity)->print();

    print_objective_function(matrixC);

    printf("[end]\n");

    matrixA->destroy();
    matrixB->destroy();
    matrixC->destroy();

    matrixIdentity->destroy();

    return EXIT_SUCCESS;
}