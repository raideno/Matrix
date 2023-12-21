#ifndef __MATRIX_FLOAT_VALUE_HPP__
#define __MATRIX_FLOAT_VALUE_HPP__

#include "matrix-value.hpp"

class MatrixFloatValue : public MatrixValue
{
public:
    float data;

    MatrixFloatValue();

    MatrixFloatValue(float data);

    ~MatrixFloatValue();

    MatrixFloatValue add(MatrixFloatValue value);

    MatrixFloatValue sub(MatrixFloatValue value);

    MatrixFloatValue mul(MatrixFloatValue value);

    MatrixFloatValue div(MatrixFloatValue value);

    static MatrixFloatValue rand();

    void print();
};

#endif