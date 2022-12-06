#include <math.h>

#include "sle.hpp"

Sle::Sle()
{
    this->order = 0;
    this->matrix = *(new MatrixClass());
    this->vector = *(new MatrixClass());
}

Sle::Sle(std::size_t order)
{
    this->order = order;
    this->matrix = *(new MatrixClass(order, order));
    this->vector = *(new MatrixClass(order, 1));
}

Sle::Sle(MatrixClass &matrix, MatrixClass &vector)
{
    // Todo:
    //      check that matrix is square
    //      check that vector have the same number of lines as matrix
    //      check that vector have only one column

    this->order = matrix.size().first;
    this->matrix = matrix;
    this->vector = vector;
}

Sle::~Sle()
{
    printf("[Sle]: one got abandonned\n");
}

void Sle::print()
{
    MatrixClass::print_matrices_concatenation(2, this->matrix, this->vector);
}

Sle &Sle::create_power_system(std::size_t order)
{
    Sle *result;
    MatrixClass *matrix = new MatrixClass(order, order);
    MatrixClass *vector = new MatrixClass(order, 1);

    for (size_t i = 0; i < order; i++)
    {
        for (size_t j = 0; j < order; j++)
        {
            (*matrix)[i][j] = std::pow(i + 1, j + 1);
            (*vector)[i][0] += (*matrix)[i][j];
        }
    }

    result = new Sle(*matrix, *vector);

    return *result;
}

Sle &Sle::create_hilbert_system(std::size_t order)
{
    Sle *result;
    MatrixClass *matrix = new MatrixClass(order, order);
    MatrixClass *vector = new MatrixClass(order, 1);

    for (size_t i = 0; i < order; i++)
    {
        for (size_t j = 0; j < order; j++)
        {
            (*matrix)[i][j] = 1 / ((i + 1) + (j + 1) - 1);
            (*vector)[i][0] += (*matrix)[i][j];
        }
    }

    result = new Sle(*matrix, *vector);

    return *result;
}

Sle &Sle::gauss()
{
    // check that it can work, and that pivots aren't null

    // always check that the system isn't empty

    for (size_t k = 0; k < this->order - 1; k++)
    {
        float a = this->matrix[k][k];
        for (size_t i = k + 1; i < order; i++)
        {
            float b = this->matrix[i][k];
            for (size_t j = k; j < order; j++)
            {
                this->matrix[i][j] -= ((b / a) * this->matrix[k][j]);
            }
            this->vector[i][0] -= ((b / a) * this->vector[k][0]);
        }
    }
}

Sle &Sle::gauss_total()
{
    MatrixClass *reference = &MatrixClass::create_matrix_with(NORMAL, this->order, 2, -1);

    for (size_t k = 0; k < this->order - 1; k++)
    {
        // getting the maximum position
        // size_t maxI = k, maxJ = k;
        // search for maximum in this column to choose the right pivot

        // search max in the matrix or sub matrix ? here you are only doing it in the column / like gauss_partielle
        size_t maxI = k; // matrix_max_column(matrix, k, true);
        size_t maxJ = k;

        for (size_t i = k; i < this->order; i++)
        {
            for (size_t j = k; j < this->order; j++)
            {
                if (abs(this->matrix[i][j]) > abs(this->matrix[maxI][maxJ]))
                {
                    maxI = i;
                    maxJ = j;
                }
            }
        }

        // we permute the lines k and maxI

        if (maxI != k)
        {
            matrix = this->matrix.permute_lines(k, maxI, true);
            vector = this->vector.permute_lines(k, maxI, true);
            // printf("permute between line %ld and %ld\n", k, maxI);
        }

        // we permut the columns k and maxJ

        if (maxJ != k)
        {
            matrix = this->matrix.permute_columns(k, maxJ, true);
            (*reference)[k][0] = k;
            (*reference)[k][1] = maxJ;
            // printf("permute between column %ld and %ld\n", k, maxJ);
            //  here => we save the permutations we have done
        }

        // printf("before step %ld\n", k);
        // print_matrices_concatenation(2, matrix, vector);

        float a = this->matrix[k][k];
        for (size_t i = k + 1; i < this->order; i++)
        {
            float b = this->matrix[i][k];
            for (size_t j = k; j < this->order; j++)
            {
                this->matrix[i][j] -= ((b / a) * this->matrix[k][j]);
            }
            this->vector[i][0] -= ((b / a) * this->vector[k][0]);
        }

        printf("k=%ld\n", k);
        MatrixClass::print_matrices_concatenation(2, matrix, vector);
        printf("\n");
    }
}

Sle &Sle::gauss_partial()
{
    // check it's a square matrix

    for (size_t k = 0; k < this->order; k++) // loop to go through each diagonal element (pivot)
    {
        // search for maximum in this column (column k) to choose the right pivot
        size_t maxI = k; // matrix_max_column(matrix, k, true);
        size_t maxJ = k;

        // on recherche le max depuis la ligne 0 ou depuis la ligne k ?
        for (size_t i = k; i < this->order; i++)
        {
            if (abs(this->matrix[i][k]) > abs(this->matrix[maxI][maxJ]))
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
            matrix = this->matrix.permute_lines(k, maxI, true);
            vector = this->vector.permute_lines(k, maxI, true);
            // printf("permute between line %ld and %ld\n", k, maxI);
        }

        // printf("before step %ld\n", k);
        // print_matrices_concatenation(2, matrix, vector);
        float a = this->matrix[k][k];
        for (size_t i = k + 1; i < this->order; i++) // loop to go through each line under that diagonal element and then delete (make zeros appear)
        {
            float b = this->matrix[i][k];
            for (size_t j = 0; j < this->order; j++)
            {
                this->matrix[i][j] -= ((b / a) * this->matrix[k][j]);
            }
            // do stuff inorder to delete (make it 0) the element ([i][k])
            this->vector[i][0] -= ((b / a) * this->vector[k][0]);
        }

        // printf("after step %ld\n", k);
        // print_matrices_concatenation(2, matrix, vector);
    }
}

MatrixClass &Sle::solve_cramer()
{
    float determinent = this->matrix.determinent();

    if (determinent == 0)
    {
        printf("[solve_with_cramer]: determinant is equal to 0\n");
        return (*new MatrixClass());
    }

    MatrixClass *solutions = new MatrixClass(this->order, 1);

    for (size_t i = 0; i < this->order; i++)
    {
        MatrixClass temp = MatrixClass::copy_matrix_in(matrix, vector, 0, i);
        (*solutions)[i][0] = temp.determinent() / determinent;
        // Todo: make sure the matrice has been destroyed
        // destroy_matrix(temp);
    }

    return *solutions;
}

MatrixClass &Sle::solve_upper_triangular()
{
    float det = this->matrix.determinent();

    if (det == 0)
    {
        printf("[solve_upper_triangular]: determinant is equal to 0\n");
        return (*new MatrixClass());
    }

    MatrixClass *solutions = new MatrixClass(this->order, 1);

    for (int i = this->order - 1; i >= 0; i--)
    {
        float sum = 0;
        for (size_t j = i + 1; j < this->order; j++)
        {
            sum += this->matrix[i][j] * (*solutions)[j][0];
        }
        (*solutions)[i][0] = (this->vector[i][0] - sum) / this->matrix[i][i];
    }

    return *solutions;
}

MatrixClass &Sle::solve_lower_triangular()
{
    float det = this->matrix.determinent();

    if (det == 0)
    {
        printf("[solve_lower_triangular]: determinant is equal to 0\n");
        return (*new MatrixClass());
    }

    MatrixClass *solutions = new MatrixClass(this->order, 1);

    for (size_t i = 0; i < order; i++)
    {
        float sum = 0;
        for (size_t j = 0; j < i; j++)
        {
            sum += this->matrix[i][j] * (*solutions)[j][0];
        }
        (*solutions)[i][0] = (this->vector[i][0] - sum) / this->matrix[i][i];
    }
}

std::ostream &operator<<(std::ostream &os, Sle &sle)
{
    MatrixClass::print_matrices_concatenation(2, &sle.matrix, &sle.vector);

    return os;
}