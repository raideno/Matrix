#include <stdio.h>
#include <iostream>
#include <cmath>

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

MatrixDoubleValue MatrixDoubleValue::abs()
{
    return MatrixDoubleValue(this->data < 0 ? this->data * -1 : this->data);
}

MatrixDoubleValue MatrixDoubleValue::power(MatrixDoubleValue value)
{
    return MatrixDoubleValue(std::pow(this->data, value.data));
}

MatrixDoubleValue MatrixDoubleValue::null()
{
    return MatrixDoubleValue(0);
}

MatrixDoubleValue MatrixDoubleValue::identity()
{
    return MatrixDoubleValue(1);
}

// TODO: implement, -1 means this is less the value, 0 means equality, 1 means this is bigger then value
MatrixValueComparaisons MatrixDoubleValue::compare(MatrixDoubleValue value)
{
    return MatrixDoubleValue::compare(*this, value);
}

MatrixDoubleValue MatrixDoubleValue::inverse()
{
    return MatrixDoubleValue(1 / this->data);
}

// TODO: implement, -1 means this is less the value, 0 means equality, 1 means this is bigger then value
MatrixValueComparaisons MatrixDoubleValue::compare(MatrixDoubleValue value1, MatrixDoubleValue value2)
{
    if (value1.data == value2.data)
        return MatrixValueComparaisons::EQUAL;
    else if (value1.data > value2.data)
        return MatrixValueComparaisons::BIGGER;
    // else if (value1.data < value2.data)
    else
        return MatrixValueComparaisons::LOWER;
}

MatrixDoubleValue MatrixDoubleValue::negative_identity()
{
    return MatrixDoubleValue(-1);
}

void MatrixDoubleValue::print()
{
    printf("%f", data);
}

MatrixDoubleValue MatrixDoubleValue::read()
{
    double data;
    scanf("%lf", &data);
    return MatrixDoubleValue(data);
}