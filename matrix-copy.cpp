#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include <iostream>

#include "util.hpp"

/*
void create_puissance_system(size_t n, Matrix **matrix, Matrix **vector)
{
    (*matrix) = allocate_matrix(n, n);
    (*vector) = allocate_matrix(n, 1);

    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = 0; j < n; j++)
        {
            (*matrix)->matrix[i][j] = power(i + 1, j + 1);
            (*vector)->matrix[i][0] += (*matrix)->matrix[i][j];
        }
    }
}

void create_hilbert_system(size_t n, Matrix **matrix, Matrix **vector)
{
    (*matrix) = allocate_matrix(n, n);
    (*vector) = allocate_matrix(n, 1);

    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = 0; j < n; j++)
        {
            (*matrix)->matrix[i][j] = 1 / ((i + 1) + (j + 1) - 1);
            (*vector)->matrix[i][0] += (*matrix)->matrix[i][j];
        }
    }
}

Matrix *copy_matrix_in(Matrix *dest, Matrix *src, size_t startI, size_t startJ, bool destructive)
{
    // check that dimensions match

    Matrix *result = destructive ? dest : copy_matrix(dest);

    for (size_t i = startI; i < startI + src->n; i++)
        for (size_t j = startJ; j < startJ + src->m; j++)
            result->matrix[i][j] = src->matrix[i - startI][j - startJ];

    return result;
}

void lu_decomposition(Matrix *matrix, Matrix **l, Matrix **u)
{
    if (matrix->n != matrix->m)
    {
        printf("[lu_decomposition]: matrix must be square\n");
        return;
    }

    *u = copy_matrix(matrix);
    *l = allocate_matrix(matrix->n, matrix->m); // make it's diagonal ones

    for (size_t i = 0; i < matrix->n; i++)
        (*l)->matrix[i][i] = 1;

    // ajouter le truc en cas ou le pivot = 0 pour permuter entre deux lignes
    // check that dimensions are good

    // printf("gauss:\n\t");

    if (matrix == NULL)
    {
        printf("[lu_decomposition]: matrix or vector is null\n");
        return;
    }

    // check if the system is solvable or not

    for (size_t k = 0; k < matrix->n; k++) // to go through all the pivots
    {
        for (size_t i = k + 1; i < matrix->n; i++) // go though all the lines below the pivot to create zeros
        {
            float a = (*u)->matrix[k][k];
            float b = (*u)->matrix[i][k];
            float factor = b / a;
            for (size_t j = k; j < matrix->n; j++)
            {
                (*u)->matrix[i][j] = (*u)->matrix[i][j] - factor * (*u)->matrix[k][j];
                (*l)->matrix[i][k] = factor;
            }
        }
    }
}

void gauss(Matrix *matrix, Matrix *vector)
{

    // check that it can work, and that pivots aren't null

    if (matrix == NULL || vector == NULL)
    {
        printf("[gauss]: matrix or vector is null\n");
        return;
    }

    for (size_t k = 0; k < matrix->n - 1; k++)
    {
        float a = matrix->matrix[k][k];
        for (size_t i = k + 1; i < matrix->n; i++)
        {
            float b = matrix->matrix[i][k];
            for (size_t j = k; j < matrix->n; j++)
            {
                matrix->matrix[i][j] -= ((b / a) * matrix->matrix[k][j]);
            }
            vector->matrix[i][0] -= ((b / a) * vector->matrix[k][0]);
        }
    }
}

void gauss_partielle(Matrix *matrix, Matrix *vector)
{
    // check it's a square matrix

    for (size_t k = 0; k < matrix->n; k++) // loop to go through each diagonal element (pivot)
    {
        // search for maximum in this column (column k) to choose the right pivot
        size_t maxI = k; // matrix_max_column(matrix, k, true);
        size_t maxJ = k;

        // on recherche le max depuis la ligne 0 ou depuis la ligne k ?
        for (size_t i = k; i < matrix->n; i++)
        {
            if (abs(matrix->matrix[i][k]) > abs(matrix->matrix[maxI][maxJ]))
            {
                maxI = i;
                maxJ = k;
            }
        }

        // printf("max: %lu\n", maxI);

        // here maxI and maxJ store the greatest element in this column, so we permute between the line k and the line of this element

        // matrix = permute_lines(matrix, k, maxI);

        // permute between line k and line maxI
        if (maxI != k)
        {
            matrix = permute_lines(matrix, k, maxI, true);
            vector = permute_lines(vector, k, maxI, true);
            // printf("permute between line %ld and %ld\n", k, maxI);
        }

        // printf("before step %ld\n", k);
        // print_matrices_concatenation(2, matrix, vector);
        float a = matrix->matrix[k][k];
        for (size_t i = k + 1; i < matrix->n; i++) // loop to go through each line under that diagonal element and then delete (make zeros appear)
        {
            float b = matrix->matrix[i][k];
            for (size_t j = 0; j < matrix->n; j++)
            {
                matrix->matrix[i][j] -= ((b / a) * matrix->matrix[k][j]);
            }
            // do stuff inorder to delete (make it 0) the element ([i][k])
            vector->matrix[i][0] -= ((b / a) * vector->matrix[k][0]);
        }

        // printf("after step %ld\n", k);
        // print_matrices_concatenation(2, matrix, vector);
    }
}

void gauss_totale(Matrix *matrix, Matrix *vector, Matrix **reference)
{
    *reference = set_matrix_name(create_matrix_with(NORMAL, matrix->n, 2, -1), "reference");

    for (size_t k = 0; k < matrix->n - 1; k++)
    {
        // getting the maximum position
        // size_t maxI = k, maxJ = k;
        // search for maximum in this column to choose the right pivot

        // search max in the matrix or sub matrix ? here you are only doing it in the column / like gauss_partielle
        size_t maxI = k; // matrix_max_column(matrix, k, true);
        size_t maxJ = k;

        for (size_t i = k; i < matrix->n; i++)
        {
            for (size_t j = k; j < matrix->n; j++)
            {
                if (abs(matrix->matrix[i][j]) > abs(matrix->matrix[maxI][maxJ]))
                {
                    maxI = i;
                    maxJ = j;
                }
            }
        }

        // we permute the lines k and maxI

        if (maxI != k)
        {
            matrix = permute_lines(matrix, k, maxI, true);
            vector = permute_lines(vector, k, maxI, true);
            // printf("permute between line %ld and %ld\n", k, maxI);
        }

        // we permut the columns k and maxJ

        if (maxJ != k)
        {
            matrix = permute_columns(matrix, k, maxJ, true);
            (*reference)->matrix[k][0] = k;
            (*reference)->matrix[k][1] = maxJ;
            // printf("permute between column %ld and %ld\n", k, maxJ);
            //  here => we save the permutations we have done
        }

        // printf("before step %ld\n", k);
        // print_matrices_concatenation(2, matrix, vector);

        float a = matrix->matrix[k][k];
        for (size_t i = k + 1; i < matrix->n; i++)
        {
            float b = matrix->matrix[i][k];
            for (size_t j = k; j < matrix->n; j++)
            {
                matrix->matrix[i][j] -= ((b / a) * matrix->matrix[k][j]);
            }
            vector->matrix[i][0] -= ((b / a) * vector->matrix[k][0]);
        }

        printf("k=%ld\n", k);
        print_matrices_concatenation(2, matrix, vector);
        printf("\n");
    }
}

Matrix *solve_with_cramer(Matrix *matrix, Matrix *vector)
{
    float det;

    if (matrix_is_null(matrix))
    {
        printf("[solve_with_cramer]: matrix is null\n");
        return NULL;
    }

    det = determinent(matrix);

    if (det == 0)
    {
        printf("[solve_with_cramer]: determinant is equal to 0\n");
        return NULL;
    }

    Matrix *solutions = allocate_matrix(matrix->n, 1);

    for (size_t i = 0; i < matrix->n; i++)
    {
        Matrix *temp = copy_matrix_in(matrix, vector, 0, i);
        solutions->matrix[i][0] = determinent(temp) / determinent(matrix);
        destroy_matrix(temp);
    }

    return solutions;
}

Matrix *solve_upper_triangular(Matrix *matrix, Matrix *vector)
{
    float det;

    if (matrix_is_null(matrix))
    {
        printf("[solve_upper_triangular]: matrix is null\n");
        return NULL;
    }

    det = determinent(matrix);

    if (det == 0)
    {
        printf("[solve_upper_triangular]: determinant is equal to 0\n");
        return NULL;
    }

    Matrix *solutions = allocate_matrix(matrix->n, 1);

    for (int i = matrix->n - 1; i >= 0; i--)
    {
        float sum = 0;
        for (size_t j = i + 1; j < matrix->n; j++)
        {
            sum += matrix->matrix[i][j] * solutions->matrix[j][0];
        }
        solutions->matrix[i][0] = (vector->matrix[i][0] - sum) / matrix->matrix[i][i];
    }

    return solutions;
}

Matrix *solve_lower_triangular(Matrix *matrix, Matrix *vector)
{
    float det;

    if (matrix_is_null(matrix))
    {
        printf("[solve_lower_triangular]: matrix is null\n");
        return NULL;
    }

    det = determinent(matrix);

    if (det == 0)
    {
        printf("[solve_lower_triangular]: determinant is equal to 0\n");
        return NULL;
    }

    Matrix *solutions = allocate_matrix(matrix->n, 1);

    //first solution can't be done with loop

for (size_t i = 0; i < matrix->n; i++)
{
    float sum = 0;
    for (size_t j = 0; j < i; j++)
    {
        sum += matrix->matrix[i][j] * solutions->matrix[j][0];
    }
    solutions->matrix[i][0] = (vector->matrix[i][0] - sum) / matrix->matrix[i][i];
}

return solutions;
}

Matrix *create_matrix()
{
    int n = prompt("donnes n:");
    int m = prompt("donnes m:");

    Matrix *matrix = allocate_matrix(n, m);

    printf("matrix name:");
    scanf("%s", matrix->name);

    read_matrix(matrix, NORMAL);

    return matrix;
}

// not sure it works
Matrix *select_lines_array(Matrix *matrix, size_t *array, size_t size)
{
    // check that matrix isn't null check that lines in array are valid, cehck that size isn't bigger than the number of lines

    Matrix *result = allocate_matrix(size, matrix->m);

    for (size_t i = 0; i < size; i++)
        for (size_t j = 0; j < matrix->m; j++)
            result->matrix[i][j] = matrix->matrix[array[i]][j];

    return result;
}

// not sure it works
Matrix *select_columns_array(Matrix *matrix, size_t *array, size_t size)
{
    // check that matrix isn't null check that lines in array are valid, cehck that size isn't bigger than the number of lines

    Matrix *result = allocate_matrix(matrix->n, size);

    for (size_t i = 0; i < matrix->n; i++)
        for (size_t j = 0; j < size; j++)
            result->matrix[i][j] = matrix->matrix[i][array[j]];

    return result;
}

Matrix *select_array(Matrix *matrix, size_t *lines, size_t n_lines, size_t *columns, size_t n_columns)
{
    // do checks

    Matrix *temp;
    Matrix *result = NULL;

    temp = select_lines_array(matrix, lines, n_lines);

    result = select_columns_array(temp, columns, n_columns);

    destroy_matrix(temp);

    return result;
}
*/