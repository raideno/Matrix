#ifndef __SLE_HPP__
#define __SLE_HPP__

#include "matrix.hpp"

enum SystemDebug
{
    SYSTEM_NONE = 1,
    SYSTEM_CREATION = 2,
    SYSTEM_ALLOCATION = 4,
    SYSTEM_DESTRUCTION = 8,
    SYSTEM_DESALLOCATION = 16,
    SYSTEM_MISC = 32,
    SYSTEM_ALL = SYSTEM_CREATION | SYSTEM_ALLOCATION | SYSTEM_DESTRUCTION | SYSTEM_DESALLOCATION | SYSTEM_MISC
};
typedef SystemDebug SystemDebug;

class Sle
{
private:
    std::string id;

    inline static unsigned int seed = 45;
    inline static SystemDebug debug_options = SystemDebug::SYSTEM_NONE;

public:
    std::string name;
    std::size_t order;
    MatrixClass *matrix;
    MatrixClass *vector;

    static void srand(unsigned int seed);

    static void set_debug_options(SystemDebug debug);
    static bool is_debug_option_set(SystemDebug debug_option);

    Sle();
    Sle(std::size_t order);
    // we don't use the matrix and vector you provide, we just make copies of them
    Sle(MatrixClass *matrix, MatrixClass *vector);

    ~Sle();

    Sle *set_name(const std::string &name);

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