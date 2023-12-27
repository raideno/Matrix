#ifndef __MATRIX_VALUE_HPP__
#define __MATRIX_VALUE_HPP__

enum MatrixValueComparaisons
{
    LOWER = -1,
    EQUAL = 0,
    BIGGER = 1
};

class MatrixValue
{
private:
public:
    // TODO: Experiment
    /**---*/
    // bool is_unknown;
    // std::string unknown_name;
    // bool fix_to(MatrixValue value);
    // bool assign(MatrixValue value);
    /**---*/

    inline MatrixValue add(MatrixValue value);
    inline MatrixValue mul(MatrixValue value);
    inline MatrixValue sub(MatrixValue value);
    inline MatrixValue div(MatrixValue value);

    inline MatrixValue power(MatrixValue value);

    inline MatrixValue abs();

    inline MatrixValue inverse();

    // TODO: implement, -1 means this is less the value, 0 means equality, 1 means this is bigger then value
    inline MatrixValueComparaisons compare(MatrixValue value);
    // TODO: implement, -1 means this is less the value, 0 means equality, 1 means this is bigger then value
    inline static MatrixValueComparaisons compare(MatrixValue value1, MatrixValue value2);

    inline void print();

    inline static MatrixValue null();
    inline static MatrixValue identity();

    // TODO: implement
    inline static MatrixValue negative_identity();

    inline static MatrixValue read();

    inline static MatrixValue rand();
};

#endif