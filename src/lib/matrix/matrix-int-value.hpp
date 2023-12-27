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

    MatrixIntValue inverse();

    // TODO: implement
    MatrixIntValue abs();

    // TODO: implement
    MatrixIntValue power(MatrixIntValue value);

    static MatrixIntValue null();

    static MatrixIntValue identity();

    static MatrixIntValue negative_identity();

    // TODO: implement, -1 means this is less the value, 0 means equality, 1 means this is bigger then value
    MatrixValueComparaisons compare(MatrixIntValue value);
    // TODO: implement, -1 means this is less the value, 0 means equality, 1 means this is bigger then value
    static MatrixValueComparaisons compare(MatrixIntValue value1, MatrixIntValue value2);

    static MatrixIntValue rand();

    static MatrixIntValue read();

    void print();
};

#endif