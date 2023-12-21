#ifndef __MATRIX_DOUBLE_VALUE_HPP__
#define __MATRIX_DOUBLE_VALUE_HPP__

#include "matrix-value.hpp"

class MatrixDoubleValue : public MatrixValue
{
public:
    double data;

    MatrixDoubleValue();

    MatrixDoubleValue(double data);

    ~MatrixDoubleValue();

    MatrixDoubleValue add(MatrixDoubleValue value);

    MatrixDoubleValue sub(MatrixDoubleValue value);

    MatrixDoubleValue mul(MatrixDoubleValue value);

    MatrixDoubleValue div(MatrixDoubleValue value);

    static MatrixDoubleValue rand();

    void print();
};

#endif