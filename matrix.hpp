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

class MatrixClass
{

public:
    float **content;
    std::size_t n, m;
    std::string name;

    MatrixClass(size_t n, size_t m);

    void print();
    void fill(float number);

    float *operator[](size_t i);

    static MatrixClass &create(size_t n, size_t m);
};

enum MatrixType
{
    NORMAL,
    DIAGONAL,
    UPPER_TRIANGLE,
    LOWER_TRIANGLE,
};
typedef enum MatrixType MatrixType;

struct Matrix
{
    size_t n, m;
    char name[25];
    float **matrix;
};
typedef struct Matrix Matrix;

typedef const std::function<void(size_t, size_t, float)> &Consumer;
typedef const std::function<float(size_t, size_t, float)> &Producer;
typedef const std::function<bool(size_t, size_t, float)> &BooleanProducer;

// implement
Matrix *resize(Matrix *matrix, size_t n, size_t m);

MatrixType matrix_type(Matrix *matrix);

void matrix_srand(unsigned int seed);

bool matrix_all(Matrix *matrix, MatrixType type, BooleanProducer boolean_producer);
bool matrix_line_all(Matrix *matrix, size_t line, BooleanProducer boolean_producer);
bool matrix_column_all(Matrix *matrix, size_t column, BooleanProducer boolean_producer);

bool matrix_one(Matrix *matrix, MatrixType type, BooleanProducer boolean_producer);
bool matrix_line_one(Matrix *matrix, size_t line, BooleanProducer boolean_producer);
bool matrix_column_one(Matrix *matrix, size_t column, BooleanProducer boolean_producer);

bool matrix_map(Matrix *matrix, MatrixType type, Producer consumer);
bool matrix_map_line(Matrix *matrix, size_t line, Producer consumer);
bool matrix_map_column(Matrix *matrix, size_t column, Producer consumer);

bool matrix_for_each(Matrix *matrix, MatrixType type, Consumer consumer);
bool matrix_for_each_line(Matrix *matrix, size_t line, Consumer consumer);
bool matrix_for_each_column(Matrix *matrix, size_t column, Consumer consumer);

Matrix *set_matrix_name(Matrix *matrix, const char *name);

Matrix *create_matrix();
Matrix *copy_matrix(Matrix *matrix);
Matrix *allocate_matrix(size_t n, size_t m);

Matrix *matrix_from_array(MatrixType type, float *array, size_t n, size_t m);

Matrix *create_matrix_with(MatrixType type, size_t n, size_t m, float number);

Matrix *create_matrix_random_int(MatrixType type, size_t n, size_t m, int min = 0, int max = 10);
Matrix *create_matrix_random_float(MatrixType type, size_t n, size_t m, float min = 0, float max = 1);

void create_hilbert_system(size_t n, Matrix **matrix, Matrix **vector);
void create_puissance_system(size_t n, Matrix **matrix, Matrix **vector);

void destroy_matrix(Matrix *matrix);
void destroy_matrices(size_t n_args, ...);

void read_matrix(Matrix *matrix, MatrixType type);

bool print_float(float number, size_t size, size_t precision, char padding, bool sign);

// make the print check the max in the column and not in the entire matrix
void print_matrix(Matrix *matrix, size_t precision = 2, bool sign = true);

void print_matrix_line(Matrix *matrix, size_t line, size_t precision = 2, bool sign = true);

void print_matrix_col(Matrix *matrix, size_t column, size_t precision = 2, bool sign = true);

void print_matrices_concatenation(size_t n_args, ...);

float trace(Matrix *matrix);
float determinent(Matrix *matrix);

Matrix *inverse_matrix(Matrix *matrix);
Matrix *transpose_matrix(Matrix *matrix);
Matrix *cofactor(Matrix *matrix);
Matrix *cofactor_of(Matrix *matrix, size_t line, size_t column);

void gauss(Matrix *matrix, Matrix *vector);
void gauss_totale(Matrix *matrix, Matrix *vector, Matrix **reference);
void gauss_partielle(Matrix *matrix, Matrix *vector);

void lu_decomposition(Matrix *matrix, Matrix **l, Matrix **u);

Matrix *solve_with_cramer(Matrix *matrix, Matrix *vector);
Matrix *solve_upper_triangular(Matrix *matrix, Matrix *vector);
Matrix *solve_lower_triangular(Matrix *matrix, Matrix *vector);

bool is_matrix_triangular(Matrix *matrix);
bool is_matrix_lower_triangular(Matrix *matrix);
bool is_matrix_upper_triangular(Matrix *matrix);

bool matrix_is_null(Matrix *matrix);
bool matrix_is_square(Matrix *matrix);
bool matrix_is_not_null(Matrix *matrix);

Matrix *add_matrix_matrix(Matrix *matrixA, Matrix *matrixB);
Matrix *subtract_matrix_matrix(Matrix *matrixA, Matrix *matrixB);
Matrix *add_matrix_float(Matrix *matrix, float a);
Matrix *substract_matrix_float(Matrix *matrix, float a);
Matrix *multiply_matrix_float(Matrix *matrix, float a);
Matrix *multiply_matrix_matrix(Matrix *matrixA, Matrix *matrixB);

float multiply_matrix_line_matrix_column(Matrix *matrixA, Matrix *matrixB, size_t LINE, size_t COLUMN);

Matrix *concatenate_vertical(size_t n_args, ...);
Matrix *concatenate_horizontal(size_t n_args, ...);

// make them inline, they don't create another matrix for the result they do it directly inside the given matrix
Matrix *permute_lines(Matrix *matrix, size_t L1, size_t L2, bool destructive = false);
Matrix *permute_columns(Matrix *matrix, size_t C1, size_t C2, bool destructive = false);

// add a param to say if we consider the sign or no, default value is (we consider)
void matrix_max(Matrix *matrix, MatrixType type, size_t *maxI, size_t *maxJ, bool abs = false);
size_t matrix_max_line(Matrix *matrix, size_t line, bool abs = false);
size_t matrix_max_column(Matrix *matrix, size_t column, bool abs = false);

void matrix_min(Matrix *matrix, MatrixType type, size_t *maxI, size_t *maxJ, bool abs = false);
size_t matrix_min_line(Matrix *matrix, size_t line, bool abs = false);
size_t matrix_min_column(Matrix *matrix, size_t column, bool abs = false);

Matrix *delete_lines(Matrix *matrix, size_t start, size_t end);
Matrix *delete_columns(Matrix *matrix, size_t start, size_t end);
Matrix *crop(Matrix *matrix, size_t startLine, size_t endLine, size_t startColumn, size_t endColumn);

Matrix *select_lines_array(Matrix *matrix, size_t *array, size_t size);
Matrix *select_columns_array(Matrix *matrix, size_t *array, size_t size);
Matrix *select_array(Matrix *matrix, size_t *lines, size_t n_lines, size_t *columns, size_t n_columns);

Matrix *select_lines(Matrix *matrix, size_t start, size_t end);
Matrix *select_columns(Matrix *matrix, size_t start, size_t end);
Matrix *select_(Matrix *matrix, size_t startLine, size_t endLine, size_t startColumn, size_t endColumn);

Matrix *copy_matrix_in(Matrix *dest, Matrix *src, size_t startI, size_t startJ, bool destructive = false);

Matrix *replace_lines(Matrix *matrixA, size_t Afrom, size_t Ato, Matrix *matrixB, size_t Bfrom, size_t Bto);
Matrix *replace_columns(Matrix *matrixA, size_t Afrom, size_t Ato, Matrix *matrixB, size_t Bfrom, size_t Bto);

#endif