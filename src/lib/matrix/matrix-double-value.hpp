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

    MatrixDoubleValue inverse();

    MatrixDoubleValue abs();

    MatrixDoubleValue power(MatrixDoubleValue value);

    // TODO: implement, -1 means this is less the value, 0 means equality, 1 means this is bigger then value
    MatrixValueComparaisons compare(MatrixDoubleValue value);
    // TODO: implement, -1 means this is less the value, 0 means equality, 1 means this is bigger then value
    static MatrixValueComparaisons compare(MatrixDoubleValue value1, MatrixDoubleValue value2);

    static MatrixDoubleValue null();

    static MatrixDoubleValue identity();

    static MatrixDoubleValue negative_identity();

    static MatrixDoubleValue rand();

    static MatrixDoubleValue read();

    void print();
};

#endif