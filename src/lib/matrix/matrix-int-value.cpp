#include <stdio.h>
#include <cmath>

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

// TODO: implement
MatrixIntValue MatrixIntValue::abs()
{
    return MatrixIntValue(this->data > 0 ? this->data : this->data * -1);
}

// TODO: implement
MatrixIntValue MatrixIntValue::power(MatrixIntValue value)
{
    return MatrixIntValue(std::pow(this->data, value.data));
}

MatrixIntValue MatrixIntValue::null()
{
    return MatrixIntValue(0);
}

MatrixIntValue MatrixIntValue::identity()
{
    return MatrixIntValue(1);
}

MatrixIntValue MatrixIntValue::negative_identity()
{
    return MatrixIntValue(-1);
}

MatrixIntValue MatrixIntValue::inverse()
{
    return MatrixIntValue(1 / this->data);
}

// TODO: implement, -1 means this is less the value, 0 means equality, 1 means this is bigger then value
MatrixValueComparaisons MatrixIntValue::compare(MatrixIntValue value)
{
    return MatrixIntValue::compare(*this, value);
}

// TODO: implement, -1 means this is less the value, 0 means equality, 1 means this is bigger then value
MatrixValueComparaisons MatrixIntValue::compare(MatrixIntValue value1, MatrixIntValue value2)
{
    if (value1.data == value2.data)
        return MatrixValueComparaisons::EQUAL;
    else if (value1.data > value2.data)
        return MatrixValueComparaisons::BIGGER;
    // else if (value1.data < value2.data)
    else
        return MatrixValueComparaisons::LOWER;
}

void MatrixIntValue::print()
{
    printf("%+d", data);
}

MatrixIntValue MatrixIntValue::read()
{
    int data;
    scanf("%d", &data);
    return MatrixIntValue(data);
}