#ifndef __MATRIX_PL_CONSTRAINT_TYPE_VALUE__
#define __MATRIX_PL_CONSTRAINT_TYPE_VALUE__

#include "../matrix/matrix-value.hpp"

// #include "pl.hpp"

// enum PLConstraintType
// {
//     BIGGER = -2,
//     BIGGER_OR_EQUAL = -1,
//     EQUAL = 0,
//     LOWER_OR_EQUAL = 1,
//     LOWER = 2,
// };
// typedef enum PLConstraintType PLConstraintType;
typedef int PLConstraintType;

class MatrixPLConstraintTypeValue : public MatrixValue
{
public:
    PLConstraintType data;

    MatrixPLConstraintTypeValue();

    MatrixPLConstraintTypeValue(PLConstraintType data);

    ~MatrixPLConstraintTypeValue();

    MatrixPLConstraintTypeValue add(MatrixPLConstraintTypeValue value);

    MatrixPLConstraintTypeValue sub(MatrixPLConstraintTypeValue value);

    MatrixPLConstraintTypeValue mul(MatrixPLConstraintTypeValue value);

    MatrixPLConstraintTypeValue div(MatrixPLConstraintTypeValue value);

    static MatrixPLConstraintTypeValue rand();

    void print();
};

#endif