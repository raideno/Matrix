#ifndef __MATRIX_INT_VALUE_HPP__
#define __MATRIX_INT_VALUE_HPP__

#include "matrix-value.hpp"

class MatrixIntValue : public MatrixValue
{
public:
    int data;

    MatrixIntValue();

    MatrixIntValue(int data);

    ~MatrixIntValue();

    MatrixIntValue add(MatrixIntValue value);

    MatrixIntValue sub(MatrixIntValue value);

    MatrixIntValue mul(MatrixIntValue value);

    MatrixIntValue div(MatrixIntValue value);

    static MatrixIntValue rand();

    void print();
};

#endif