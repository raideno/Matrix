#ifndef __SLE_HPP__
#define __SLE_HPP__

#include "matrix.hpp"

class Sle
{
private:
public:
    std::size_t order;
    MatrixClass matrix;
    MatrixClass vector;

    Sle();
    Sle(std::size_t order);
    Sle(MatrixClass &matrix, MatrixClass &vector);
    ~Sle();

    void read(); /*implement it*/
    void print();

    Sle &copy();

    Sle &gauss();
    Sle &gauss_total();
    Sle &gauss_partial();

    MatrixClass &solve_cramer();
    MatrixClass &solve_upper_triangular();
    MatrixClass &solve_lower_triangular();

    static Sle &create_power_system(std::size_t order);
    static Sle &create_hilbert_system(std::size_t order);
};

std::ostream &operator<<(std::ostream &os, Sle &sle);

#endif