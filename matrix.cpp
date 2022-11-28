#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include <iostream>

#include "util.hpp"
#include "matrix.hpp"

void MatrixClass::fill(float number)
{
    for (size_t i = 0; i < this->n; i++)
        for (size_t j = 0; j < this->m; j++)
            (*this)[i][j] = number;
}

float *MatrixClass::operator[](size_t i)
{
    // check if it exists first;
    return this->content[i];
}

void MatrixClass::print()
{
    printf("[\n");
    for (size_t i = 0; i < this->n; i++)
    {
        printf("\t[");
        for (size_t j = 0; j < this->m; j++)
        {
            printf("%.2f%s", (*this)[i][j], j == this->m - 1 ? "\0" : ", ");
        }
        printf("]\n");
    }
    printf("]\n");
}

MatrixClass::MatrixClass(size_t n, size_t m)
{
    this->n = n;
    this->m = m;

    this->content = (float **)calloc(n, sizeof(float *));

    for (size_t i = 0; i < this->n; i++)
    {
        this->content[i] = (float *)calloc(this->m, sizeof(float));
    }
}

MatrixClass &MatrixClass::create(size_t n, size_t m)
{
    MatrixClass *matrix = new MatrixClass(n, m);
    return *matrix;
}

MatrixType matrix_type(Matrix *matrix)
{
    /*
        we check if it's a diagonal matrix
        we check if it's a lower triangular / upper triangular matrix
        otherwise it's a normal matrix
    */

    // check if it's a all zero matrix and return NORMAL if it's the case
    bool allZeroes;
    bool isDiagonal = true;
    bool isLowerTriangular = true;
    bool isUpperTriangular = true;
    bool isNormal = true;

    for (size_t i = 0; i < matrix->n; i++)
        for (size_t j = 0; j < matrix->m; j++)
            if (matrix->matrix[i][j] != 0)
                allZeroes = false;

    if (allZeroes)
        return NORMAL;

    if (matrix->n != matrix->m)
        return NORMAL;

    // DIAGONAL
    for (size_t i = 0; i < matrix->n; i++)
        for (size_t j = 0; j < matrix->m; j++)
            if (i != j && matrix->matrix[i][j] != 0)
                isDiagonal = false;

    /*check that diagonal is filled with some stuff and it's not only a 0 matrix*/

    if (isDiagonal)
        return DIAGONAL;

    // LOWER TRIANGLE
    for (size_t i = 0; i < matrix->n; i++)
        for (size_t j = i + 1; j < matrix->m; j++)
            if (matrix->matrix[i][j] != 0)
                isLowerTriangular = false;

    if (isLowerTriangular)
        return LOWER_TRIANGLE;

    // UPPER TRIANGLE

    for (size_t i = 0; i < matrix->n; i++)
        for (size_t j = 0; j < i - 1; j++)
            if (matrix->matrix[i][j] != 0)
                isUpperTriangular = false;

    if (isUpperTriangular)
        return LOWER_TRIANGLE;

    return NORMAL;
}

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

Matrix *subtract_matrix_matrix(Matrix *matrixA, Matrix *matrixB)
{
    Matrix *result;

    if (matrixA == NULL || matrixB == NULL)
    {
        printf("[subtract_matrix_matrix]: one of the two matrices is null\n");
        return NULL;
    }

    if (matrixA->n != matrixB->n || matrixA->m != matrixB->m)
    {
        printf("[subtract_matrix_matrix]: matrices have to be of the same dimention\n");
        return NULL;
    }

    result = allocate_matrix(matrixA->n, matrixA->m);

    matrix_map(result, NORMAL, [&matrixA, &matrixB](size_t i, size_t j, float element) -> float
               { return matrixA->matrix[i][j] - matrixB->matrix[i][j]; });

    return result;
}

bool matrix_map(Matrix *matrix, MatrixType type, Producer producer)
{
    if (matrix_is_null(matrix))
    {
        printf("[matrix_better_map]: matrix is null\n");
        return false;
    }

    if ((type == DIAGONAL || type == UPPER_TRIANGLE || type == LOWER_TRIANGLE) && !matrix_is_square(matrix))
    {
        printf("[matrix_better_map]: matrix must be a square matrix for this type\n");
        return false;
    }

    switch (type)
    {
    case NORMAL:
        for (size_t i = 0; i < matrix->n; i++)
            for (size_t j = 0; j < matrix->m; j++)
                matrix->matrix[i][j] = producer(i, j, matrix->matrix[i][j]);
        break;
    case DIAGONAL:
        for (size_t i = 0; i < matrix->n; i++)
            matrix->matrix[i][i] = producer(i, i, matrix->matrix[i][i]);

        break;
    case UPPER_TRIANGLE:
        for (size_t i = 0; i < matrix->n; i++)

            for (size_t j = i; j < matrix->m; j++)
                matrix->matrix[i][j] = producer(i, j, matrix->matrix[i][j]);

        break;
    case LOWER_TRIANGLE:
        for (size_t i = 0; i < matrix->n; i++)
            for (size_t j = 0; j <= i; j++)
                matrix->matrix[i][j] = producer(i, j, matrix->matrix[i][j]);
        break;
    default:
        printf("[create_matrix_with]: invalid matrix type\n");
        return false;
        break;
    }

    return true;
}

bool matrix_map_line(Matrix *matrix, size_t line, Producer producer)
{
    // make checks that matrix isn't null and that the given line exist and other stuff like this
    for (size_t j = 0; j < matrix->m; j++)
        matrix->matrix[line][j] = producer(line, j, matrix->matrix[line][j]);

    return true;
}

bool matrix_map_column(Matrix *matrix, size_t column, Producer producer)
{
    // make checks that matrix isn't null and that the given line exist and other stuff like this
    for (size_t i = 0; i < matrix->n; i++)
        matrix->matrix[i][column] = producer(i, column, matrix->matrix[i][column]);
    return true;
}

bool matrix_for_each(Matrix *matrix, MatrixType type, Consumer consumer)
{
    if (matrix_is_null(matrix))
    {
        printf("[matrix_better_for_each]: matrix is null\n");
        return false;
    }

    if ((type == DIAGONAL || type == UPPER_TRIANGLE || type == LOWER_TRIANGLE) && !matrix_is_square(matrix))
    {
        printf("[matrix_better_for_each]: matrix must be a square matrix for this type\n");
        return false;
    }

    return matrix_map(
        matrix, type, [consumer](size_t i, size_t j, float number) -> float
        { consumer(i, j, number); return number; });
}

Matrix *create_matrix_with(MatrixType type, size_t n, size_t m, float number)
{

    // if type isn't valid we don't create or we put it default type as NORMAL
    Matrix *matrix = allocate_matrix(n, m);

    if (matrix_is_null(matrix))
    {
        printf("[create_matrix_with]: matrix is null\n");
        return NULL;
    }

    if ((type == DIAGONAL || type == UPPER_TRIANGLE || type == LOWER_TRIANGLE) && !matrix_is_square(matrix))
    {
        printf("[create_matrix_with]: matrix must be a square matrix for this type\n");
        return NULL;
    }

    /*what if type isn't good*/
    matrix_map(
        matrix, type, [number](size_t i, size_t j, float element) -> float
        { return number; });

    return matrix;
}

void read_matrix(Matrix *matrix, MatrixType type)
{
    if (matrix_is_null(matrix))
    {
        printf("[read_matrix]: matrix is null\n");
        return;
    }

    if ((type == DIAGONAL || type == UPPER_TRIANGLE || type == LOWER_TRIANGLE) && !matrix_is_square(matrix))
    {
        printf("[read_matrix]: matrix must be a square matrix for this type\n");
        return;
    }

    matrix_map(
        matrix, type, [](size_t i, size_t j, float number) -> float
        {
            float result;
            printf("matrix[%ld][%ld]=", i, i);
            scanf("%f", &result);
            return result; });
}

void matrix_srand(unsigned int seed)
{
    srand(seed);
}

bool matrix_all(Matrix *matrix, MatrixType type, BooleanProducer boolean_producer)
{
    bool result = true;

    matrix_for_each(matrix, type, [&boolean_producer, &result, &matrix](size_t i, size_t j, float element) -> void
                    {
        if (!boolean_producer(i, j, matrix->matrix[i][j]))
            result = false; });

    return result;
}

bool matrix_line_all(Matrix *matrix, size_t line, BooleanProducer boolean_producer)
{
    for (size_t j = 0; j < matrix->m; j++)
        if (!boolean_producer(line, j, matrix->matrix[line][j]))
            return false;

    return true;
}

bool matrix_column_all(Matrix *matrix, size_t column, BooleanProducer boolean_producer)
{
    for (size_t i = 0; i < matrix->n; i++)
        if (!boolean_producer(i, column, matrix->matrix[i][column]))
            return false;

    return true;
}

//

bool matrix_one(Matrix *matrix, MatrixType type, BooleanProducer boolean_producer)
{
    bool result = false;

    matrix_for_each(matrix, type, [&boolean_producer, &result, &matrix](size_t i, size_t j, float element) -> void
                    {
        if (boolean_producer(i, j, matrix->matrix[i][j]))
            result = true; });

    return result;
}

bool matrix_line_one(Matrix *matrix, size_t line, BooleanProducer boolean_producer)
{
    for (size_t j = 0; j < matrix->m; j++)
        if (boolean_producer(line, j, matrix->matrix[line][j]))
            return true;

    return false;
}

bool matrix_column_one(Matrix *matrix, size_t column, BooleanProducer boolean_producer)
{
    for (size_t i = 0; i < matrix->n; i++)
        if (boolean_producer(i, column, matrix->matrix[i][column]))
            return true;

    return false;
}

Matrix *add_matrix_float(Matrix *matrix, float a)
{
    Matrix *result = copy_matrix(matrix);

    matrix_map(matrix, NORMAL, [a](size_t i, size_t j, float element) -> float
               { return element + a; });

    return result;
}

Matrix *substract_matrix_float(Matrix *matrix, float a)
{
    return add_matrix_float(matrix, -a);
}

// make the print check the max in the column and not in the entire matrix
void print_matrix(Matrix *matrix, size_t precision, bool sign)
{
    size_t maxI, maxJ, size;

    if (matrix_is_null(matrix))
    {
        printf("[print]: matrix is null\n");
        return;
    }

    if (strlen(matrix->name) > 0)
        printf("%s:\n", matrix->name);

    matrix_max(matrix, NORMAL, &maxI, &maxJ);
    size = count_digits(matrix->matrix[maxI][maxJ]);

    printf("[\n");
    for (size_t i = 0; i < matrix->n; i++)
    {
        printf("   [");
        for (size_t j = 0; j < matrix->m; j++)
        {
            print_float(matrix->matrix[i][j], size, precision, ' ', sign);
            printf("%s", j == matrix->m - 1 ? "\0" : ", ");
        }
        printf("]\n");
    }
    printf("]\n");
}

bool matrix_for_each_line(Matrix *matrix, size_t line, Consumer consumer)
{
    // make checks that matrix isn't null and that the given line exists and other basic stuff like this
    for (size_t j = 0; j < matrix->m; j++)
        consumer(line, j, matrix->matrix[line][j]);
    return true;
}

bool matrix_for_each_column(Matrix *matrix, size_t column, Consumer consumer)
{
    // make checks that matrix isn't null and that the given line exists and other basic stuff like this
    for (size_t i = 0; i < matrix->n; i++)
        consumer(i, column, matrix->matrix[i][column]);
    return true;
}

Matrix *create_matrix_random_float(MatrixType type, size_t n, size_t m)
{
    Matrix *result = allocate_matrix(n, m);

    matrix_map(
        result, type, [](size_t i, size_t j, float number) -> float
        { return random_float(0, 1); });

    return result;
}

Matrix *create_matrix_random_float(MatrixType type, size_t n, size_t m, float min, float max)
{
    Matrix *result = allocate_matrix(n, m);

    matrix_map(
        result, type, [min, max](size_t i, size_t j, float number) -> float
        { return random_float(min, max); });

    return result;
}

Matrix *create_matrix_random_int(MatrixType type, size_t n, size_t m, int min, int max)
{
    Matrix *result = allocate_matrix(n, m);

    matrix_map(
        result, type, [min, max](size_t i, size_t j, float number) -> float
        { return (float)random_integer(min, max); });

    return result;
}

Matrix *replace_columns(Matrix *matrixA, size_t Afrom, size_t Ato, Matrix *matrixB, size_t Bfrom, size_t Bto)
{
    Matrix *result;
    size_t size = Ato - Afrom + 1;

    if (matrix_is_null(matrixA) || matrix_is_null(matrixB))
    {
        printf("[replace_columns]: one of the matrices is null\n");
        return NULL;
    }

    if (matrixA->n != matrixB->n)
    {
        printf("[replace_columns]: the two matrices aren't compatible\n");
        return NULL;
    }

    if ((Ato - Afrom != Bto - Bfrom) || (Afrom > Ato || Bfrom > Bto) || (Afrom > matrixA->m - 1 || Bfrom > matrixB->m - 1 || Ato > matrixA->m - 1 || Bto > matrixB->m - 1))
    {
        printf("[replace_columns]: invalid params\n");
        return NULL;
    }

    result = copy_matrix(matrixA);

    for (size_t i = 0; i < matrixA->n; i++)
        for (size_t j = Bfrom; j < size; j++)
            result->matrix[i][j - Bfrom + Afrom] = matrixB->matrix[i][j];

    return result;
}

Matrix *replace_lines(Matrix *matrixA, size_t Afrom, size_t Ato, Matrix *matrixB, size_t Bfrom, size_t Bto)
{
    Matrix *result;
    size_t size = Ato - Afrom + 1;

    if (matrix_is_null(matrixA) || matrix_is_null(matrixB))
    {
        printf("[replace_lines]: one of the matrices is null\n");
        return NULL;
    }

    if (matrixA->m != matrixB->m)
    {
        printf("[replace_lines]: the two matrices aren't compatible\n");
        return NULL;
    }

    if ((Ato - Afrom != Bto - Bfrom) || (Afrom > Ato || Bfrom > Bto) || (Afrom > matrixA->n - 1 || Bfrom > matrixB->n - 1 || Ato > matrixA->n - 1 || Bto > matrixB->n - 1))
    {
        printf("[replace_lines]: invalid params\n");
        return NULL;
    }

    result = copy_matrix(matrixA);

    for (size_t i = Bfrom; i < size; i++)
        for (size_t j = 0; j < matrixA->m; j++)
            result->matrix[i - Bfrom + Afrom][j] = matrixB->matrix[i][j];

    return result;
}

Matrix *set_matrix_name(Matrix *matrix, const char name[25])
{
    for (size_t i = 0; i < 25; i++)
        matrix->name[i] = name[i];

    return matrix;
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

Matrix *concatenate_horizontal(size_t n_args, ...)
{
    // if one of the params is NULL, or invalid we destroy the allocated matrice + print an error
    va_list ap;
    Matrix *matrix;
    Matrix *result;
    Matrix **matrices = (Matrix **)calloc(n_args, sizeof(Matrix *));

    int lines = 0;
    int columns = 0;
    int progress = 0;

    va_start(ap, n_args);

    for (int i = 0; i < n_args; i++)
    {
        matrix = va_arg(ap, Matrix *);
        matrices[i] = matrix;
        if (matrices[i]->n > lines)
            lines = matrices[i]->n;
        columns += matrices[i]->m;
    }
    va_end(ap);

    result = allocate_matrix(lines, columns);

    for (size_t k = 0; k < n_args; k++)
    {
        for (size_t i = 0; i < matrices[k]->n; i++)
        {
            for (size_t j = 0; j < matrices[k]->m; j++)
            {
                result->matrix[i][j + progress] = matrices[k]->matrix[i][j];
            }
        }
        progress += matrices[k]->m;
    }

    return result;
}

Matrix *concatenate_vertical(size_t n_args, ...)
{
    // if one of the params is NULL, or invalid we destroy the allocated matrice + print an error

    va_list ap;
    Matrix *matrix;
    Matrix *result;
    Matrix **matrices = (Matrix **)calloc(n_args, sizeof(Matrix *));

    int lines = 0;
    int columns = 0;
    int progress = 0;

    va_start(ap, n_args);

    for (int i = 0; i < n_args; i++)
    {
        matrix = va_arg(ap, Matrix *);
        matrices[i] = matrix;
        if (matrices[i]->n > columns)
            columns = matrices[i]->m;
        lines += matrices[i]->n;
    }
    va_end(ap);

    result = allocate_matrix(lines, columns);

    for (size_t k = 0; k < n_args; k++)
    {
        for (size_t i = 0; i < matrices[k]->n; i++)
        {
            for (size_t j = 0; j < matrices[k]->m; j++)
            {
                result->matrix[i + progress][j] = matrices[k]->matrix[i][j];
            }
        }
        progress += matrices[k]->n;
    }

    return result;
}

// not working correctly for non square matrices
Matrix *matrix_from_array(MatrixType type, float *array, size_t n, size_t m)
{
    int counter = 0;
    Matrix *matrix = allocate_matrix(n, m);

    // check that the sizes are compatible

    matrix_map(matrix, type, [&array, &counter](size_t i, size_t j, float element) -> float
               { return array[counter++]; });

    return matrix;
}

void print_matrices_concatenation(size_t n_args, ...)
{
    int lines;
    va_list ap;
    Matrix *matrix;

    va_start(ap, n_args);

    matrix = va_arg(ap, Matrix *);
    lines = matrix->n;

    for (int i = 1; i < n_args; i++)
    {
        if (matrix == NULL)
        {
            printf("[print_matrices_concatenation]: null matrix was provided\n");
            return;
        }
        if (matrix->n > lines)
            lines = matrix->n;
        matrix = va_arg(ap, Matrix *); /*mets la en premiere ligne si jamais probleme*/
    }
    va_end(ap);

    for (size_t i = 0; i < lines; i++)
    {
        va_start(ap, n_args);
        for (size_t j = 0; j < n_args; j++)
        {
            matrix = va_arg(ap, Matrix *);

            if (i < matrix->n)
                print_matrix_line(matrix, i, 2, true);
            else
                printf("\t");
            printf("\t");
        }
        va_end(ap);
        printf("\n");
    }
}

bool is_matrix_triangular(Matrix *matrix)
{
    if (matrix_is_null(matrix))
        return false;

    if (matrix->n != matrix->m)
        return false;

    return is_matrix_upper_triangular(matrix) || is_matrix_lower_triangular(matrix);
}

bool is_matrix_lower_triangular(Matrix *matrix)
{
    if (matrix_is_null(matrix))
        return false;

    if (matrix->n != matrix->m)
        return false;

    for (size_t i = 0; i < matrix->n; i++)
        for (size_t j = 0; j < i; j++)
            if (matrix->matrix[i][j] != 0)
                return false;

    return true;
}

bool is_matrix_upper_triangular(Matrix *matrix)
{
    if (matrix_is_null(matrix))
        return false;

    if (matrix->n != matrix->m)
        return false;

    for (size_t i = 0; i < matrix->n; i++)
        for (size_t j = i + 1; j < matrix->n; j++)
            if (matrix->matrix[i][j] != 0)
                return false;

    return true;
}

Matrix *multiply_matrix_matrix(Matrix *matrixA, Matrix *matrixB)
{
    Matrix *result;

    if (matrixA == NULL || matrixB == NULL)
    {
        printf("[multiply_matrix_matrix]: one of the matricies is null\n");
        return 0;
    }

    if (matrixA->m != matrixB->n)
    {
        printf("[multiply_matrix_matrix]: number of lines and number of columns must be equal\n");
        return NULL;
    }

    result = allocate_matrix(matrixA->n, matrixB->m);

    for (size_t i = 0; i < matrixA->n; i++)
        for (size_t j = 0; j < matrixB->m; j++)
            result->matrix[i][j] = multiply_matrix_line_matrix_column(matrixA, matrixB, i, j);

    return result;
}

float multiply_matrix_line_matrix_column(Matrix *matrixA, Matrix *matrixB, size_t LINE, size_t COLUMN)
{
    float result = 0;

    if (matrixA == NULL || matrixB == NULL)
    {
        printf("[multiply_matrix_line_matrix_column]: one of the matricies is null\n");
        return 0;
    }

    if (LINE > matrixA->n - 1 || COLUMN > matrixB->m - 1)
    {
        printf("[multiply_matrix_line_matrix_column]: given an invalid LINE or COLUMN\n");
        return 0;
    }

    for (size_t i = 0; i < matrixA->m; i++)
        result += matrixA->matrix[LINE][i] * matrixB->matrix[i][COLUMN];

    return result;
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

    /*first solution can't be done with loop*/

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

Matrix *multiply_matrix_float(Matrix *matrix, float number)
{
    Matrix *result;

    if (matrix_is_null(matrix))
    {
        printf("[multiply_matrix_float]: matrix is null\n");
        return NULL;
    }

    matrix_map(result, NORMAL, [&matrix, number](size_t i, size_t j, float element) -> float
               { return matrix->matrix[i][j] * number; });

    return result;
}

Matrix *cofactor(Matrix *matrix)
{
    Matrix *result = NULL;

    if (matrix_is_null(matrix))
    {
        printf("[cofactor]: matrix is null\n");
        return NULL;
    }

    if (matrix->n != matrix->m)
    {
        printf("[cofactor]: matrix have to be a ssquare matrix\n");
        return NULL;
    }

    result = allocate_matrix(matrix->n, matrix->n);

    for (size_t i = 0; i < matrix->n; i++)
    {
        for (size_t j = 0; j < matrix->n; j++)
        {
            Matrix *cofact = cofactor_of(matrix, i, j);
            float det_cofact = determinent(cofact);
            result->matrix[i][j] = power(-1, i + 1 + j + 1) * det_cofact;
            destroy_matrix(cofact);
        }
    }

    return result;
}

Matrix *inverse_matrix(Matrix *matrix)
{
    float det;
    Matrix *result;
    Matrix *cofactor_matrix;
    Matrix *transpose_cofactor_matrix;

    if (matrix_is_null(matrix))
    {
        printf("[inverse_matrix]: matrix is null\n");
        return NULL;
    }

    if (matrix->n != matrix->m)
    {
        printf("[inverse_matrix]: matrix have to be a ssquare matrix\n");
        return NULL;
    }

    det = determinent(matrix);

    if (det == 0)
    {
        printf("[inverse_matrix]: determinant is equal to 0\n");
        return NULL;
    }

    cofactor_matrix = cofactor(matrix);

    transpose_cofactor_matrix = transpose_matrix(cofactor_matrix);

    result = multiply_matrix_float(transpose_cofactor_matrix, 1 / det);

    destroy_matrix(cofactor_matrix);
    destroy_matrix(transpose_cofactor_matrix);

    return result;
}

// only for square matrices ?
Matrix *cofactor_of(Matrix *matrix, size_t line, size_t column)
{
    int k = 0;
    float *array;
    Matrix *result;

    if (matrix_is_null(matrix))
    {
        printf("[cofactor_of]: matrix is null\n");
        return NULL;
    }

    if (line > matrix->n - 1 || column > matrix->m - 1)
    {
        printf("[cofactor_of]: provided an invalid column / line\n");
        return NULL;
    }

    array = (float *)calloc(matrix->n * matrix->n, sizeof(float));

    for (size_t i = 0; i < matrix->n; i++)
    {
        for (size_t j = 0; j < matrix->m; j++)
        {
            if (i != line && j != column)
            {
                array[k] = matrix->matrix[i][j];
                k++;
            }
        }
    }

    result = matrix_from_array(NORMAL, array, matrix->n - 1, matrix->m - 1);

    free(array);

    return result;
}

/*optimiser la fonction pour selectionner la ligne avec le plus de zero !*/
/*faire une fonction qui trouve la ligne ou il y ale plus de zero*/
/*faire une fonction qui trouve la cologne ou il y a le plus de zero*/
float determinent(Matrix *matrix)
{
    float result = 0;

    if (matrix->n != matrix->m)
    {
        printf("[determinent]: can't compute determinent of a non square matrix\n");
        return 0;
    }

    if (matrix->n == 1)
        return matrix->matrix[0][0];

    if (matrix->n == 2)
        return (matrix->matrix[0][0] * matrix->matrix[1][1]) - (matrix->matrix[0][1] * matrix->matrix[1][0]);

    for (size_t i = 0; i < matrix->n; i++)
    {
        Matrix *cofact = cofactor_of(matrix, 0, i);
        /*0 in 0 + i in power fonction is for the j (i) but since we selected the first line*/
        /*should we let the "+1" or no ? or replace it with a "+1" ? and why exactly "+2" ? why it wouldn't work normaly ?*/
        result += power(-1, 0 + i + 2) * matrix->matrix[0][i] * determinent(cofact);
        destroy_matrix(cofact);
    }

    return result;
}

void print_matrix_line(Matrix *matrix, size_t line, size_t precision, bool sign)
{
    size_t maxI, maxJ, size;

    if (matrix_is_null(matrix))
    {
        printf("[print_matrix_line]: matrice is null\n");
        return;
    }

    if (line > matrix->n - 1)
    {
        printf("[print_matrix_line]: invalid line provided\n");
        return;
    }

    matrix_max(matrix, NORMAL, &maxI, &maxJ);
    size = count_digits(matrix->matrix[maxI][maxJ]);

    printf("[");
    for (size_t j = 0; j < matrix->m; j++)
    {

        // printf("%.2f", matrix->matrix[line][j]);
        print_float(matrix->matrix[line][j], size, precision, ' ', sign);
        printf("%s", j == matrix->m - 1 ? "\0" : ", ");
    }
    printf("]");
}

void print_matrix_col(Matrix *matrix, size_t column, size_t precision, bool sign)
{
    size_t maxI, maxJ, size;

    if (matrix_is_null(matrix))
    {
        printf("[print_matrix_col]: matrice is null\n");
        return;
    }

    if (column > matrix->m - 1)
    {
        printf("[print_matrix_col]: invalid column provided\n");
        return;
    }

    matrix_max(matrix, NORMAL, &maxI, &maxJ);
    size = count_digits(matrix->matrix[maxI][maxJ]);

    printf("[");
    for (size_t i = 0; i < matrix->n; i++)
    {
        print_float(matrix->matrix[i][column], size, precision, ' ', sign);
        printf("%s", i == matrix->n - 1 ? "\0" : ", ");
    }
    printf("]");
}

Matrix *add_matrix_matrix(Matrix *matrixA, Matrix *matrixB)
{
    Matrix *result;

    if (matrixA == NULL || matrixB == NULL)
    {
        printf("[add_matrix_matrix]: one of the two matrices is null\n");
        return NULL;
    }

    if (matrixA->n != matrixB->n || matrixA->m != matrixB->m)
    {
        printf("[add_matrix_matrix]: matrices have to be of the same dimention\n");
        return NULL;
    }

    result = allocate_matrix(matrixA->n, matrixA->m);

    matrix_map(result, NORMAL, [&matrixA, &matrixB](size_t i, size_t j, float element) -> float
               { return matrixA->matrix[i][j] + matrixB->matrix[i][j]; });

    return result;
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

float trace(Matrix *matrix)
{
    float result = 0;

    if (matrix_is_null(matrix))
    {
        printf("[trace]: matrix is null\n");
        return 0;
    }

    if (matrix->n != matrix->m)
    {
        printf("[trace]: matrix have to be a square matrix\n");
        return 0;
    }

    for (size_t i = 0; i < matrix->n; i++)
        result += matrix->matrix[i][i];

    return result;
}

void destroy_matrix(Matrix *matrix)
{
    if (matrix_is_null(matrix))
    {
        printf("[destroy_matrix]: matrix is NULL\n");
        return;
    }

    for (size_t i = 0; i < matrix->n; i++)
        free(matrix->matrix[i]);

    free(matrix->matrix);

    free(matrix);
}

void destroy_matrices(size_t n_args, ...)
{
    va_list ap;
    Matrix *matrix;

    va_start(ap, n_args);

    for (size_t i = 0; i < n_args; i++)
    {
        matrix = va_arg(ap, Matrix *);
        destroy_matrix(matrix);
    }

    va_end(ap);
}

Matrix *allocate_matrix(size_t n, size_t m)
{
    Matrix *matrix = (Matrix *)calloc(1, sizeof(Matrix));

    if (matrix_is_null(matrix))
    {
        printf("[allocate_matrix]: NULL\n");
        free(matrix);
        return NULL;
    }

    matrix->n = n;
    matrix->m = m;

    matrix->matrix = (float **)calloc(n, sizeof(float *));

    if (matrix->matrix == NULL)
    {
        printf("[allocate_matrix]: NULL\n");
        free(matrix);
        return NULL;
    }

    for (size_t i = 0; i < matrix->n; i++)
    {
        matrix->matrix[i] = (float *)calloc(matrix->m, sizeof(float));
        if (matrix->matrix[i] == NULL)
        {
            printf("[allocate_matrix]: NULL\n");
            // free all the others + free(matrix)
            return NULL;
        }
    }

    return matrix;
}

Matrix *transpose_matrix(Matrix *matrix)
{
    Matrix *result;

    if (matrix_is_null(matrix))
    {
        printf("[transpose_matrix]: matrix is null\n");
        return NULL;
    }

    result = allocate_matrix(matrix->m, matrix->n);

    for (size_t i = 0; i < matrix->n; i++)
        for (size_t j = 0; j < matrix->m; j++)
            result->matrix[j][i] = matrix->matrix[i][j];

    return result;
}

Matrix *copy_matrix(Matrix *matrix)
{
    Matrix *result;

    if (matrix_is_null(matrix))
    {
        printf("[copy_matrix]: matrix is null\n");
        return NULL;
    }

    result = allocate_matrix(matrix->n, matrix->m);

    for (size_t i = 0; i < matrix->n; i++)
        for (size_t j = 0; j < matrix->m; j++)
            result->matrix[i][j] = matrix->matrix[i][j];

    return result;
}

Matrix *delete_lines(Matrix *matrix, size_t start, size_t end)
{
    // check that start < end

    if (matrix_is_null(matrix))
    {
        printf("[delete_lines]: matrix is null\n");
        return NULL;
    }

    if (start > matrix->n - 1 || end > matrix->n - 1)
    {
        printf("[delete_lines]: received invalid startLine or endLine\n");
        return NULL;
    }

    size_t lines = matrix->n - (end - start + 1);
    Matrix *result = allocate_matrix(lines, matrix->m);

    for (size_t i = 0; i < start; i++)
        for (size_t j = 0; j < matrix->m; j++)
            result->matrix[i][j] = matrix->matrix[i][j];

    for (size_t i = end + 1; i < matrix->n; i++)
        for (size_t j = 0; j < matrix->m; j++)
            result->matrix[i - end][j] = matrix->matrix[i][j];

    return result;
}

Matrix *delete_columns(Matrix *matrix, size_t start, size_t end)
{
    // check that start < end

    if (matrix_is_null(matrix))
    {
        printf("[delete_columns]: matrix is null\n");
        return NULL;
    }

    if (start > matrix->m - 1 || end > matrix->m - 1)
    {
        printf("[delete_columns]: received invalid startColumn or endColumn\n");
        return NULL;
    }

    size_t columns = matrix->m - (end - start + 1);
    Matrix *result = allocate_matrix(matrix->n, columns);

    for (size_t i = 0; i < matrix->n; i++)
    {
        for (size_t j = 0; j < start; j++)
            result->matrix[i][j] = matrix->matrix[i][j];

        for (size_t j = end + 1; j < matrix->m; j++)
            result->matrix[i][j - end] = matrix->matrix[i][j];
    }

    return result;
}

Matrix *crop(Matrix *matrix, size_t startLine, size_t endLine, size_t startColumn, size_t endColumn)
{
    Matrix *temp = NULL;
    Matrix *result = NULL;

    temp = delete_lines(matrix, startLine, endLine);

    // printf("intermediare:\n");
    // print_matrix(temp);

    result = delete_columns(temp, startColumn, endColumn);

    destroy_matrix(temp);

    return result;
}

Matrix *select_lines(Matrix *matrix, size_t start, size_t end)
{
    // check that start < end

    if (matrix_is_null(matrix))
    {
        printf("[select_lines]: matrix is null\n");
        return NULL;
    }

    if (start > end)
    {
        printf("[select_lines]: start is bigger than end\n");
        return NULL;
    }

    if (start > matrix->n - 1 || end > matrix->n - 1)
    {
        printf("[select_lines]: received invalid startLine or endLine\n");
        return NULL;
    }

    size_t lines = end - start + 1;
    Matrix *result = allocate_matrix(lines, matrix->m);

    for (size_t i = start; i <= end; i++)
        for (size_t j = 0; j < matrix->m; j++)
            result->matrix[i - start][j] = matrix->matrix[i][j];

    return result;
}

Matrix *select_columns(Matrix *matrix, size_t start, size_t end)
{
    // check that start < end

    if (matrix_is_null(matrix))
    {
        printf("[select_columns]: matrix is null\n");
        return NULL;
    }

    if (start > end)
    {
        printf("[select_lines]: start is bigger than end\n");
        return NULL;
    }

    if (start > matrix->m - 1 || end > matrix->m - 1)
    {
        printf("[select_columns]: received invalid startColumn or endColumn\n");
        return NULL;
    }

    size_t columns = end - start + 1;
    Matrix *result = allocate_matrix(matrix->n, columns);

    for (size_t i = 0; i < matrix->n; i++)
        for (size_t j = start; j <= end; j++)
            result->matrix[i][j - start] = matrix->matrix[i][j];

    return result;
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

Matrix *select_(Matrix *matrix, size_t startLine, size_t endLine, size_t startColumn, size_t endColumn)
{
    Matrix *temp = NULL;
    Matrix *result = NULL;

    temp = select_lines(matrix, startLine, endLine);

    result = select_columns(temp, startColumn, endColumn);

    destroy_matrix(temp);

    return result;
}

bool matrix_is_square(Matrix *matrix)
{
    return matrix->n == matrix->m;
}

bool matrix_is_not_null(Matrix *matrix)
{
    return matrix != NULL;
}

bool matrix_is_null(Matrix *matrix)
{
    return matrix == NULL;
}

Matrix *permute_lines(Matrix *matrix, size_t L1, size_t L2, bool destructive)
{
    float temp;
    Matrix *result;

    if (matrix_is_null(matrix))
    {
        printf("[permute_lines]: matrix is null\n");
        return NULL;
    }

    if (L1 > matrix->n - 1 || L2 > matrix->n - 1)
    {
        printf("[permute_lines]: provided an invalid line (L1 or L2)\n");
        return NULL;
    }

    result = destructive ? matrix : copy_matrix(matrix);

    for (size_t j = 0; j < matrix->m; j++)
    {
        temp = result->matrix[L1][j];
        result->matrix[L1][j] = result->matrix[L2][j];
        result->matrix[L2][j] = temp;
    }

    return result;
}

Matrix *permute_columns(Matrix *matrix, size_t C1, size_t C2, bool destructive)
{
    float temp;
    Matrix *result;

    if (matrix_is_null(matrix))
    {
        printf("[permute_columns]: matrix is null\n");
        return NULL;
    }

    if (C1 > matrix->m - 1 || C2 > matrix->m - 1)
    {
        printf("[permute_columns]: provided an invalid column (C1 or C2)\n");
        return NULL;
    }

    result = destructive ? matrix : copy_matrix(matrix);

    for (size_t i = 0; i < matrix->n; i++)
    {
        temp = result->matrix[i][C1];
        result->matrix[i][C1] = result->matrix[i][C2];
        result->matrix[i][C2] = temp;
    }

    return result;
}

void matrix_max(Matrix *matrix, MatrixType type, size_t *maxI, size_t *maxJ, bool absolute)
{
    size_t mI, mJ;

    if (matrix_is_null(matrix))
    {
        printf("[matrix_max]: matrix is null\n");
        return;
    }

    // (0, 0) is always present in all types
    mI = 0;
    mJ = 0;

    matrix_for_each(matrix, type, [&absolute, &matrix, &mI, &mJ](size_t i, size_t j, float element) -> void
                    { 
                        if (absolute) {
                            if (abs(element) > abs(matrix->matrix[mI][mJ])) {
                                mI = i;
                                mJ = j;
                            }
                        } else {
                                if (element > matrix->matrix[mI][mJ]) {
                                mI = i;
                                mJ = j;
                            }
                        } });

    *maxI = mI;
    *maxJ = mJ;
}

size_t matrix_max_line(Matrix *matrix, size_t line, bool abs)
{
    size_t mJ;

    if (matrix_is_null(matrix))
    {
        printf("[matrix_max_line]: matrix is null\n");
        return 0;
    }

    if (line > matrix->n - 1)
    {
        printf("[matrix_max_line]: provided an invalid line\n");
        return 0;
    }

    mJ = 0;

    matrix_for_each_line(matrix, line, [&matrix, &line, &mJ](size_t i, size_t j, float element) -> void
                         {
        if (element > matrix->matrix[line][mJ])
            mJ = j; });

    return mJ;
}

size_t matrix_max_column(Matrix *matrix, size_t column, bool absolute)
{
    size_t mI;

    if (matrix_is_null(matrix))
    {
        printf("[matrix_max_column]: matrix is null\n");
        return 0;
    }

    if (column > matrix->m - 1)
    {
        printf("[matrix_max_column]: provided an invalid column\n");
        return 0;
    }

    mI = 0;

    matrix_for_each_column(matrix, column, [&matrix, &absolute, &column, &mI](size_t i, size_t j, float element) -> void
                           {
                            if (!absolute)
                            {
                                if (element > matrix->matrix[mI][column])
                                    mI = i;
                            } else {
                                if (abs(element) > abs(matrix->matrix[mI][column]))
                                    mI = i;
                            } });

    return mI;
}

//--//

void matrix_min(Matrix *matrix, MatrixType type, size_t *minI, size_t *minJ, bool abs)
{
    size_t mI, mJ;

    if (matrix_is_null(matrix))
    {
        printf("[matrix_min]: matrix is null\n");
        return;
    }

    mI = 0;
    mJ = 0;

    matrix_for_each(matrix, type, [&matrix, &mI, &mJ](size_t i, size_t j, float element) -> void
                    { if (element < matrix->matrix[mI][mJ]) {
                mI = i;
                mJ = j;
            } });

    *minI = mI;
    *minJ = mJ;
}

size_t matrix_min_line(Matrix *matrix, size_t line, bool abs)
{
    size_t mJ;

    if (matrix_is_null(matrix))
    {
        printf("[matrix_min_line]: matrix is null\n");
        return 0;
    }

    if (line > matrix->n - 1)
    {
        printf("[matrix_min_line]: provided an invalid line\n");
        return 0;
    }

    mJ = 0;

    matrix_for_each_line(matrix, line, [&matrix, &line, &mJ](size_t i, size_t j, float element) -> void
                         {
        if (element < matrix->matrix[line][mJ])
            mJ = j; });

    return mJ;
}

size_t matrix_min_column(Matrix *matrix, size_t column, bool abs)
{
    size_t mI;

    if (matrix_is_null(matrix))
    {
        printf("[matrix_min_column]: matrix is null\n");
        return 0;
    }

    if (column > matrix->m - 1)
    {
        printf("[matrix_min_column]: provided an invalid column\n");
        return 0;
    }

    mI = 0;

    matrix_for_each_column(matrix, column, [&matrix, &column, &mI](size_t i, size_t j, float element) -> void
                           {
        if (element < matrix->matrix[mI][column])
            mI = i; });

    return mI;
}