#include <string>
#include <wchar.h>

#include "../matrix/matrix.hpp"

#include "pl.hpp"

template <typename T>
size_t PL<T>::number_of_created_pls = 0;

template <typename T>
size_t PL<T>::number_of_destroyed_pls = 0;

/**
 * ------------------------------------------------------
 */

template <typename T>
PL<T>::PL(size_t number_of_constraints, size_t number_of_variables, MatrixClass<T> *matrixA, MatrixClass<MatrixPLConstraintTypeValue> *matrixAB, MatrixClass<T> *matrixB, MatrixClass<T> *matrixC)
{
    /**
     * TODO: do the checking and make ure sizes are correct
     */

    this->number_of_constraints = number_of_constraints;
    this->number_of_variables = number_of_variables;

    this->matrixA = matrixA;
    this->matrixAB = matrixAB;
    this->matrixB = matrixB;
    this->matrixC = matrixC;

    PL<T>::number_of_created_pls++;
    // PL<T>::all_created_pls.push_back(this);

    return;
}

template <typename T>
PL<T>::~PL()
{
    this->matrixA->destroy();
    this->matrixAB->destroy();
    this->matrixB->destroy();
    this->matrixC->destroy();

    PL<T>::number_of_destroyed_pls--;
    // std::remove(PL<T>::all_created_pls.begin(), PL<T>::all_created_pls.end(), this);

    return;
}

template <typename T>
PL<T> *PL<T>::create_pl(size_t constraints_len, size_t variables_len)
{
    MatrixClass<T> *matrixA = MatrixClass<T>::create_matrix(constraints_len, variables_len);
    MatrixClass<MatrixPLConstraintTypeValue> *matrixAB = MatrixClass<MatrixPLConstraintTypeValue>::create_matrix(constraints_len, 1);
    MatrixClass<T> *matrixB = MatrixClass<T>::create_matrix(constraints_len, 1);
    MatrixClass<T> *matrixC = MatrixClass<T>::create_matrix(1, variables_len);

    matrixA->set_name("Constraints Coefficients");
    matrixAB->set_name("Constraints Type");
    matrixB->set_name("Constraints Right Member");
    matrixA->set_name("Objective Function Coefficients");

    PL *pl = new PL(matrixA, matrixAB, matrixB, matrixC);

    return pl;
}

template <typename T>
PL<T> *PL<T>::create_random_pl(size_t constraints_len, size_t variables_len)
{

    MatrixClass<T> *matrixA = MatrixClass<T>::create_matrix_random(MatrixType::NORMAL, constraints_len, variables_len);
    MatrixClass<MatrixPLConstraintTypeValue> *matrixAB = MatrixClass<MatrixPLConstraintTypeValue>::create_matrix_random(MatrixType::NORMAL, constraints_len, 1);
    MatrixClass<T> *matrixB = MatrixClass<T>::create_matrix_random(MatrixType::NORMAL, constraints_len, 1);
    MatrixClass<T> *matrixC = MatrixClass<T>::create_matrix_random(MatrixType::NORMAL, 1, variables_len);

    matrixA->set_name("Constraints Coefficients");
    matrixAB->set_name("Constraints Type");
    matrixB->set_name("Constraints Right Member");
    matrixA->set_name("Objective Function Coefficients");

    PL *pl = new PL(constraints_len, variables_len, matrixA, matrixAB, matrixB, matrixC);

    return pl;
}

template <typename T>
void PL<T>::print()
{
    this->print_objective_function();

    for (size_t i = 0; i < this->matrixA->size().first; i++)
    {
        this->print_constraint(i);
    }
}

template <typename T>
void PL<T>::destroy()
{
    delete this;
}

template <typename T>
void PL<T>::print_constraint(size_t constraint_index)
{
    // TODO: check that the matrices are valid
    // TODO: check that it's the good constraint number

    for (size_t j = 0; j < this->matrixA->size().second; j++)
    {
        this->matrixA->get(constraint_index, j).print();
        PL<T>::print_variable("x", j);
        printf("\t");
    }

    matrixAB->get(constraint_index, 0).print();

    printf("\t");

    this->matrixB->get(constraint_index, 0).print();

    printf("\n");
}

template <typename T>
wchar_t PL<T>::get_subscript_character(unsigned int number)
{
    wchar_t subscript_characters[10] = {
        0x2080,
        0x2081,
        0x2082,
        0x2083,
        0x2084,
        0x2085,
        0x2086,
        0x2087,
        0x2088,
        0x2089,
    };

    if (number < 0 || number > 9)
        return subscript_characters[0];
    else
        return subscript_characters[number];
}

template <typename T>
void PL<T>::print_variable(const char *name, unsigned int number)
{
    // char variable_id = (char)(0x2080 + number);

    printf("%s", name);

    std::string string_number = std::to_string(number);

    for (char char_digit : string_number)
    {
        int int_digit = char_digit - '0';
        printf("%lc", get_subscript_character(int_digit));
    }
}

template <typename T>
void PL<T>::print_objective_function()
{
    printf("F(");
    for (size_t i = 0; i < this->matrixC->size().second; i++)
    {
        PL<T>::print_variable("x", i);
        if (i != this->matrixC->size().second - 1)
            printf(",\t");
    }
    printf(") = ");

    for (size_t i = 0; i < this->matrixC->size().second; i++)
    {
        this->matrixC->get(0, i).print();
        PL<T>::print_variable("x", i);
        printf("\t");
    };

    printf("\n");

    return;
}

template <typename T>
PLConstraintType PL<T>::get_constraint_type(size_t constraint_index)
{
    return this->matrixAB->get(constraint_index, 0).data;
}

template <typename T>
bool PL<T>::convert_contraint_type_to(size_t constraint_index, PLConstraintType target_constraint_type)
{
    /**
     * bigger-equal -   lower-equal         x
     * lower-equal  -   bigger-equal        x
     *                                      -
     *                                      -
     *                                      -
     * bigger-equal -   equal               -
     * lower-equal  -   equal               -
     *                                      -
     *                                      -
     *                                      -
     * equal        -   bigger-equal        -
     * equal        -   lower-equal         -
     *                                      -
     *                                      -
     *                                      -
     * equal        - equal                 x
     * bigger-equal - bigger-equal          x
     * lower-equal  - lower-equal           x
     * lower        - lower                 x
     * bigger       - bigger                x
     *
     * other ones..
     */

    PLConstraintType actual_constraint_type = this->matrixAB->get(constraint_index, 0).data;

    if (actual_constraint_type == target_constraint_type)
        return false;

    /**
     * as long as equality isn't involved and the actual_constraint_type and target_constraint_type aren't equal the operation below is valid
     */
    if (actual_constraint_type != 0 && target_constraint_type != 0)
    {
        /**
         * multiply matrixA at line 1 by -1 and matrixB by -1 and matrixAB by -1
         */
        this->matrixA->map_line(
            constraint_index,
            [](size_t _i, size_t _j, T element) -> T
            { return element.mul(T(-1)); },
            true);

        this->matrixAB->map_line(
            constraint_index,
            [](size_t _i, size_t _j, MatrixPLConstraintTypeValue element) -> MatrixPLConstraintTypeValue
            { return element.mul(MatrixPLConstraintTypeValue(-1)); },
            true);

        this->matrixB->map_line(
            constraint_index,
            [](size_t _i, size_t _j, T element) -> T
            { return element.mul(T(-1)); },
            true);

        return true;
    }

    if (target_constraint_type == 0)
    {
        /**
         * make sure we (actual_constraint_type) are either (bigger|lower)-or-equal
         * add new column in C matrix and make it 0
         * add new column in A matrix and make it 1 (same sign / value as constraint type) at constraint_index line and 0 everywhere else
         * increment number of variables index
         * convert constraint type to equality
         */

        // make sure we (actual_constraint_type) are either (bigger|lower)-or-equal
        if (actual_constraint_type == -2 || actual_constraint_type == 2)
            return false;

        // add new column in C matrix and make it 0
        this->matrixC->resize(this->matrixC->size().first, this->matrixC->size().second + 1, true);

        // add new column in A matrix and make it 1 (same sign / value as constraint type) at constraint_index line and 0 everywhere else
        this->matrixA->resize(this->matrixA->size().first, this->matrixA->size().second + 1, true);
        this->matrixA->set(constraint_index, this->matrixA->size().second - 1, T(actual_constraint_type));

        // increment number of variables index
        this->number_of_variables++;

        // convert constraint type to equality
        this->matrixAB->set(constraint_index, 0, MatrixPLConstraintTypeValue(0));

        return true;
    }

    if (actual_constraint_type == 0)
    {
        return false;
    }

    return false;
}