#include <math.h>

#include "sle.hpp"

Sle::Sle()
{
    this->order = 0;
    this->matrix = (new MatrixClass())->set_name("null-matrix");
    this->vector = (new MatrixClass())->set_name("null-vector");

    printf("[Sle]: empty one got created\n");
}

Sle::Sle(std::size_t order)
{
    this->order = order;
    this->matrix = (new MatrixClass(order, order))->set_name("matrix");
    this->vector = (new MatrixClass(order, 1))->set_name("vector");

    printf("[Sle]: got created\n");
}

Sle::Sle(MatrixClass *matrix, MatrixClass *vector)
{
    // Todo:
    //      check that matrix is square
    //      check that vector have the same number of lines as matrix
    //      check that vector have only one column

    this->order = matrix->size().first;

    this->matrix = matrix->copy();
    this->vector = vector->copy();

    printf("[Sle]: one got created\n");
}

Sle::~Sle()
{
    printf("[~Sle]: one got abandonned\n");

    delete this->matrix;
    delete this->vector;
}

Sle *Sle::copy()
{
    MatrixClass *matrix = this->matrix->copy();
    MatrixClass *vector = this->vector->copy();

    return new Sle(matrix, vector);
}

Sle *Sle::create_random_int_system(std::size_t order, int min, int max)
{
    Sle *result;
    MatrixClass *matrix = MatrixClass::create_matrix_random_int(MatrixType::NORMAL, order, order, min, max);
    MatrixClass *vector = MatrixClass::create_matrix_random_int(MatrixType::NORMAL, order, 1, min, max);

    result = new Sle(matrix, vector);

    return result;
}

Sle *Sle::create_random_float_system(std::size_t order, float min, float max)
{
    Sle *result;
    MatrixClass *matrix = MatrixClass::create_matrix_random_float(MatrixType::NORMAL, order, order, min, max);
    MatrixClass *vector = MatrixClass::create_matrix_random_float(MatrixType::NORMAL, order, 1, min, max);

    result = new Sle(matrix, vector);

    return result;
}

void Sle::print()
{
    MatrixClass::print_matrices_concatenation(2, this->matrix, this->vector);
}

Sle *Sle::create_power_system(std::size_t order)
{
    Sle *result;

    MatrixClass *matrix = (new MatrixClass(order, order))->set_name("power-system-matrix");
    MatrixClass *vector = (new MatrixClass(order, 1))->set_name("power-system-vector");

    for (size_t i = 0; i < order; i++)
    {
        for (size_t j = 0; j < order; j++)
        {
            (*matrix)[i][j] = std::pow(i + 1, j + 1);
            (*vector)[i][0] += (*matrix)[i][j];
        }
    }

    result = new Sle(matrix, vector);

    return result;
}

Sle *Sle::create_hilbert_system(std::size_t order)
{
    Sle *result;
    MatrixClass *matrix = (new MatrixClass(order, order))->set_name("hilbert-system-matrix");
    MatrixClass *vector = (new MatrixClass(order, 1))->set_name("hilbert-system-vector");

    for (size_t i = 0; i < order; i++)
    {
        for (size_t j = 0; j < order; j++)
        {
            (*matrix)[i][j] = 1 / ((i + 1) + (j + 1) - 1);
            (*vector)[i][0] += (*matrix)[i][j];
        }
    }

    result = new Sle(matrix, vector);

    return result;
}

Sle *Sle::gauss_jordan(bool inplace)
{
    // check that it can work, and that pivots aren't null

    // always check that the system isn't empty

    Sle *result = inplace ? this : this->copy();

    for (size_t k = 0; k < result->order; k++)
    {
        float a = result->matrix->get(k, k);
        for (size_t i = k + 1; i < order; i++)
        {
            float b = result->matrix->get(i, k);
            for (size_t j = k; j < order; j++)
            {
                (*result->matrix)[i][j] -= ((b / a) * result->matrix->get(k, j));
            }
            (*result->vector)[i][0] -= ((b / a) * result->vector->get(k, 0));
        }

        float temp = 1 / result->matrix->get(k, k);

        result->matrix->map_line(
            k, [temp](std::size_t i, std::size_t j, float element) -> float
            { return element * temp; },
            inplace);
        (*result->vector)[k][0] *= temp;
    }

    for (int k = this->order - 1; k >= 0; k--)
    {
        float a = result->matrix->get(k, k);
        for (int i = k - 1; i >= 0; i--)
        {
            float b = result->matrix->get(i, k);
            for (size_t j = 0; j < this->order; j++)
            {
                (*result->matrix)[i][j] -= ((b / a) * result->matrix->get(k, j));
            }
            (*result->vector)[i][0] -= ((b / a) * result->vector->get(k, 0));
        }
    }

    return result;
}

Sle *Sle::gauss(bool inplace)
{
    // check that it can work, and that pivots aren't null

    // always check that the system isn't empty

    Sle *result = inplace ? this : this->copy();

    for (size_t k = 0; k < result->order - 1; k++)
    {
        float a = result->matrix->get(k, k);
        for (size_t i = k + 1; i < order; i++)
        {
            float b = result->matrix->get(i, k);
            for (size_t j = k; j < order; j++)
            {
                (*result->matrix)[i][j] -= ((b / a) * result->matrix->get(k, j));
            }
            (*result->vector)[i][0] -= ((b / a) * result->vector->get(k, 0));
        }
    }

    return result;
}

Sle *Sle::gauss_total(bool inplace)
{
    Sle *result = inplace ? this : this->copy();

    MatrixClass *reference = MatrixClass::create_matrix_with(NORMAL, this->order, 2, -1);

    for (size_t k = 0; k < result->order - 1; k++)
    {
        // getting the maximum position
        // size_t maxI = k, maxJ = k;
        // search for maximum in result column to choose the right pivot

        // search max in the matrix or sub matrix ? here you are only doing it in the column / like gauss_partielle
        size_t maxI = k; // matrix_max_column(matrix, k, true);
        size_t maxJ = k;

        for (size_t i = k; i < result->order; i++)
        {
            for (size_t j = k; j < result->order; j++)
            {
                if (abs(result->matrix->get(i, j)) > abs(result->matrix->get(maxI, maxJ)))
                {
                    maxI = i;
                    maxJ = j;
                }
            }
        }

        // we permute the lines k and maxI

        if (maxI != k)
        {
            matrix = result->matrix->permute_lines(k, maxI, true);
            vector = result->vector->permute_lines(k, maxI, true);
            // printf("permute between line %ld and %ld\n", k, maxI);
        }

        // we permut the columns k and maxJ

        if (maxJ != k)
        {
            matrix = result->matrix->permute_columns(k, maxJ, true);
            (*reference)[k][0] = k;
            (*reference)[k][1] = maxJ;
            // printf("permute between column %ld and %ld\n", k, maxJ);
            //  here => we save the permutations we have done
        }

        // printf("before step %ld\n", k);
        // print_matrices_concatenation(2, matrix, vector);

        float a = result->matrix->get(k, k);
        for (size_t i = k + 1; i < result->order; i++)
        {
            float b = result->matrix->get(i, k);
            for (size_t j = k; j < result->order; j++)
            {
                (*result->matrix)[i][j] -= ((b / a) * result->matrix->get(k, j));
            }
            (*result->vector)[i][0] -= ((b / a) * result->vector->get(k, 0));
        }

        printf("k=%ld\n", k);
        MatrixClass::print_matrices_concatenation(2, matrix, vector);
        printf("\n");
    }

    return result;
}

Sle *Sle::gauss_partial(bool inplace)
{
    Sle *result = inplace ? this : this->copy();

    // check it's a square matrix

    for (size_t k = 0; k < result->order; k++) // loop to go through each diagonal element (pivot)
    {
        // search for maximum in result column (column k) to choose the right pivot
        size_t maxI = k; // matrix_max_column(matrix, k, true);
        size_t maxJ = k;

        // on recherche le max depuis la ligne 0 ou depuis la ligne k ?
        for (size_t i = k; i < result->order; i++)
        {
            if (abs(result->matrix->get(i, k)) > abs(result->matrix->get(maxI, maxJ)))
            {
                maxI = i;
                maxJ = k;
            }
        }

        // printf("max: %lu\n", maxI);

        // here maxI and maxJ store the greatest element in result column, so we permute between the line k and the line of result element

        // matrix = permute_lines(matrix, k, maxI);

        // permute between line k and line maxI
        if (maxI != k)
        {
            matrix = result->matrix->permute_lines(k, maxI, true);
            vector = result->vector->permute_lines(k, maxI, true);
            // printf("permute between line %ld and %ld\n", k, maxI);
        }

        // printf("before step %ld\n", k);
        // print_matrices_concatenation(2, matrix, vector);
        float a = result->matrix->get(k, k);
        for (size_t i = k + 1; i < result->order; i++) // loop to go through each line under that diagonal element and then delete (make zeros appear)
        {
            float b = result->matrix->get(i, k);
            for (size_t j = 0; j < result->order; j++)
            {
                (*result->matrix)[i][j] -= ((b / a) * result->matrix->get(k, j));
            }
            // do stuff inorder to delete (make it 0) the element ([i][k])
            (*result->vector)[i][0] -= ((b / a) * result->vector->get(k, 0));
        }

        // printf("after step %ld\n", k);
        // print_matrices_concatenation(2, matrix, vector);
    }

    return result;
}

MatrixClass *Sle::solve_cramer()
{
    float determinent = this->matrix->determinent();

    if (determinent == 0)
    {
        printf("[solve_with_cramer]: determinant is equal to 0\n");
        return new MatrixClass();
    }

    MatrixClass *solutions = new MatrixClass(this->order, 1);

    for (size_t i = 0; i < this->order; i++)
    {
        MatrixClass *temp = MatrixClass::copy_matrix_in(matrix, vector, 0, i);
        (*solutions)[i][0] = temp->determinent() / determinent;
        // Todo: make sure the matrice has been destroyed
        delete temp;
        // destroy_matrix(temp);
    }

    return solutions;
}

MatrixClass *Sle::solve_upper_triangular()
{
    float det = this->matrix->determinent();

    if (det == 0)
    {
        printf("[solve_upper_triangular]: determinant is equal to 0\n");
        return new MatrixClass();
    }

    MatrixClass *solutions = new MatrixClass(this->order, 1);

    for (int i = this->order - 1; i >= 0; i--)
    {
        float sum = 0;
        for (size_t j = i + 1; j < this->order; j++)
        {
            sum += this->matrix->get(i, j) * (*solutions)[j][0];
        }
        (*solutions)[i][0] = (this->vector->get(i, 0) - sum) / this->matrix->get(i, i);
    }

    return solutions;
}

MatrixClass *Sle::solve_lower_triangular()
{
    float det = this->matrix->determinent();

    if (det == 0)
    {
        printf("[solve_lower_triangular]: determinant is equal to 0\n");
        return new MatrixClass();
    }

    MatrixClass *solutions = new MatrixClass(this->order, 1);

    for (size_t i = 0; i < order; i++)
    {
        float sum = 0;
        for (size_t j = 0; j < i; j++)
        {
            sum += this->matrix->get(i, j) * (*solutions)[j][0];
        }
        (*solutions)[i][0] = (this->vector->get(i, 0) - sum) / this->matrix->get(i, i);
    }

    return solutions;
}

std::ostream &operator<<(std::ostream &os, Sle &sle)
{
    sle.print();

    return os;
}