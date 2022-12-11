#ifndef __SLE_HPP__
#define __SLE_HPP__

#include "matrix.hpp"

class Sle
{
private:
public:
    std::size_t order;
    MatrixClass *matrix;
    MatrixClass *vector;

    Sle();
    Sle(std::size_t order);
    // we don't use the matrix and vector you provide, we just make copies of them
    Sle(MatrixClass *matrix, MatrixClass *vector);

    ~Sle();

    void read(); /*implement it*/
    void print();

    Sle *copy();

    Sle *gauss(bool inplace = false);
    Sle *gauss_total(bool inplace = false);
    Sle *gauss_partial(bool inplace = false);
    // need optimisation on the jordan part
    Sle *gauss_jordan(bool inplace = false);

    MatrixClass *solve_cramer();
    MatrixClass *solve_diagonal();
    MatrixClass *solve_upper_triangular();
    MatrixClass *solve_lower_triangular();

    MatrixClass *solve(MatrixType type);

    static Sle *create_power_system(std::size_t order);
    static Sle *create_hilbert_system(std::size_t order);
    static Sle *create_random_int_system(std::size_t order, int min = 0, int max = 10);
    static Sle *create_random_float_system(std::size_t order, float min = 0, float max = 10);
};

std::ostream &operator<<(std::ostream &os, Sle &sle);

#endif