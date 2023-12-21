#include <string>
#include <wchar.h>

#include "../matrix/matrix.hpp"

#include "pl.hpp"

template <typename T>
PL<T>::PL()
{
    this->matrixA = NULL;
    this->matrixAB = NULL;
    this->matrixB = NULL;
    this->matrixC = NULL;
    return;
}

template <typename T>
PL<T>::PL(MatrixClass<T> *matrixA, MatrixClass<MatrixPLConstraintTypeValue> *matrixAB, MatrixClass<T> *matrixB, MatrixClass<T> *matrixC)
{
    this->matrixA = matrixA;
    this->matrixAB = matrixAB;
    this->matrixB = matrixB;
    this->matrixC = matrixC;
}

template <typename T>
PL<T>::~PL()
{
    return;
}

template <typename T>
PL<T> *PL<T>::create_pl(size_t constraints_len, size_t variables_len)
{
    PL *pl = new PL();

    return pl;
}

template <typename T>
PL<T> *PL<T>::create_pl_from(size_t constraints_len, size_t variables_len)
{
    PL *pl = new PL();

    return pl;
}

template <typename T>
PL<T> *PL<T>::create_pl_random(size_t constraints_len, size_t variables_len)
{

    MatrixClass<T> *matrixA = MatrixClass<T>::create_matrix_random(MatrixType::NORMAL, constraints_len, variables_len);
    MatrixClass<MatrixPLConstraintTypeValue> *matrixAB = MatrixClass<MatrixPLConstraintTypeValue>::create_matrix_random(MatrixType::NORMAL, constraints_len, 1);
    MatrixClass<T> *matrixB = MatrixClass<T>::create_matrix_random(MatrixType::NORMAL, constraints_len, 1);
    MatrixClass<T> *matrixC = MatrixClass<T>::create_matrix_random(MatrixType::NORMAL, 1, variables_len);

    matrixA->set_name("Constraints Coefficients");
    matrixAB->set_name("Constraints Type");
    matrixB->set_name("Constraints Right Member");
    matrixA->set_name("Objective Function Coefficients");

    PL *pl = new PL(matrixA, matrixAB, matrixB, matrixC);

    return pl;
}

template <typename T>
void PL<T>::print()
{
    printf("[pl]:\n");

    this->print_objective_function();

    printf("-------------------------------------------------------------------------------------\n");

    for (size_t i = 0; i < this->matrixA->size().first; i++)
    {
        printf("\t\t\t\t\t");
        this->print_constraint(i);
    }
}

template <typename T>
void PL<T>::destroy()
{
    this->matrixA->destroy();
    this->matrixAB->destroy();
    this->matrixB->destroy();
    this->matrixC->destroy();
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

    printf("\t\t");

    for (size_t i = 0; i < this->matrixC->size().second; i++)
    {
        this->matrixC->get(0, i).print();
        PL<T>::print_variable("x", i);
        printf("\t");
    };

    printf("\n");

    return;
}