#ifndef __PL_HPP__
#define __PL_HPP__

#include <iostream>
#include <type_traits>

#include "../matrix/matrix.hpp"
#include "../matrix/matrix-value.hpp"

#include "./matrix-pl-constraint-type-value.hpp"

#define MAXIMUM_PLS 64

// enum PLConstraintType
// {
//     BIGGER = -2,
//     BIGGER_OR_EQUAL = -1,
//     EQUAL = 0,
//     LOWER = 1,
//     LOWER_OR_EQUAL = 2,
// };
// // typedef enum PLConstraintType PLConstraintType;

template <typename T>
class PL
{
    static_assert(std::is_base_of<MatrixValue, T>{}, "Must derive from MatrixValue");

private:
    static size_t *created_pls_length;
    static PL *all_created_pls[MAXIMUM_PLS];

    MatrixClass<T> *matrixC;
    MatrixClass<T> *matrixA;

    // constraints type
    MatrixClass<MatrixPLConstraintTypeValue> *matrixAB;

    MatrixClass<T> *matrixB;

    static wchar_t get_subscript_character(unsigned int number);
    static void print_variable(const char *name = "x", unsigned int number = 0);

    PL();
    PL(MatrixClass<T> *matrixA, MatrixClass<MatrixPLConstraintTypeValue> *matrixAB, MatrixClass<T> *matrixB, MatrixClass<T> *matrixC);
    ~PL();

public:
    static PL *create_pl(size_t constraints_len, size_t variables_len);
    static PL *create_pl_from(size_t constraints_len, size_t variables_len);
    static PL *create_pl_random(size_t constraints_len, size_t variables_len);

    void print();
    void print_objective_function();
    void print_constraint(size_t constraint_index);

    void destroy();
};

/**
 * pl-class
 *
 * receive objective function coefficients matrix
 * receive constraints coefficients matrix
 * receive second member coefficients matrix
 *
 * forme transformer classes
 *
 */

/**
 * dictionarry-class
 */

/**
 * simplex-class
 */

/**
 * simplex_duall-class
 */

#endif