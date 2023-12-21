#include <stdio.h>

#include "../util/util.hpp"

#include "matrix-value.hpp"
#include "matrix-double-value.hpp"

MatrixDoubleValue::MatrixDoubleValue()
{
    // printf("[MatrixDoubleValue()]: have been called\n");
}

MatrixDoubleValue::MatrixDoubleValue(double data)
{
    // printf("[MatrixDoubleValue(double)]: have been called\n");
    this->data = data;
}

MatrixDoubleValue::~MatrixDoubleValue()
{
    // printf("[~Double()]: have been called\n");
}

MatrixDoubleValue MatrixDoubleValue::add(MatrixDoubleValue value)
{
    MatrixDoubleValue result(this->data + value.data);
    return result;
}

MatrixDoubleValue MatrixDoubleValue::sub(MatrixDoubleValue value)
{
    MatrixDoubleValue result(this->data - value.data);
    return result;
}

MatrixDoubleValue MatrixDoubleValue::mul(MatrixDoubleValue value)
{
    MatrixDoubleValue result(this->data * value.data);
    return result;
}

MatrixDoubleValue MatrixDoubleValue::div(MatrixDoubleValue value)
{
    MatrixDoubleValue result(this->data / value.data);
    return result;
}

MatrixDoubleValue MatrixDoubleValue::rand()
{
    return MatrixDoubleValue(random_double());
}

void MatrixDoubleValue::print()
{
    printf("%f", data);
}