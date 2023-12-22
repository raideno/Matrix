#include <stdio.h>

#include "../util/util.hpp"

#include "matrix-pl-constraint-type-value.hpp"

MatrixPLConstraintTypeValue::MatrixPLConstraintTypeValue()
{
    // printf("[MatrixPLConstraintTypeValue()]: have been called\n");
    // this->data = PLConstraintType::EQUAL;
    this->data = 0;
}

MatrixPLConstraintTypeValue::MatrixPLConstraintTypeValue(PLConstraintType data)
{
    // printf("[MatrixPLConstraintTypeValue(int)]: have been called\n");
    this->data = data;
}

MatrixPLConstraintTypeValue::~MatrixPLConstraintTypeValue()
{
    // printf("[~Int()]: have been called\n");
}

MatrixPLConstraintTypeValue MatrixPLConstraintTypeValue::add(MatrixPLConstraintTypeValue value)
{
    MatrixPLConstraintTypeValue result;
    return result;
}

MatrixPLConstraintTypeValue MatrixPLConstraintTypeValue::sub(MatrixPLConstraintTypeValue value)
{
    MatrixPLConstraintTypeValue result;
    return result;
}

MatrixPLConstraintTypeValue MatrixPLConstraintTypeValue::mul(MatrixPLConstraintTypeValue value)
{
    MatrixPLConstraintTypeValue result = this->data * value.data;
    return result;
}

MatrixPLConstraintTypeValue MatrixPLConstraintTypeValue::div(MatrixPLConstraintTypeValue value)
{
    MatrixPLConstraintTypeValue result;
    return result;
}

MatrixPLConstraintTypeValue MatrixPLConstraintTypeValue::rand()
{
    int integer = random_integer(-2, 2);
    switch (integer)
    {
    case -2:
        return -2;
        // return PLConstraintType::BIGGER;

    case -1:
        return -1;
        // return PLConstraintType::BIGGER_OR_EQUAL;

    case 0:
        return 0;
        // return PLConstraintType::EQUAL;

    case 1:
        return 1;
        // return PLConstraintType::LOWER_OR_EQUAL;

    case 2:
        return 2;
        // return PLConstraintType::LOWER;

    default:
        return 0;
        // return PLConstraintType::EQUAL;
        break;
    }
}

void MatrixPLConstraintTypeValue::print()
{
    switch (this->data)
    {
    case -2:
        printf(">");
        break;

    case -1:
        printf("≥");
        break;

    case 0:
        printf("=");
        break;

    case 1:
        printf("≤");
        break;

    case 2:
        printf("<");
        break;

    default:
        printf("-");
        break;
    }
}