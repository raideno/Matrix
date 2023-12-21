#include <stdio.h>

#include "../util/util.hpp"

#include "matrix-value.hpp"
#include "matrix-int-value.hpp"

MatrixIntValue::MatrixIntValue()
{
    // printf("[MatrixIntValue()]: have been called\n");
}

MatrixIntValue::MatrixIntValue(int data)
{
    // printf("[MatrixIntValue(int)]: have been called\n");
    this->data = data;
}

MatrixIntValue::~MatrixIntValue()
{
    // printf("[~Int()]: have been called\n");
}

MatrixIntValue MatrixIntValue::add(MatrixIntValue value)
{
    MatrixIntValue result(this->data + value.data);
    return result;
}

MatrixIntValue MatrixIntValue::sub(MatrixIntValue value)
{
    MatrixIntValue result(this->data - value.data);
    return result;
}

MatrixIntValue MatrixIntValue::mul(MatrixIntValue value)
{
    MatrixIntValue result(this->data * value.data);
    return result;
}

MatrixIntValue MatrixIntValue::div(MatrixIntValue value)
{
    MatrixIntValue result(this->data / value.data);
    return result;
}

MatrixIntValue MatrixIntValue::rand()
{
    return MatrixIntValue(random_integer());
}

void MatrixIntValue::print()
{
    printf("%+d", data);
}