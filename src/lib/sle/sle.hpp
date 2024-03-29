#ifndef __SLE_HPP__
#define __SLE_HPP__

#include "../matrix/matrix.hpp"

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

/*
    Throw Exceptions in C
*/
template <typename T>
class Sle
{
private:
    std::string id = "";
    std::string name = "";
    std::size_t order = 0;

    MatrixClass<T> *matrix = NULL;
    MatrixClass<T> *vector = NULL;

    inline static unsigned int seed = 45;
    inline static SystemDebug debug_options = SystemDebug::SYSTEM_NONE;

    static std::string generate_unique_id(size_t n, size_t m);

    Sle();
    Sle(std::size_t order);
    Sle(MatrixClass<T> *matrix, MatrixClass<T> *vector);

    ~Sle();

public:
    static void srand(unsigned int seed);

    static void set_debug_options(SystemDebug debug);
    static bool is_debug_option_set(SystemDebug debug_option);

    static Sle *creaate_system(std::size_t order);
    static Sle *creaate_system(MatrixClass<T> *matrix, MatrixClass<T> *vector);

    // we don't use the matrix and vector you provide, we just make copies of them
    // we return NULL from the constructor if the creation fails

    void destroy();

    Sle *set_name(const std::string &name);

    const std::string &get_name();

    std::size_t get_order();

    void set_matrix(MatrixClass<T> *matrix);
    void set_vector(MatrixClass<T> *vector);

    void read(); /*implement it*/
    void print();

    Sle *copy();

    Sle *gauss(bool inplace = false);
    Sle *gauss_total(bool inplace = false);
    Sle *gauss_partial(bool inplace = false);
    // need optimisation on the jordan part
    Sle *gauss_jordan(bool inplace = false);

    MatrixClass<T> *solve_cramer();
    MatrixClass<T> *solve_diagonal();
    MatrixClass<T> *solve_upper_triangular();
    MatrixClass<T> *solve_lower_triangular();

    MatrixClass<T> *solve(MatrixType type);

    static Sle *create_power_system(std::size_t order);
    static Sle *create_hilbert_system(std::size_t order);
    static Sle *create_random_int_system(std::size_t order, int min = 0, int max = 10);
    static Sle *create_random_float_system(std::size_t order, float min = 0, float max = 10);
};

template <typename T>
std::ostream &operator<<(std::ostream &os, Sle<T> &sle);

#endif