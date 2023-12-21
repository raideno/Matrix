#ifndef __MATRIX_VALUE_HPP__
#define __MATRIX_VALUE_HPP__

class MatrixValue
{
private:
public:
    inline void print();
    inline MatrixValue add(MatrixValue value);
    inline MatrixValue mul(MatrixValue value);
    inline MatrixValue sub(MatrixValue value);
    inline MatrixValue div(MatrixValue value);
    inline static MatrixValue rand();
};

#endif