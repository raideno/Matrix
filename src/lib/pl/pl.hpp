#ifndef __PL_HPP__
#define __PL_HPP__

#include <vector>
#include <iostream>
#include <type_traits>

#include "../matrix/matrix.hpp"
#include "../matrix/matrix-value.hpp"

#include "./matrix-pl-constraint-type-value.hpp"

// enum PLConstraintType
// {
//     BIGGER = -2,
//     BIGGER_OR_EQUAL = -1,
//     EQUAL = 0,
//     LOWER = 1,
//     LOWER_OR_EQUAL = 2,
// };
// // typedef enum PLConstraintType PLConstraintType;

enum PLForms
{
    MIXED_CANONICAL,    // truc par defaut
    CANONICAL_PURE,     // all constraints are lower-or-equal (<=)
    STANDAR,            // all constraints are equal (=)
    SIMPLIFIED_STANDAR, // standar + matrix can be decomposed to A = (I | H)
};
typedef enum PLForms PLForms;

template <typename T>
class PL
{
    static_assert(std::is_base_of<MatrixValue, T>{}, "Must derive from MatrixValue");

private:
    static size_t number_of_created_pls;
    static size_t number_of_destroyed_pls;
    // static std::vector<PL *> all_created_pls;

    static wchar_t get_subscript_character(unsigned int number);
    static void print_variable(const char *name = "x", unsigned int number = 0);

    PL(size_t number_of_constraints, size_t number_of_variables, MatrixClass<T> *matrixA, MatrixClass<MatrixPLConstraintTypeValue> *matrixAB, MatrixClass<T> *matrixB, MatrixClass<T> *matrixC);
    ~PL();

    size_t number_of_variables;
    size_t number_of_constraints;

    MatrixClass<T> *matrixC;
    MatrixClass<T> *matrixA;

    // constraints type
    MatrixClass<MatrixPLConstraintTypeValue> *matrixAB;

    MatrixClass<T> *matrixB;

public:
    static PL *create_pl(size_t constraints_len, size_t variables_len);
    static PL *create_random_pl(size_t constraints_len, size_t variables_len);

    bool convert_contraint_type_to(size_t constraint_index, PLConstraintType constraint_type);

    PLConstraintType get_constraint_type(size_t constraint_index);

    PL *convert_to_form(PLForms form, bool in_place = false);
    bool is_on_form(PLForms form);

    static PL *copy(PL *pl);

    // std::string name;
    // std::string description;

    // bool standarize_constraint();
    // bool convert_constraint();

    // bool constraints_map();
    // bool constraints_foreach();
    // bool constraints_every();
    // bool constraints_each();

    // bool variables_map();
    // bool variables_foreach();
    // bool variables_every();
    // bool variables_each();

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