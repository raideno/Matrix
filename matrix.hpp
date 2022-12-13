#ifndef __MATRIX_HPP__
#define __MATRIX_HPP__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <iostream>

#include <functional>

// functions to round, to transform to integer
// functions to modify matrix print format, or pass format allon side with parameters

// add an optional param to say if the function should act destructive or noo

/*IMPORTANT*/
/*
    make constructor private and use Factory methodes instead
*/
/*
    Throw Exceptions in C
*/
enum MatrixType
{
    NORMAL,
    DIAGONAL,
    UPPER_TRIANGLE,
    LOWER_TRIANGLE,
};
typedef enum MatrixType MatrixType;

enum MatrixDebug
{
    MATRIX_NONE = 1,
    MATRIX_CREATION = 2,
    MATRIX_ALLOCATION = 4,
    MATRIX_DESTRUCTION = 8,
    MATRIX_DESALLOCATION = 16,
    MATRIX_MISC = 32,
    MATRIX_ALL = MATRIX_CREATION | MATRIX_ALLOCATION | MATRIX_DESTRUCTION | MATRIX_DESALLOCATION | MATRIX_MISC,
};

typedef const std::function<void(size_t, size_t, float)> &Consumer;
typedef const std::function<float(size_t, size_t, float)> &Producer;
typedef const std::function<bool(size_t, size_t, float)> &BooleanProducer;

/*row and column operations*/

class MatrixClass
{

private:
    float **content = NULL;

    std::string id = "";
    std::string name = "";
    std::size_t n = 0;
    std::size_t m = 0;

    inline static unsigned int seed = 45;
    inline static MatrixDebug debug_options = MatrixDebug::MATRIX_NONE;

    inline static std::size_t allocated_matrices = 0;
    inline static std::size_t desallocated_matrices = 0;

    inline static std::size_t created_matrices = 0;
    inline static std::size_t destroyed_matrices = 0;

    static void desallocate(MatrixClass *matrix);
    static void desallocate_matrices(size_t n_args, ...);

    static float **allocate_matrix(size_t n, size_t m);

    static std::string generate_unique_id(size_t n, size_t m);

    MatrixClass();
    MatrixClass(size_t n, size_t m);

    ~MatrixClass();

public:
    // add matrix element by element multiplication and matrix element by element division
    /*reduce functions*/
    /*norme vectorielle*/
    /*valeurs propres, vecteurs propres*/
    /*delimité une matrice, sous matrice*/

    static void srand(unsigned int seed);

    static void set_debug_options(MatrixDebug debug);
    static bool is_debug_option_set(MatrixDebug debug_option);

    void destroy();

    // creation stuff
    /*----*/
    static MatrixClass *create_matrix(size_t n, size_t m);
    static MatrixClass *matrix_from_array(MatrixType type, float *array, size_t n, size_t m);
    static MatrixClass *create_matrix_with(MatrixType type, size_t n, size_t m, float number);
    static MatrixClass *create_matrix_random_int(MatrixType type, size_t n, size_t m, int min = 0, int max = 10);
    static MatrixClass *create_matrix_random_float(MatrixType type, size_t n, size_t m, float min = 0, float max = 1);
    /*----*/

    float get(size_t i, size_t j);
    void set(size_t i, size_t j, float element);

    const std::string &get_name();
    MatrixClass *set_name(const std::string &name);

    std::pair<std::size_t, std::size_t> size();

    MatrixClass *copy();
    static MatrixClass *copy(MatrixClass *matrix);

    void for_each(MatrixType type, Consumer consumer);
    void for_each_line(size_t line, Consumer consumer);
    void for_each_column(size_t column, Consumer consumer);

    MatrixClass *map(MatrixType type, Producer consumer, bool inplace = false);
    MatrixClass *map_line(size_t line, Producer consumer, bool inplace = false);
    MatrixClass *map_column(size_t column, Producer consumer, bool inplace = false);

    MatrixClass *permute_lines(size_t L1, size_t L2, bool destructive = false);
    MatrixClass *permute_columns(size_t C1, size_t C2, bool destructive = false);

    MatrixClass *inverse(bool destructive = false);
    MatrixClass *transpose(bool destructive = false);
    MatrixClass *cofactor();
    MatrixClass *cofactor_of(size_t line, size_t column);

    bool all(MatrixType type, BooleanProducer boolean_producer);
    bool line_all(size_t line, BooleanProducer boolean_producer);
    bool column_all(size_t column, BooleanProducer boolean_producer);

    bool one(MatrixType type, BooleanProducer boolean_producer);
    bool line_one(size_t line, BooleanProducer boolean_producer);
    bool column_one(size_t column, BooleanProducer boolean_producer);

    static MatrixClass *concatenate_vertical(size_t n_args, ...);
    static MatrixClass *concatenate_horizontal(size_t n_args, ...);

    void read(MatrixType type = NORMAL);

    bool is_null();
    bool is_square();

    bool is_diagonal();
    bool is_triangular();
    bool is_lower_triangular();
    bool is_upper_triangular();

    MatrixClass *select_lines(size_t start, size_t end);
    MatrixClass *select_columns(size_t start, size_t end);
    MatrixClass *select_(size_t startLine, size_t endLine, size_t startColumn, size_t endColumn);

    MatrixClass *delete_lines(size_t start, size_t end);
    MatrixClass *delete_columns(size_t start, size_t end);
    MatrixClass *crop(size_t startLine, size_t endLine, size_t startColumn, size_t endColumn);

    float trace();
    float determinent();

    static MatrixClass *add_matrix_matrix(MatrixClass *matrix_A, MatrixClass *matrix_B);
    static MatrixClass *subtract_matrix_matrix(MatrixClass *matrix_A, MatrixClass *matrix_B);
    static MatrixClass *divide_matrix_matrix(MatrixClass *matrix_A, MatrixClass *matrix_B);
    static MatrixClass *multiply_matrix_matrix(MatrixClass *matrix_A, MatrixClass *matrix_B);

    static MatrixClass *add_matrix_float(MatrixClass *matrix, float a);
    static MatrixClass *substract_matrix_float(MatrixClass *matrix, float a);
    static MatrixClass *multiply_matrix_float(MatrixClass *matrix, float a);
    static MatrixClass *divide_matrix_float(MatrixClass *matrix, float a);

    MatrixClass &dot(MatrixClass &matrix);

    static MatrixClass &matrix_multiplication(MatrixClass &matrix_A, MatrixClass &matrix_B);
    static float multiply_matrix_line_matrix_column(MatrixClass &matrix_A, MatrixClass &matrix_B, size_t LINE, size_t COLUMN);

    std::pair<size_t, size_t> min(MatrixType type, size_t *maxI, size_t *maxJ, bool abs = false);
    size_t min_line(size_t line, bool abs = false);
    size_t min_column(size_t column, bool abs = false);

    std::pair<size_t, size_t> max(MatrixType type, bool abs = false);
    size_t max_line(size_t line, bool abs = false);
    size_t max_column(size_t column, bool abs = false);

    // make the print check the max in the column and not in the entire matrix
    void print(size_t precision = 2, bool sign = true);
    void print_line(size_t line, size_t precision = 2, bool sign = true);
    void print_col(size_t column, size_t precision = 2, bool sign = true);
    static void print_matrices_concatenation(size_t n_args, ...);

    MatrixClass *resize(size_t n, size_t m, bool inline_ = false);

    static MatrixClass *replace_lines(MatrixClass *matrix_A, size_t Afrom, size_t Ato, MatrixClass *matrix_B, size_t Bfrom, size_t Bto);
    static MatrixClass *replace_columns(MatrixClass *matrix_A, size_t Afrom, size_t Ato, MatrixClass *matrix_B, size_t Bfrom, size_t Bto);
    static MatrixClass *replace_lines_with(MatrixClass *matrix_A, size_t Afrom, size_t Ato, float number);
    static MatrixClass *replace_columns_with(MatrixClass *matrix_A, size_t Afrom, size_t Ato, float number);

    MatrixClass *select_lines_array(size_t *array, size_t size);
    MatrixClass *select_columns_array(size_t *array, size_t size);
    MatrixClass *select_array(size_t *lines, size_t n_lines, size_t *columns, size_t n_columns);

    MatrixType type();

    static MatrixClass *copy_matrix_in(MatrixClass *dest, MatrixClass *src, size_t startI, size_t startJ, bool destructive = false);

    static std::pair<MatrixClass *, MatrixClass *> lu_decomposition(MatrixClass *matrix);

    /*--operators we need--*/
    /*
        Matrix + Matrix
        Matrix * Matrix (Not Matrix multiplication, matrix multiplication is done with .dot() method)
        Matrix - Matrix
        Matrix / Matrix

        Matrix + number
        Matrix - number
        Matrix * number
        Matrix / number

        Matrix += Matrix
        Matrix -= Matrix
        Matrix /= Matrix
        Matrix *= Matrix

        Matrix[]

        << Matrix <<
    */

    MatrixClass *operator+(MatrixClass *matrix);
    MatrixClass *operator-(MatrixClass *matrix);
    MatrixClass *operator*(MatrixClass *matrix);
    MatrixClass *operator/(MatrixClass *matrix);

    MatrixClass *operator+(float number);
    MatrixClass *operator-(float number);
    MatrixClass *operator*(float number);
    MatrixClass *operator/(float number);

    MatrixClass *operator+=(float number);
    MatrixClass *operator/=(float number);
    MatrixClass *operator*=(float number);
    MatrixClass *operator-=(float number);

    MatrixClass *operator++();
    MatrixClass *operator--();

    float *operator[](size_t i);
};

std::ostream &operator<<(std::ostream &os, MatrixClass &matrix);

#endif