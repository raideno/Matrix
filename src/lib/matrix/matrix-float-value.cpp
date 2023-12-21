#include <stdio.h>

#include "../util/util.hpp"

#include "matrix-value.hpp"
#include "matrix-float-value.hpp"

MatrixFloatValue::MatrixFloatValue()
{
    // printf("[MatrixFloatValue()]: have been called\n");
}

MatrixFloatValue::MatrixFloatValue(float data)
{
    // printf("[MatrixFloatValue(float)]: have been called\n");
    this->data = data;
}

MatrixFloatValue::~MatrixFloatValue()
{
    // printf("[~Float()]: have been called\n");
}

MatrixFloatValue MatrixFloatValue::add(MatrixFloatValue value)
{
    MatrixFloatValue result(this->data + value.data);
    return result;
}

MatrixFloatValue MatrixFloatValue::sub(MatrixFloatValue value)
{
    MatrixFloatValue result(this->data - value.data);
    return result;
}

MatrixFloatValue MatrixFloatValue::mul(MatrixFloatValue value)
{
    MatrixFloatValue result(this->data * value.data);
    return result;
}

MatrixFloatValue MatrixFloatValue::div(MatrixFloatValue value)
{
    MatrixFloatValue result(this->data / value.data);
    return result;
}

MatrixFloatValue MatrixFloatValue::rand()
{
    return MatrixFloatValue(random_float());
}

void MatrixFloatValue::print()
{
    printf("%f", data);
}