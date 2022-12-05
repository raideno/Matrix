#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <iostream>

#include <functional>

// functions to round, to transform to integer
// functions to modify matrix print format, or pass format allon side with parameters

// add an optional param to say if the function should act destructive or noo

enum MatrixType
{
    NORMAL,
    DIAGONAL,
    UPPER_TRIANGLE,
    LOWER_TRIANGLE,
};
typedef enum MatrixType MatrixType;

typedef const std::function<void(size_t, size_t, float)> &Consumer;
typedef const std::function<float(size_t, size_t, float)> &Producer;
typedef const std::function<bool(size_t, size_t, float)> &BooleanProducer;

class MatrixClass
{

private:
    char name[25];
    float **content;
    std::size_t n, m;
    std::size_t start_line, start_column;

    static float **allocate_matrix(size_t n, size_t m);

public:
    // add matrix element by element multiplication and matrix element by element division

    static void set_allocations_debug(bool value);

    MatrixClass();
    MatrixClass(size_t n, size_t m);

    ~MatrixClass();

    /*implemented*/
    void desallocate();
    void desallocate_matrices(size_t n_args, ...);
    /*---*/

    /*implemented*/
    MatrixClass &set_name(const char *name);

    /*---*/

    /*implemented*/
    float get(size_t i, size_t j);
    /*---*/

    /*---*/

    // creation stuff
    /*----*/
    static MatrixClass &create_matrix(size_t n, size_t m);
    static MatrixClass &matrix_from_array(MatrixType type, float *array, size_t n, size_t m);
    static MatrixClass &create_matrix_with(MatrixType type, size_t n, size_t m, float number);
    static MatrixClass &create_matrix_random_int(MatrixType type, size_t n, size_t m, int min = 0, int max = 10);
    static MatrixClass &create_matrix_random_float(MatrixType type, size_t n, size_t m, float min = 0, float max = 1);
    /*----*/

    /*implemented*/
    MatrixClass &copy();
    static MatrixClass &copy(MatrixClass &matrix);
    static void srand(unsigned int seed);
    /*---*/

    /*implemented*/
    void for_each(MatrixType type, Consumer consumer);
    void for_each_line(size_t line, Consumer consumer);
    void for_each_column(size_t column, Consumer consumer);

    MatrixClass &map(MatrixType type, Producer consumer, bool inplace = false);
    MatrixClass &map_line(size_t line, Producer consumer, bool inplace = false);
    MatrixClass &map_column(size_t column, Producer consumer, bool inplace = false);
    /*---*/

    /*implemented*/
    MatrixClass &permute_lines(size_t L1, size_t L2, bool destructive = false);
    MatrixClass &permute_columns(size_t C1, size_t C2, bool destructive = false);
    /*---*/

    /*implemented*/
    MatrixClass &inverse(bool destructive = false);
    MatrixClass &transpose(bool destructive = false);
    MatrixClass &cofactor();
    MatrixClass &cofactor_of(size_t line, size_t column);
    /*---*/

    /*implemented*/
    bool all(MatrixType type, BooleanProducer boolean_producer);
    bool line_all(size_t line, BooleanProducer boolean_producer);
    bool column_all(size_t column, BooleanProducer boolean_producer);

    bool one(MatrixType type, BooleanProducer boolean_producer);
    bool line_one(size_t line, BooleanProducer boolean_producer);
    bool column_one(size_t column, BooleanProducer boolean_producer);
    /*---*/

    /*implemented*/
    static MatrixClass &concatenate_vertical(size_t n_args, ...);
    static MatrixClass &concatenate_horizontal(size_t n_args, ...);
    /*---*/

    /*implemented*/
    void read(MatrixType type = NORMAL);
    /*---*/

    /*implemented*/
    bool is_null();
    bool is_square();
    /*---*/

    /*implemented*/
    bool is_diagonal();
    bool is_triangular();
    bool is_lower_triangular();
    bool is_upper_triangular();
    /*---*/

    /*implemented*/
    MatrixClass &select_lines(size_t start, size_t end);
    MatrixClass &select_columns(size_t start, size_t end);
    MatrixClass &select_(size_t startLine, size_t endLine, size_t startColumn, size_t endColumn);
    /*---*/

    /*implemented*/
    MatrixClass &delete_lines(size_t start, size_t end);
    MatrixClass &delete_columns(size_t start, size_t end);
    MatrixClass &crop(size_t startLine, size_t endLine, size_t startColumn, size_t endColumn);
    /*---*/

    /*implemented*/
    float trace();
    float determinent();
    /*---*/

    /*implemented*/
    static MatrixClass &add_matrix_matrix(MatrixClass &matrixA, MatrixClass &matrixB);
    static MatrixClass &subtract_matrix_matrix(MatrixClass &matrixA, MatrixClass &matrixB);
    static MatrixClass &add_matrix_with_float(MatrixClass &matrix, float a);
    static MatrixClass &substract_matrix_with_float(MatrixClass &matrix, float a);
    static MatrixClass &multiply_matrix_with_float(MatrixClass &matrix, float a);
    static MatrixClass &divide_matrix_with_float(MatrixClass &matrix, float a);
    /*---*/

    /*implemented*/
    MatrixClass &dot(MatrixClass &matrix);
    static MatrixClass &multiply_matrix_with_matrix(MatrixClass &matrixA, MatrixClass &matrixB);
    static float multiply_matrix_line_matrix_column(MatrixClass &matrixA, MatrixClass &matrixB, size_t LINE, size_t COLUMN);
    /*---*/

    /*implemented*/
    std::pair<size_t, size_t> min(MatrixType type, size_t *maxI, size_t *maxJ, bool abs = false);
    size_t min_line(size_t line, bool abs = false);
    size_t min_column(size_t column, bool abs = false);
    /*---*/

    /*implemented*/
    std::pair<size_t, size_t> max(MatrixType type, bool abs = false);
    size_t max_line(size_t line, bool abs = false);
    size_t max_column(size_t column, bool abs = false);
    /*---*/

    /*implemented*/
    // make the print check the max in the column and not in the entire matrix
    void print(size_t precision = 2, bool sign = true);
    void print_line(size_t line, size_t precision = 2, bool sign = true);
    void print_col(size_t column, size_t precision = 2, bool sign = true);
    static void print_matrices_concatenation(size_t n_args, ...);
    /*---*/

    /*implemented*/
    MatrixClass &resize(size_t n, size_t m, bool inline_ = false);
    /*---*/

    /*implemented*/
    static MatrixClass &replace_lines(MatrixClass &matrixA, size_t Afrom, size_t Ato, MatrixClass &matrixB, size_t Bfrom, size_t Bto);
    static MatrixClass &replace_columns(MatrixClass &matrixA, size_t Afrom, size_t Ato, MatrixClass &matrixB, size_t Bfrom, size_t Bto);
    static MatrixClass &replace_lines_with(MatrixClass &matrixA, size_t Afrom, size_t Ato, float number);
    static MatrixClass &replace_columns_with(MatrixClass &matrixA, size_t Afrom, size_t Ato, float number);
    /*---*/

    /*implemented*/
    MatrixClass &select_lines_array(size_t *array, size_t size);
    MatrixClass &select_columns_array(size_t *array, size_t size);
    MatrixClass &select_array(size_t *lines, size_t n_lines, size_t *columns, size_t n_columns);
    /*---*/

    /*implemented*/
    MatrixType type();
    /*---*/

    // how we gonna do for this ones
    /*
    Matrix *copy_matrix_in(Matrix *dest, Matrix *src, size_t startI, size_t startJ, bool destructive = false);
    */

    /*
    void create_hilbert_system(size_t n, Matrix **matrix, Matrix **vector);
    void create_puissance_system(size_t n, Matrix **matrix, Matrix **vector);

    void gauss(Matrix *matrix, Matrix *vector);
    void gauss_totale(Matrix *matrix, Matrix *vector, Matrix **reference);
    void gauss_partielle(Matrix *matrix, Matrix *vector);

    void lu_decomposition(Matrix *matrix, Matrix **l, Matrix **u);

    Matrix *solve_with_cramer(Matrix *matrix, Matrix *vector);
    Matrix *solve_upper_triangular(Matrix *matrix, Matrix *vector);
    Matrix *solve_lower_triangular(Matrix *matrix, Matrix *vector);
    */

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

        Matrix[]

        Matrix += Matrix
        Matrix -= Matrix
        Matrix /= Matrix
        Matrix *= Matrix

        << Matrix <<
    */

    /*implemented*/
    MatrixClass &operator+(MatrixClass &matrix);
    MatrixClass &operator-(MatrixClass &matrix);
    MatrixClass &operator*(MatrixClass &matrix);
    MatrixClass &operator/(MatrixClass &matrix);

    MatrixClass &operator+(float number);
    MatrixClass &operator-(float number);
    MatrixClass &operator*(float number);
    MatrixClass &operator/(float number);

    MatrixClass &operator+=(float number);
    MatrixClass &operator/=(float number);
    MatrixClass &operator*=(float number);
    MatrixClass &operator-=(float number);

    MatrixClass &operator++();
    MatrixClass &operator--();

    float *operator[](size_t i);
    /*---*/
};

std::ostream &operator<<(std::ostream &os, MatrixClass &matrix);

#endif