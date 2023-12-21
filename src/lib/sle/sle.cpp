#include <math.h>

#include "sle.hpp"
#include "../util/util.hpp"
#include "../matrix/matrix.hpp"

template <typename T>
void Sle<T>::set_matrix(MatrixClass<T> *matrix)
{
    // check that the order is the same
    this->matrix = matrix != NULL ? matrix->copy() : NULL;
}

template <typename T>
void Sle<T>::set_vector(MatrixClass<T> *vector)
{
    this->vector = vector != NULL ? vector->copy() : NULL;
}

template <typename T>
const std::string &Sle<T>::get_name()
{
    return this->name;
}

template <typename T>
std::size_t Sle<T>::get_order()
{
    return this->order;
}

template <typename T>
Sle<T> *Sle<T>::creaate_system(std::size_t order)
{
    return new Sle<T>(order);
}

template <typename T>
Sle<T> *Sle<T>::creaate_system(MatrixClass<T> *matrix, MatrixClass<T> *vector)
{
    return new Sle<T>(matrix, vector);
}

template <typename T>
Sle<T>::Sle()
{
    this->order = 0;
    this->matrix = NULL;
    this->vector = NULL;

    if (Sle<T>::is_debug_option_set(SystemDebug::SYSTEM_CREATION))
        printf(COLOR_GREEN "[Sle<T>]:" COLOR_RESET "empty one got created\n");
}

template <typename T>
Sle<T>::Sle(std::size_t order)
{
    this->order = order;
    this->matrix = MatrixClass<T>::create_matrix(order, order)->set_name("matrix");
    this->vector = MatrixClass<T>::create_matrix(order, 1)->set_name("vector");

    if (Sle<T>::is_debug_option_set(SystemDebug::SYSTEM_CREATION))
        printf(COLOR_GREEN "[Sle<T>]:" COLOR_RESET "one got created\n");
}

template <typename T>
void Sle<T>::destroy()
{
    delete this;
}

template <typename T>
Sle<T>::Sle(MatrixClass<T> *matrix, MatrixClass<T> *vector)
{
    if (matrix == NULL || vector == NULL || vector->is_null() || matrix->is_null() || !matrix->is_square() || vector->size().first != matrix->size().first || vector->size().second != 1)
    {
        if (Sle<T>::is_debug_option_set(SystemDebug::SYSTEM_MISC))
            printf(COLOR_RED "[Sle<T>]:" COLOR_RESET "error while creating sle<T> with given matrice and vector\n");
        this->set_matrix(NULL);
        this->set_vector(NULL);
        return;
    }

    this->order = matrix->size().first;

    this->set_matrix(matrix);
    this->set_vector(vector);

    if (Sle<T>::is_debug_option_set(SystemDebug::SYSTEM_CREATION))
        printf(COLOR_GREEN "[Sle<T>]:" COLOR_RESET "one got created\n");
}

template <typename T>
Sle<T>::~Sle<T>()
{
    if (Sle<T>::is_debug_option_set(SystemDebug::SYSTEM_DESTRUCTION))
        printf(COLOR_RED "[~Sle<T>]:" COLOR_RESET "(%s) got destroyed\n", this->name.length() == 0 ? "/" : this->name.c_str());

    this->matrix->destroy();
    this->vector->destroy();
}

template <typename T>
void Sle<T>::set_debug_options(SystemDebug debug)
{
    Sle<T>::debug_options = debug;
}

template <typename T>
bool Sle<T>::is_debug_option_set(SystemDebug debug_option)
{
    return (Sle<T>::debug_options & debug_option) == debug_option;
}

template <typename T>
void Sle<T>::srand(unsigned int seed)
{
    Sle<T>::seed = seed;
}

template <typename T>
Sle<T> *Sle<T>::set_name(const std::string &name)
{
    this->name = name;

    return this;
}

template <typename T>
Sle<T> *Sle<T>::copy()
{
    MatrixClass<T> *matrix = this->matrix->copy();
    MatrixClass<T> *vector = this->vector->copy();

    return new Sle<T>(matrix, vector);
}

template <typename T>
Sle<T> *Sle<T>::create_random_int_system(std::size_t order, int min, int max)
{
    MatrixClass<T> *matrix = MatrixClass<T>::create_matrix_random_int(MatrixType::NORMAL, order, order, min, max);
    MatrixClass<T> *vector = MatrixClass<T>::create_matrix_random_int(MatrixType::NORMAL, order, 1, min, max);

    return new Sle<T>(matrix, vector);
}

template <typename T>
Sle<T> *Sle<T>::create_random_float_system(std::size_t order, float min, float max)
{
    MatrixClass<T> *matrix = MatrixClass<T>::create_matrix_random_float(MatrixType::NORMAL, order, order, min, max);
    MatrixClass<T> *vector = MatrixClass<T>::create_matrix_random_float(MatrixType::NORMAL, order, 1, min, max);

    return new Sle<T>(matrix, vector);
}

template <typename T>
void Sle<T>::print()
{
    if (this->name.length() > 0)
        std::cout << this->name + ":" << std::endl;

    MatrixClass<T>::print_matrices_concatenation(2, this->matrix, this->vector);
}

template <typename T>
Sle<T> *Sle<T>::create_power_system(std::size_t order)
{
    MatrixClass<T> *matrix = MatrixClass<T>::create_matrix(order, order)->set_name("power-system-matrix");
    MatrixClass<T> *vector = MatrixClass<T>::create_matrix(order, 1)->set_name("power-system-vector");

    for (size_t i = 0; i < order; i++)
    {
        for (size_t j = 0; j < order; j++)
        {
            (*matrix)[i][j] = std::pow(i + 1, j + 1);
            (*vector)[i][0] += (*matrix)[i][j];
        }
    }

    return new Sle<T>(matrix, vector);
}

template <typename T>
Sle<T> *Sle<T>::create_hilbert_system(std::size_t order)
{
    MatrixClass<T> *matrix = MatrixClass<T>::create_matrix(order, order)->set_name("hilbert-system-matrix");
    MatrixClass<T> *vector = MatrixClass<T>::create_matrix(order, 1)->set_name("hilbert-system-vector");

    for (size_t i = 0; i < order; i++)
    {
        for (size_t j = 0; j < order; j++)
        {
            (*matrix)[i][j] = 1 / ((i + 1) + (j + 1) - 1);
            (*vector)[i][0] += (*matrix)[i][j];
        }
    }

    return new Sle<T>(matrix, vector);
}

// old:
/*
    template <typename T>
Sle<T> *Sle<T>::gauss_jordan(bool inplace)
{
    // check that it can work, and that pivots aren't null

    // always check that the system isn't empty

    Sle<T> *result = inplace ? this : this->copy();

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
*/

template <typename T>
Sle<T> *Sle<T>::gauss_jordan(bool inplace)
{
    // check that it can work, and that pivots aren't null

    // always check that the system isn't empty

    Sle<T> *result = inplace ? this : this->copy();

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

template <typename T>
Sle<T> *Sle<T>::gauss(bool inplace)
{
    // check that it can work, and that pivots aren't null

    // always check that the system isn't empty

    Sle<T> *result = inplace ? this : this->copy();

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
template <typename T>

Sle<T> *Sle<T>::gauss_total(bool inplace)
{
    Sle<T> *result = inplace ? this : this->copy();

    MatrixClass<T> *reference = MatrixClass<T>::create_matrix_with(NORMAL, this->order, 2, -1);

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
        MatrixClass<T>::print_matrices_concatenation(2, matrix, vector);
        printf("\n");
    }

    return result;
}

template <typename T>
Sle<T> *Sle<T>::gauss_partial(bool inplace)
{
    Sle<T> *result = inplace ? this : this->copy();

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

template <typename T>
MatrixClass<T> *Sle<T>::solve_cramer()
{
    float determinent = this->matrix->determinent();

    if (determinent == 0)
    {
        printf("[solve_with_cramer]: determinant is equal to 0\n");
        return NULL;
    }

    MatrixClass<T> *solutions = MatrixClass<T>::create_matrix(this->order, 1);

    for (size_t i = 0; i < this->order; i++)
    {
        MatrixClass<T> *temp = MatrixClass<T>::copy_matrix_in(matrix, vector, 0, i);
        (*solutions)[i][0] = temp->determinent() / determinent;
        // Todo: make sure the matrice has been destroyed
        temp->destroy();
        // destroy_matrix(temp);
    }

    return solutions;
}

template <typename T>
MatrixClass<T> *Sle<T>::solve_upper_triangular()
{
    float det = this->matrix->determinent();

    if (det == 0)
    {
        printf("[solve_upper_triangular]: determinant is equal to 0\n");
        return NULL;
    }

    MatrixClass<T> *solutions = MatrixClass<T>::create_matrix(this->order, 1);

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

template <typename T>
MatrixClass<T> *Sle<T>::solve_lower_triangular()
{
    float det = this->matrix->determinent();

    if (det == 0)
    {
        printf("[solve_lower_triangular]: determinant is equal to 0\n");
        return NULL;
    }

    MatrixClass<T> *solutions = MatrixClass<T>::create_matrix(this->order, 1);

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

template <typename T>
std::ostream &operator<<(std::ostream &os, Sle<T> &sle)
{
    sle.print();

    return os;
}