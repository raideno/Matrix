#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include <iostream>

#include "matrix.hpp"
#include "../util/util.hpp"

bool MatrixClass::have_center()
{
    auto [n, m] = this->size();

    return !(n % 2 == 0 || m % 2 == 0);
}

std::pair<int, int> MatrixClass::get_center()
{
    auto [n, m] = this->size();

    if (!this->have_center())
        return std::pair(-1, -1);
    else
        return std::pair(((n + 1) / 2) - 1, ((m + 1) / 2) - 1);
}

MatrixClass *MatrixClass::flip()
{
    MatrixClass *result = this->map(
        NORMAL, [this](std::size_t i, std::size_t j, float value) -> float
        { 
            auto [n, m] = this->size();
            return this->get(n - i - 1, m - j - 1); });
    return result;
}

void MatrixClass::before_each(MatrixClass *matrix, const std::string &name)
{
    if (MatrixClass::is_debug_option_set(MatrixDebug::MATRIX_MISC))
        printf(COLOR_BOLD_WHITE "[MatrixClass-before_each]" COLOR_RESET "executed %s for %s matrix\n", name.c_str(), matrix->name.c_str());
}

void MatrixClass::after_each(MatrixClass *matrix, const std::string &name)
{
    if (MatrixClass::is_debug_option_set(MatrixDebug::MATRIX_MISC))
        printf(COLOR_BOLD_WHITE "[MatrixClass-before_each]" COLOR_RESET "finished %s for %s matrix\n", name.c_str(), matrix->name.c_str());
}

MatrixClass *MatrixClass::convolution(MatrixClass *matrix)
{
    return NULL;
}

MatrixClass *MatrixClass::round(bool inplace)
{
    before_each(this, "round");

    MatrixClass *result = this->map(
        MatrixType::NORMAL, [](size_t i, size_t j, float value) -> float
        { return std::round(value); },
        inplace);

    after_each(this, "round");

    return result;
}

const std::string &MatrixClass::get_name()
{
    before_each(this, "get_name");

    after_each(this, "get_name");

    return this->name;
}

/*not set*/
void MatrixClass::set_debug_options(MatrixDebug debug)
{
    MatrixClass::debug_options = debug;
}

/*not set*/
bool MatrixClass::is_debug_option_set(MatrixDebug debug_option)
{
    return (MatrixClass::debug_options & debug_option) == debug_option;
}

void MatrixClass::set(size_t i, size_t j, float element)
{
    before_each(this, "set");

    this->content[i][j] = element;

    after_each(this, "set");
}

// not sure it works
MatrixClass *MatrixClass::select_lines_array(size_t *array, size_t size)
{
    // check that matrix isn't null check that lines in array are valid, cehck that size isn't bigger than the number of lines
    before_each(this, "select_lines_array");

    MatrixClass *result = new MatrixClass(size, this->m);

    for (size_t i = 0; i < size; i++)
        for (size_t j = 0; j < this->m; j++)
            (*result)[i][j] = (*this)[array[i]][j];

    after_each(this, "select_lines_array");

    return result;
}

// not sure it works
MatrixClass *MatrixClass::select_columns_array(size_t *array, size_t size)
{
    // check that matrix isn't null check that lines in array are valid, cehck that size isn't bigger than the number of lines
    before_each(this, "select_columns_array");

    MatrixClass *result = new MatrixClass(this->n, size);

    for (size_t i = 0; i < this->n; i++)
        for (size_t j = 0; j < size; j++)
            (*result)[i][j] = (*this)[i][array[j]];

    after_each(this, "select_lines_column");

    return result;
}

MatrixClass *MatrixClass::select_array(size_t *lines, size_t n_lines, size_t *columns, size_t n_columns)
{
    // do checks

    before_each(this, "select_array");

    MatrixClass *temp;
    MatrixClass *result = NULL;

    temp = this->select_lines_array(lines, n_lines);

    result = this->select_columns_array(columns, n_columns);

    // Todo: make sure temp is properly detroyed
    // destroy_matrix(temp);

    after_each(this, "select_array");

    return result;
}

/*not set*/
std::pair<MatrixClass *, MatrixClass *> MatrixClass::lu_decomposition(MatrixClass *matrix)
{
    // check that matrix is square and compatible, det != 0

    MatrixClass *u = matrix->copy()->set_name("upper");
    MatrixClass *l = (new MatrixClass(matrix->n, matrix->m))->set_name("lower"); // make it's diagonal ones

    l->map(
        DIAGONAL, [](size_t i, size_t j, float element) -> float
        { return 1; },
        true);

    // ajouter le truc en cas ou le pivot = 0 pour permuter entre deux lignes
    // check that dimensions are good

    // printf("gauss:\n\t");

    // check if the system is solvable or not

    for (size_t k = 0; k < matrix->n; k++) // to go through all the pivots
    {
        for (size_t i = k + 1; i < matrix->n; i++) // go though all the lines below the pivot to create zeros
        {
            float a = (*u)[k][k];
            float b = (*u)[i][k];
            float factor = b / a;
            for (size_t j = k; j < matrix->n; j++)
            {
                (*u)[i][j] = (*u)[i][j] - factor * (*u)[k][j];
                (*l)[i][k] = factor;
            }
        }
    }

    return std::pair<MatrixClass *, MatrixClass *>(l, u);
}

/*not set*/
// should we remove the destructive option ?
MatrixClass *MatrixClass::copy_matrix_in(MatrixClass *dest, MatrixClass *src, size_t startI, size_t startJ, bool destructive)
{

    MatrixClass *result = destructive ? dest : dest->copy();

    for (size_t i = startI; i < startI + src->n; i++)
        for (size_t j = startJ; j < startJ + src->m; j++)
            (*result)[i][j] = src->get(i - startI, j - startJ);

    return result;
}

std::pair<std::size_t, std::size_t> MatrixClass::size()
{
    return std::pair<std::size_t, std::size_t>(this->n, this->m);
}

MatrixType MatrixClass::type()
{
    // we check if it's a diagonal matrix
    // we check if it's a lower triangular / upper triangular matrix
    // otherwise it's a normal matrix

    // check if it's a all zero matrix and return NORMAL if it's the case
    before_each(this, "type");

    bool allZeroes;
    bool isDiagonal = true;
    bool isLowerTriangular = true;
    bool isUpperTriangular = true;
    bool isNormal = true;

    for (size_t i = 0; i < this->n; i++)
        for (size_t j = 0; j < this->m; j++)
            if ((*this)[i][j] != 0)
                allZeroes = false;

    if (allZeroes)
        return NORMAL;

    if (this->n != this->m)
        return NORMAL;

    // DIAGONAL
    for (size_t i = 0; i < this->n; i++)
        for (size_t j = 0; j < this->m; j++)
            if (i != j && (*this)[i][j] != 0)
                isDiagonal = false;

    // check that diagonal is filled with some stuff and it's not only a 0 matrix

    if (isDiagonal)
        return DIAGONAL;

    // LOWER TRIANGLE
    for (size_t i = 0; i < this->n; i++)
        for (size_t j = i + 1; j < this->m; j++)
            if ((*this)[i][j] != 0)
                isLowerTriangular = false;

    if (isLowerTriangular)
        return LOWER_TRIANGLE;

    // UPPER TRIANGLE

    for (size_t i = 0; i < this->n; i++)
        for (size_t j = 0; j < i - 1; j++)
            if ((*this)[i][j] != 0)
                isUpperTriangular = false;

    if (isUpperTriangular)
        return LOWER_TRIANGLE;

    after_each(this, "type");

    return NORMAL;
}

/*not set*/
MatrixClass *MatrixClass::replace_lines(MatrixClass *matrix_A, size_t Afrom, size_t Ato, MatrixClass *matrix_B, size_t Bfrom, size_t Bto)
{
    MatrixClass *result;
    size_t size = Ato - Afrom + 1;

    if (matrix_A->m != matrix_B->m)
    {
        printf("[replace_lines]: the two matrices aren't compatible\n");
        return NULL;
    }

    if ((Ato - Afrom != Bto - Bfrom) || (Afrom > Ato || Bfrom > Bto) || (Afrom > matrix_A->n - 1 || Bfrom > matrix_B->n - 1 || Ato > matrix_A->n - 1 || Bto > matrix_B->n - 1))
    {
        printf("[replace_lines]: invalid params\n");
        return NULL;
    }

    result = matrix_A->copy();

    for (size_t i = Bfrom; i < size; i++)
        for (size_t j = 0; j < matrix_A->m; j++)
            (*result)[i - Bfrom + Afrom][j] = matrix_B->get(i, j);

    return result;
}

MatrixClass *MatrixClass::replace_columns(MatrixClass *matrix_A, size_t Afrom, size_t Ato, MatrixClass *matrix_B, size_t Bfrom, size_t Bto)
{
    MatrixClass *result;
    size_t size = Ato - Afrom + 1;

    if (matrix_A->n != matrix_B->n)
    {
        printf("[replace_columns]: the two matrices aren't compatible\n");
        return NULL;
    }

    if ((Ato - Afrom != Bto - Bfrom) || (Afrom > Ato || Bfrom > Bto) || (Afrom > matrix_A->m - 1 || Bfrom > matrix_B->m - 1 || Ato > matrix_A->m - 1 || Bto > matrix_B->m - 1))
    {
        printf("[replace_columns]: invalid params\n");
        return NULL;
    }

    result = matrix_A->copy();

    for (size_t i = 0; i < matrix_A->n; i++)
        for (size_t j = Bfrom; j < size; j++)
            (*result)[i][j - Bfrom + Afrom] = matrix_B->get(i, j);

    return result;
}

/*not set*/
MatrixClass *MatrixClass::replace_lines_with(MatrixClass *matrix_A, size_t Afrom, size_t Ato, float number)
{
    MatrixClass *result;

    if ((Afrom > Ato) || (Afrom > matrix_A->n - 1 || Ato > matrix_A->n - 1))
    {
        printf("[replace_lines]: invalid params\n");
        return NULL;
    }

    result = matrix_A->copy();

    for (size_t i = Ato; i <= Afrom; i++)
        for (size_t j = 0; j < matrix_A->m; j++)
            (*result)[i][j] = number;

    return result;
}

/*not set*/
MatrixClass *MatrixClass::replace_columns_with(MatrixClass *matrix_A, size_t Afrom, size_t Ato, float number)
{
    MatrixClass *result;

    if ((Afrom > Ato) || (Afrom > matrix_A->m - 1 || Ato > matrix_A->m - 1))
    {
        printf("[replace_columns]: invalid params\n");
        return NULL;
    }

    result = matrix_A->copy();

    for (size_t i = 0; i < matrix_A->n; i++)
        for (size_t j = Afrom; j <= Ato; j++)
            (*result)[i][j] = number;

    return result;
}

MatrixClass *MatrixClass::resize(size_t n, size_t m, bool inline_)
{
    before_each(this, "resize");
    // check that it's not negatif values

    if (inline_)
    {
        MatrixClass *save = this->copy()->set_name("resize-save");

        MatrixClass::desallocate(this);

        this->allocate_matrix(n, m);

        for (size_t i = 0; i < std::min(this->n, n); i++)
            for (size_t j = 0; j < std::min(this->m, m); j++)
                (*this)[i][j] = save->get(i, j);

        this->n = n;
        this->m = m;

        // Todo: destroy save

        after_each(this, "resize");

        return this;
    }
    else
    {
        MatrixClass *result = new MatrixClass(n, m);

        for (size_t i = 0; i < std::min(this->n, n); i++)
            for (size_t j = 0; j < std::min(this->m, m); j++)
                (*result)[i][j] = (*this)[i][j];

        after_each(this, "resize");

        return result;
    }
}

/*not set*/
void MatrixClass::desallocate_matrices(size_t n_args, ...)
{
    va_list ap;
    MatrixClass *matrix;

    va_start(ap, n_args);

    for (size_t i = 0; i < n_args; i++)
    {
        matrix = va_arg(ap, MatrixClass *);
        MatrixClass::desallocate(matrix);
    }

    va_end(ap);
}

/*not set*/
void MatrixClass::desallocate(MatrixClass *matrix)
{
    if (matrix->content == NULL)
        return;

    for (size_t i = 0; i < matrix->n; i++)
        if (matrix->content[i] != NULL)
            free(matrix->content[i]);

    free(matrix->content);

    /*should remove it*/
    matrix->content = NULL;
    matrix->n = 0;
    matrix->m = 0;
    /*---*/

    if (MatrixClass::is_debug_option_set(MatrixDebug::MATRIX_DESALLOCATION))
        printf(COLOR_RED "[MatrixClass-desallocate]:" COLOR_RESET "a matrix " COLOR_UNDERLINE "(%s)" COLOR_RESET " got desallocated\n", matrix->name.length() == 0 ? "/" : matrix->name.c_str());

    MatrixClass::desallocated_matrices++;

    // Todo: recheck this one
    // Not sure about this one
    // free(this);
}

MatrixClass *MatrixClass::set_name(const std::string &name)
{
    before_each(this, "set_name");

    this->name = name;

    after_each(this, "set_name");

    return this;
}

MatrixClass::~MatrixClass()
{
    before_each(this, "~MatrixClass()");

    /*
        if (MatrixClass::is_debug_option_set(MatrixDebug::MATRIX_DESTRUCTION))
            printf(COLOR_RED "[~MatrixClass]:" COLOR_RESET "destroying a matrix " COLOR_UNDERLINE "(%s)" COLOR_RESET "\n", this->name.length() == 0 ? "/" : this->name.c_str());
    */

    /*
    if (MatrixClass::is_debug_option_set(MatrixDebug::MISC))
        printf(COLOR_YELLOW "[~MatrixClass]:" COLOR_RESET "%p %ld %ld(content, n, m)\n", this->content, this->n, this->m);
    */

    if (this->content == NULL)
    {
        printf(COLOR_BOLD_RED "[~MatrixClass]:" COLOR_RESET "matrix " COLOR_UNDERLINE "(%s)" COLOR_RESET " already got destroyed\n", this->name.length() == 0 ? "/" : this->name.c_str());
        this->n = 0;
        this->m = 0;
        return;
    }

    MatrixClass::desallocate(this);

    this->n = 0;
    this->m = 0;
    this->content = NULL;

    if (MatrixClass::is_debug_option_set(MatrixDebug::MATRIX_DESTRUCTION))
        printf(COLOR_RED "[~MatrixClass]:" COLOR_RESET "a matrix " COLOR_UNDERLINE "(%s)" COLOR_RESET " got destroyed\n", this->name.length() == 0 ? "/" : this->name.length() == 0 ? "/"
                                                                                                                                                                                    : this->name.c_str());

    MatrixClass::destroyed_matrices++;

    if (MatrixClass::is_debug_option_set(MatrixDebug::MATRIX_MISC))
        printf(COLOR_YELLOW "[~MatrixClass-]:" COLOR_RESET "%ld, " COLOR_BOLD_WHITE "%ld" COLOR_RESET ", %ld, " COLOR_BOLD_WHITE "%ld" COLOR_RESET "(created, allocated, destroyed, desallocated)\n", MatrixClass::created_matrices, MatrixClass::allocated_matrices, MatrixClass::destroyed_matrices, MatrixClass::desallocated_matrices);

    after_each(this, "~MatrixClass()");
}

// make the print check the max in the column and not in the entire matrix
void MatrixClass::print(size_t precision, bool sign)
{
    before_each(this, "print");

    size_t size;
    std::pair<size_t, size_t> max;

    if (this->name.length() > 0)
        std::cout << this->name << ":" << std::endl;

    max = this->max(NORMAL);
    size = count_digits(this->get(max.first, max.second));

    printf("[\n");
    for (size_t i = 0; i < this->n; i++)
    {
        printf("   [");
        for (size_t j = 0; j < this->m; j++)
        {
            print_float(this->get(i, j), size, precision, ' ', sign);
            printf("%s", j == this->m - 1 ? "\0" : ", ");
        }
        printf("]\n");
    }
    printf("]\n");

    after_each(this, "print");
}

void MatrixClass::print_line(size_t line, size_t precision, bool sign)
{
    before_each(this, "print_line");

    size_t size;
    std::pair<size_t, size_t> max;

    if (line > this->n - 1)
    {
        printf("[print_matrix_line]: invalid line provided\n");
        return;
    }

    max = this->max(NORMAL);
    size = count_digits(this->get(max.first, max.second));

    printf("[");
    for (size_t j = 0; j < this->m; j++)
    {
        // printf("%.2f", matrix->matrix[line][j]);
        print_float(this->get(line, j), size, precision, ' ', sign);
        printf("%s", j == this->m - 1 ? "\0" : ", ");
    }
    printf("]");

    after_each(this, "print_line");
}

void MatrixClass::print_col(size_t column, size_t precision, bool sign)
{
    before_each(this, "print_col");

    size_t size;
    std::pair<size_t, size_t> max;

    if (column > this->m - 1)
    {
        printf("[print_matrix_col]: invalid column provided\n");
        return;
    }

    max = this->max(NORMAL);
    size = count_digits(this->get(max.first, max.second));

    printf("[");
    for (size_t i = 0; i < this->n; i++)
    {
        print_float(this->get(i, column), size, precision, ' ', sign);
        printf("%s", i == this->n - 1 ? "\0" : ", ");
    }
    printf("]");

    after_each(this, "print_col");
}

/*not set*/
// ca serai pas plus inteligent d'afficher colonne par colonne ?
void MatrixClass::print_matrices_concatenation(size_t n_args, ...)
{
    int lines;
    va_list ap;
    MatrixClass *matrix;

    va_start(ap, n_args);

    matrix = va_arg(ap, MatrixClass *);
    lines = matrix->n;

    for (int i = 1; i < n_args; i++)
    {
        if (matrix == NULL)
        {
            printf("[print_matrices_concatenation]: null matrix was provided\n");
            return;
        }
        if (matrix->n > lines)
            lines = matrix->n;
        matrix = va_arg(ap, MatrixClass *); // mets la en premiere ligne si jamais probleme
    }
    va_end(ap);

    for (size_t i = 0; i < lines; i++)
    {
        va_start(ap, n_args);
        for (size_t j = 0; j < n_args; j++)
        {
            matrix = va_arg(ap, MatrixClass *);

            if (i < matrix->n)
                matrix->print_line(i, 2, true);
            // print_matrix_line(matrix, i, 2, true);
            else
                printf("\t");
            printf("\t");
        }
        va_end(ap);
        printf("\n");
    }
}

/*not set*/
std::ostream &operator<<(std::ostream &os, MatrixClass &matrix)
{

    matrix.print();

    return os;
}

float MatrixClass::get(size_t i, size_t j, float default_return_value)
{
    before_each(this, "get");

    if (i > this->n - 1)
    {
        if (MatrixClass::is_debug_option_set(MatrixDebug::MATRIX_MISC))
            printf(COLOR_BOLD_WHITE "[get]:" COLOR_RESET "invalid i provided\n");
        return default_return_value;
    }

    if (j > this->m - 1)
    {
        if (MatrixClass::is_debug_option_set(MatrixDebug::MATRIX_MISC))
            printf(COLOR_BOLD_WHITE "[get]:" COLOR_RESET "invalid i provided\n");
        return default_return_value;
    }

    after_each(this, "get");

    return this->content[i][j];
}

/*not set*/
MatrixClass *MatrixClass::divide_matrix_matrix(MatrixClass *matrix_A, MatrixClass *matrix_B)
{
    MatrixClass *result;

    if (matrix_A->size() != matrix_B->size())
    {
        printf("[divide_matrix_matrix]: matrices have to be of the same dimensions\n");
        return NULL;
    }

    result = new MatrixClass(matrix_A->n, matrix_A->m);

    result->map(
        MatrixType::NORMAL, [&matrix_A, &matrix_B](size_t i, size_t j, float element) -> float
        { return (matrix_A->get(i, j) / matrix_B->get(i, j)); },
        true);

    return result;
}

/*not set*/
MatrixClass *MatrixClass::multiply_matrix_matrix(MatrixClass *matrix_A, MatrixClass *matrix_B)
{
    MatrixClass *result;

    if (matrix_A->size() != matrix_B->size())
    {
        printf("[multiply_matrix_matrix]: matrices have to be of the same dimensions\n");
        return NULL;
    }

    result = new MatrixClass(matrix_A->n, matrix_A->m);

    result->map(
        MatrixType::NORMAL, [&matrix_A, &matrix_B](size_t i, size_t j, float element) -> float
        { return matrix_A->get(i, j) * matrix_B->get(i, j); },
        true);

    return result;
}

MatrixClass *MatrixClass::operator+(MatrixClass *matrix)
{
    before_each(this, "operator");

    MatrixClass *result = MatrixClass::add_matrix_matrix(this, matrix);

    after_each(this, "operator+");

    return result;
}

MatrixClass *MatrixClass::operator-(MatrixClass *matrix)
{
    before_each(this, "operator");

    MatrixClass *result = MatrixClass::subtract_matrix_matrix(this, matrix);

    after_each(this, "operator");

    return result;
}

MatrixClass *MatrixClass::operator*(MatrixClass *matrix)
{
    before_each(this, "operator");

    MatrixClass *result = MatrixClass::multiply_matrix_matrix(this, matrix);

    after_each(this, "operator");

    return result;
}

MatrixClass *MatrixClass::operator/(MatrixClass *matrix)
{
    before_each(this, "operator");

    MatrixClass *result = MatrixClass::divide_matrix_matrix(this, matrix);

    after_each(this, "operator");

    return result;
}

MatrixClass *MatrixClass::operator+(float number)
{
    before_each(this, "operator");

    MatrixClass *result = MatrixClass::add_matrix_float(this, number);

    after_each(this, "operator");

    return result;
}

MatrixClass *MatrixClass::operator-(float number)
{
    before_each(this, "operator");

    MatrixClass *result = MatrixClass::substract_matrix_float(this, number);

    after_each(this, "operator");

    return result;
}

MatrixClass *MatrixClass::operator*(float number)
{
    before_each(this, "operator");

    MatrixClass *result = MatrixClass::multiply_matrix_float(this, number);

    after_each(this, "operator");

    return result;
}

MatrixClass *MatrixClass::operator/(float number)
{
    before_each(this, "operator");

    MatrixClass *result = MatrixClass::divide_matrix_float(this, number);

    after_each(this, "operator");

    return result;
}

/*
MatrixClass &MatrixClass::operator+=(float number)
{
}

MatrixClass &MatrixClass::operator/=(float number)
{
}

MatrixClass &MatrixClass::operator*=(float number)
{
}

MatrixClass &MatrixClass::operator-=(float number)
{
}
*/

MatrixClass *MatrixClass::operator++()
{
    before_each(this, "operator");

    auto result = MatrixClass::add_matrix_float(this, 1);

    after_each(this, "operator");

    return result;
}

MatrixClass *MatrixClass::operator--()
{
    before_each(this, "operator");

    auto result = MatrixClass::substract_matrix_float(this, 1);

    after_each(this, "operator");

    return result;
}

float *MatrixClass::operator[](size_t i)
{
    before_each(this, "operator");

    auto result = this->content[i];

    after_each(this, "operator");

    return result;
}

std::pair<size_t, size_t> MatrixClass::max(MatrixType type, bool absolute)
{
    before_each(this, "max");

    std::pair<size_t, size_t> max(0, 0);

    this->for_each(type, [&absolute, &matrix = *this, &max](size_t i, size_t j, float element) -> void
                   { 
                        if (absolute) {
                            if (abs(element) > abs(matrix[max.first][max.second])) {
                                max.first = i;
                                max.second = j;
                            }
                        } else {
                                if (element > matrix[max.first][max.second]) {
                                max.first = i;
                                max.second = j;
                            }
                        } });

    after_each(this, "max");

    return max;
}

size_t MatrixClass::max_line(size_t line, bool abs)
{
    before_each(this, "max_line");

    size_t mJ = 0;

    this->for_each_line(line, [&matrix = *this, &line, &mJ](size_t i, size_t j, float element) -> void
                        {
        if (element > matrix[line][mJ])
            mJ = j; });

    after_each(this, "max_line");

    return mJ;
}

size_t MatrixClass::max_column(size_t column, bool absolute)
{
    before_each(this, "max_column");

    size_t mI = 0;

    this->for_each_column(column, [&matrix = *this, &absolute, &column, &mI](size_t i, size_t j, float element) -> void
                          {
                            if (!absolute)
                            {
                                if (element > matrix[mI][column])
                                    mI = i;
                            } else {
                                if (abs(element) > abs(matrix[mI][column]))
                                    mI = i;
                            } });

    after_each(this, "max_column");

    return mI;
}

std::pair<size_t, size_t> MatrixClass::min(MatrixType type, size_t *maxI, size_t *maxJ, bool absolute)
{
    before_each(this, "min");

    std::pair<size_t, size_t> max(0, 0);

    this->for_each(type, [&absolute, &matrix = *this, &max](size_t i, size_t j, float element) -> void
                   { 
                        if (absolute) {
                            if (abs(element) < abs(matrix[max.first][max.second])) {
                                max.first = i;
                                max.second = j;
                            }
                        } else {
                                if (element < matrix[max.first][max.second]) {
                                max.first = i;
                                max.second = j;
                            }
                        } });

    after_each(this, "min");

    return max;
}

size_t MatrixClass::min_line(size_t line, bool absolute)
{
    before_each(this, "min_line");

    size_t mJ = 0;

    this->for_each_line(line, [&absolute, &matrix = *this, &line, &mJ](size_t i, size_t j, float element) -> void
                        {
                            if (!absolute)
                            {
                                if (element < matrix[line][mJ])
                                    mJ = j;
                            } else {
                                if (abs(element) < abs(matrix[line][mJ]))
                                    mJ = j;
                            } });

    after_each(this, "min_line");

    return mJ;
}

size_t MatrixClass::min_column(size_t column, bool absolute)
{
    before_each(this, "min_column");

    size_t mI = 0;

    this->for_each_column(column, [&matrix = *this, &absolute, &column, &mI](size_t i, size_t j, float element) -> void
                          {
                            if (!absolute)
                            {
                                if (element < matrix[mI][column])
                                    mI = i;
                            } else {
                                if (abs(element) < abs(matrix[mI][column]))
                                    mI = i;
                            } });

    after_each(this, "min_column");

    return mI;
}

MatrixClass *MatrixClass::dot(MatrixClass *matrix)
{
    before_each(this, "dot");

    auto result = MatrixClass::matrix_multiplication(this, matrix);

    after_each(this, "dot");

    return result;
}

/*not set*/
MatrixClass *MatrixClass::matrix_multiplication(MatrixClass *matrix_A, MatrixClass *matrix_B)
{
    MatrixClass *result;

    if (matrix_A->m != matrix_B->n)
    {
        printf("[multiply_matrix_matrix]: number of lines and number of columns must be equal\n");
        return NULL;
    }

    result = new MatrixClass(matrix_A->n, matrix_B->m);

    for (size_t i = 0; i < matrix_A->n; i++)
        for (size_t j = 0; j < matrix_B->m; j++)
            (*result)[i][j] = multiply_matrix_line_matrix_column(matrix_A, matrix_B, i, j);

    return result;
}

/*not set*/
float MatrixClass::multiply_matrix_line_matrix_column(MatrixClass *matrix_A, MatrixClass *matrix_B, size_t LINE, size_t COLUMN)
{
    float result = 0;

    if (LINE > matrix_A->n - 1 || COLUMN > matrix_B->m - 1)
    {
        printf("[multiply_matrix_line_matrix_column]: given an invalid LINE or COLUMN\n");
        return 0;
    }

    for (size_t i = 0; i < matrix_A->m; i++)
        result += (matrix_A->get(LINE, i) * matrix_B->get(i, COLUMN));

    return result;
}

/*optimiser la fonction pour selectionner la ligne avec le plus de zero !*/
/*faire une fonction qui trouve la ligne ou il y ale plus de zero*/
/*faire une fonction qui trouve la cologne ou il y a le plus de zero*/
float MatrixClass::determinent()
{
    before_each(this, "determinent");

    float result = 0;

    if (!this->is_square())
    {
        printf("[determinent]: can't compute determinent of a non square matrix\n");
        return 0;
    }

    if (this->n == 1)
        return (*this)[0][0];

    if (this->n == 2)
        return ((*this)[0][0] * (*this)[1][1]) - ((*this)[0][1] * (*this)[1][0]);

    for (size_t i = 0; i < this->n; i++)
    {
        MatrixClass *cofact = this->cofactor_of(0, i)->set_name("cofactor-matrix");
        /*0 in 0 + i in power fonction is for the j (i) but since we selected the first line*/
        /*should we let the "+1" or no ? or replace it with a "+1" ? and why exactly "+2" ? why it wouldn't work normaly ?*/
        result += std::pow(-1, 0 + i + 2) * (*this)[0][i] * cofact->determinent();

        delete cofact;
        // TODO: fix the matrix destruction
        // delete &cofact;
        // destroy_matrix(cofact);
    }

    after_each(this, "determinent");

    return result;
}

float MatrixClass::trace()
{
    before_each(this, "trace");

    float result = 0;

    if (this->is_square())
    {
        printf("[trace]: matrix is not square\n");
        return 0;
    }

    this->for_each(DIAGONAL, [&result](size_t i, size_t j, float element) -> void
                   { result += element; });

    after_each(this, "trace");

    return result;
}

MatrixClass *MatrixClass::select_lines(size_t start, size_t end)
{
    before_each(this, "select_lines");

    if (start > end)
    {
        printf("[select_lines]: start is bigger than end\n");
        return NULL;
    }

    if (start > this->n - 1 || end > this->n - 1)
    {
        printf("[select_lines]: received invalid startLine or endLine\n");
        return NULL;
    }

    size_t lines = end - start + 1;
    MatrixClass *result = new MatrixClass(lines, this->m);

    for (size_t i = start; i <= end; i++)
        for (size_t j = 0; j < this->m; j++)
            (*result)[i - start][j] = (*this)[i][j];

    after_each(this, "select_lines");

    return result;
}

MatrixClass *MatrixClass::select_columns(size_t start, size_t end)
{
    // check that start < end
    before_each(this, "select_columns");

    if (start > end)
    {
        printf("[select_lines]: start is bigger than end\n");
        return NULL;
    }

    if (start > this->m - 1 || end > this->m - 1)
    {
        printf("[select_columns]: received invalid startColumn or endColumn\n");
        return NULL;
    }

    size_t columns = end - start + 1;
    MatrixClass *result = new MatrixClass(this->n, columns);

    for (size_t i = 0; i < this->n; i++)
        for (size_t j = start; j <= end; j++)
            (*result)[i][j - start] = (*this)[i][j];

    after_each(this, "select_columns");

    return result;
}

MatrixClass *MatrixClass::select_(size_t startLine, size_t endLine, size_t startColumn, size_t endColumn)
{
    before_each(this, "select_");

    MatrixClass *temp;
    MatrixClass *result;

    temp = this->select_lines(startLine, endLine);

    result = temp->select_columns(startColumn, endColumn);

    // TODO:

    delete temp;
    // destroy_matrix(temp);

    after_each(this, "select_");

    return result;
}

MatrixClass *MatrixClass::delete_lines(size_t start, size_t end)
{
    before_each(this, "delete_lines");

    if (start > this->n - 1 || end > this->n - 1)
    {
        printf("[delete_lines]: received invalid startLine or endLine\n");
        return NULL;
    }

    size_t lines = this->n - (end - start + 1);
    MatrixClass *result = new MatrixClass(lines, this->m);

    for (size_t i = 0; i < start; i++)
        for (size_t j = 0; j < this->m; j++)
            (*result)[i][j] = (*this)[i][j];

    for (size_t i = end + 1; i < this->n; i++)
        for (size_t j = 0; j < this->m; j++)
            (*result)[i - end][j] = (*this)[i][j];

    after_each(this, "delete_lines");

    return result;
}

MatrixClass *MatrixClass::delete_columns(size_t start, size_t end)
{

    before_each(this, "delete_columns");

    if (start > this->m - 1 || end > this->m - 1)
    {
        printf("[delete_columns]: received invalid startColumn or endColumn\n");
        return NULL;
    }

    size_t columns = this->m - (end - start + 1);
    MatrixClass *result = new MatrixClass(this->n, columns);

    for (size_t i = 0; i < this->n; i++)
    {
        for (size_t j = 0; j < start; j++)
            (*this)[i][j] = (*this)[i][j];

        for (size_t j = end + 1; j < this->m; j++)
            (*this)[i][j - end] = (*this)[i][j];
    }

    after_each(this, "delete_columns");

    return result;
}

MatrixClass *MatrixClass::crop(size_t startLine, size_t endLine, size_t startColumn, size_t endColumn)
{
    before_each(this, "crop");

    MatrixClass *temp;
    MatrixClass *result;

    temp = this->delete_lines(startLine, endLine);

    // printf("intermediare:\n");
    // print_matrix(temp);

    result = temp->delete_columns(startColumn, endColumn);

    // TODO: fix the delete process
    delete temp;

    // destroy_matrix(temp);

    after_each(this, "crop");

    return result;
}

bool MatrixClass::is_diagonal()
{
    before_each(this, "is_diagonal");

    if (!this->is_square())
        return false;

    for (size_t i = 0; i < this->n; i++)
    {
        for (size_t j = 0; j < this->m; j++)
        {
            if (i != j && (*this)[i][j] != 0)
                return false;
        }
    }

    after_each(this, "is_diagonal");

    return true;
}

bool MatrixClass::is_triangular()
{
    before_each(this, "is_triangular");

    if (!this->is_square())
        return false;

    after_each(this, "is_triangular");

    return this->is_lower_triangular() || this->is_upper_triangular();
}

bool MatrixClass::is_lower_triangular()
{
    before_each(this, "is_lower_triangular");

    if (!this->is_square())
        return false;

    for (size_t i = 0; i < this->n; i++)
        for (size_t j = i + 1; j < this->m; j++)
            if ((*this)[i][j] != 0)
                return false;

    after_each(this, "is_lower_triangular");

    return true;
}

bool MatrixClass::is_upper_triangular()
{
    before_each(this, "is_upper_triangular");

    if (!this->is_square())
        return false;

    for (size_t i = 0; i < this->n; i++)
        for (size_t j = 0; j < i; j++)
            if ((*this)[i][j] != 0)
                return false;

    after_each(this, "is_upper_triangular");

    return true;
}

bool MatrixClass::is_null()
{
    before_each(this, "is_null");

    after_each(this, "is_null");

    return this->content == NULL;
}

bool MatrixClass::is_square()
{
    before_each(this, "is_square");

    // TODO: check that it's not null also
    after_each(this, "is_square");

    return this->n == this->m;
}

void MatrixClass::read(MatrixType type)
{
    before_each(this, "read");

    if ((type == DIAGONAL || type == UPPER_TRIANGLE || type == LOWER_TRIANGLE) && !this->is_square())
    {
        printf("[read_matrix]: matrix must be a square matrix for this type\n");
        return;
    }

    this->map(
        type, [](size_t i, size_t j, float number) -> float
        {
            float result;
            printf("matrix[%ld][%ld]=", i, i);
            scanf("%f", &result);
            return result; },
        true);

    after_each(this, "read");
}

/*not set*/
MatrixClass *MatrixClass::concatenate_vertical(size_t n_args, ...)
{

    // if one of the params is NULL, or invalid we destroy the allocated matrice + print an error
    va_list ap;
    MatrixClass *matrix;
    MatrixClass *result;
    MatrixClass **matrices = (MatrixClass **)calloc(n_args, sizeof(MatrixClass *));

    int lines = 0;
    int columns = 0;
    int progress = 0;

    va_start(ap, n_args);

    for (int i = 0; i < n_args; i++)
    {
        matrix = va_arg(ap, MatrixClass *);
        matrices[i] = matrix;
        if (matrices[i]->n > lines)
            lines = matrices[i]->n;
        columns += matrices[i]->m;
    }
    va_end(ap);

    result = new MatrixClass(lines, columns);

    for (size_t k = 0; k < n_args; k++)
    {
        for (size_t i = 0; i < matrices[k]->n; i++)
        {
            for (size_t j = 0; j < matrices[k]->m; j++)
            {
                (*result)[i][j + progress] = (*matrices[k])[i][j];
            }
        }
        progress += matrices[k]->m;
    }

    return result;
}

/*not set*/
MatrixClass *MatrixClass::concatenate_horizontal(size_t n_args, ...)
{
    // if one of the params is NULL, or invalid we destroy the allocated matrice + print an error

    va_list ap;
    MatrixClass *matrix;
    MatrixClass *result;
    MatrixClass **matrices = (MatrixClass **)calloc(n_args, sizeof(MatrixClass *));

    int lines = 0;
    int columns = 0;
    int progress = 0;

    va_start(ap, n_args);

    for (int i = 0; i < n_args; i++)
    {
        matrix = va_arg(ap, MatrixClass *);
        matrices[i] = matrix;
        if (matrices[i]->n > columns)
            columns = matrices[i]->m;
        lines += matrices[i]->n;
    }
    va_end(ap);

    result = new MatrixClass(lines, columns);

    for (size_t k = 0; k < n_args; k++)
    {
        for (size_t i = 0; i < matrices[k]->n; i++)
        {
            for (size_t j = 0; j < matrices[k]->m; j++)
            {
                (*result)[i + progress][j] = (*matrices[k])[i][j];
            }
        }
        progress += matrices[k]->n;
    }

    return result;
}

bool MatrixClass::one(MatrixType type, BooleanProducer boolean_producer)
{
    before_each(this, "one");

    bool result = false;

    this->for_each(type, [&boolean_producer, &result](size_t i, size_t j, float element) -> void
                   {
        if (boolean_producer(i, j, element))
            result = true; });

    after_each(this, "one");

    return result;
}

float MatrixClass::reduce(MatrixType type, Reducer reducer, float initialValue)
{
    before_each(this, "reduce");

    float result = initialValue;

    this->for_each(type, [&reducer, &result](size_t i, size_t j, float value) -> void
                   { result = reducer(result, i, j, value); });

    after_each(this, "reduce");

    return result;
}

float MatrixClass::reduce_line(size_t line, Reducer reducer, float initialValue)
{
    before_each(this, "reduce_line");

    float result = initialValue;

    this->for_each_line(line, [&reducer, &result](size_t i, size_t j, float value) -> void
                        { result += reducer(result, i, j, value); });

    after_each(this, "reduce_line");

    return result;
}

float MatrixClass::reduce_column(size_t column, Reducer reducer, float initialValue)
{
    before_each(this, "reduce_column");

    float result = initialValue;

    this->for_each_column(column, [&reducer, &result](size_t i, size_t j, float value) -> void
                          { result += reducer(result, i, j, value); });

    after_each(this, "reduce_column");

    return result;
}

bool MatrixClass::line_one(size_t line, BooleanProducer boolean_producer)
{
    before_each(this, "line_one");

    for (size_t j = 0; j < this->m; j++)
        if (boolean_producer(line, j, (*this)[line][j]))
            return true;

    after_each(this, "line_one");

    return false;
}

bool MatrixClass::column_one(size_t column, BooleanProducer boolean_producer)
{
    before_each(this, "column_one");

    for (size_t i = 0; i < this->n; i++)
        if (boolean_producer(i, column, (*this)[i][column]))
            return true;

    after_each(this, "column_one");

    return false;
}

bool MatrixClass::all(MatrixType type, BooleanProducer boolean_producer)
{
    before_each(this, "all");

    bool result = true;

    this->for_each(type, [&boolean_producer, &result](size_t i, size_t j, float element) -> void
                   {
        if (!boolean_producer(i, j, element))
            result = false; });

    after_each(this, "all");

    return result;
}

bool MatrixClass::line_all(size_t line, BooleanProducer boolean_producer)
{
    before_each(this, "line_all");

    for (size_t j = 0; j < this->m; j++)
        if (!boolean_producer(line, j, (*this)[line][j]))
            return false;

    after_each(this, "line_all");

    return true;
}

bool MatrixClass::column_all(size_t column, BooleanProducer boolean_producer)
{
    before_each(this, "column_all");

    for (size_t i = 0; i < this->n; i++)
        if (!boolean_producer(i, column, (*this)[i][column]))
            return false;

    after_each(this, "column_all");

    return true;
}

MatrixClass *MatrixClass::inverse(bool destructive)
{
    before_each(this, "inverse");

    float det;
    MatrixClass *result;
    MatrixClass *cofactor_matrix;
    MatrixClass *transpose_cofactor_matrix;

    det = this->determinent();

    if (det == 0)
    {
        printf("[inverse_matrix]: determinant is equal to 0\n");
        return NULL;
    }

    cofactor_matrix = this->cofactor();

    transpose_cofactor_matrix = cofactor_matrix->transpose();

    result = MatrixClass::multiply_matrix_float(transpose_cofactor_matrix, (1 / det));

    delete cofactor_matrix;
    delete transpose_cofactor_matrix;

    after_each(this, "inverse");

    return result;
}

MatrixClass *MatrixClass::transpose(bool destructive)
{
    before_each(this, "transpose");

    if (destructive)
    {
        MatrixClass *save = this->copy();

        MatrixClass::desallocate(this);

        this->n = save->m;
        this->m = save->n;

        this->content = allocate_matrix(this->n, this->m);

        for (size_t i = 0; i < this->m; i++)
            for (size_t j = 0; j < this->n; j++)
                (*this)[j][i] = save->get(i, j);

        delete save;

        after_each(this, "transpose");

        return this;
    }
    else
    {
        MatrixClass *result = new MatrixClass(this->m, this->n);

        for (size_t i = 0; i < this->n; i++)
            for (size_t j = 0; j < this->m; j++)
                (*result)[j][i] = (*this)[i][j];

        after_each(this, "transpose");

        return result;
    }
}

MatrixClass *MatrixClass::cofactor()
{
    // TODO: check that matrix is a square matrix

    before_each(this, "cofactor");

    MatrixClass *result = new MatrixClass(this->n, this->m);

    for (size_t i = 0; i < this->n; i++)
    {
        for (size_t j = 0; j < this->n; j++)
        {
            MatrixClass *cofact = this->cofactor_of(i, j);
            float det_cofact = cofact->determinent();
            /*(*result)[i][j] = power(-1, i + 1 + j + 1) * det_cofact;*/
            (*result)[i][j] = std::pow(-1, i + 1 + j + 1) * det_cofact;
            delete cofact;
        }
    }

    after_each(this, "cofactor");

    return result;
}

MatrixClass *MatrixClass::cofactor_of(size_t line, size_t column)
{
    before_each(this, "cofactor_of");

    int k = 0;
    float *array;
    MatrixClass *result;

    // TODO: check that these are valid lines and columns

    array = (float *)calloc(this->n * this->n, sizeof(float));

    for (size_t i = 0; i < this->n; i++)
    {
        for (size_t j = 0; j < this->m; j++)
        {
            if (i != line && j != column)
            {
                array[k] = (*this)[i][j];
                k++;
            }
        }
    }

    result = MatrixClass::matrix_from_array(NORMAL, array, this->n - 1, this->m - 1);

    free(array);

    after_each(this, "cofactor_of");

    return result;
}

MatrixClass::MatrixClass()
{
    before_each(this, "MatrixClass");

    this->m = 0;
    this->n = 0;
    this->content = NULL;

    if (MatrixClass::is_debug_option_set(MatrixDebug::MATRIX_CREATION))
        printf(COLOR_GREEN "[MatrixClass]:" COLOR_RESET "empty matrix got created\n");

    MatrixClass::created_matrices++;

    after_each(this, "MatrixClass");
}

MatrixClass *MatrixClass::permute_lines(size_t L1, size_t L2, bool destructive)
{
    before_each(this, "permute_lines");

    float temp;
    MatrixClass *result = destructive ? this : this->copy();

    for (size_t j = 0; j < result->m; j++)
    {
        temp = (*result)[L1][j];
        (*result)[L1][j] = (*result)[L2][j];
        (*result)[L2][j] = temp;
    }

    after_each(this, "permute_lines");

    return result;
}

MatrixClass *MatrixClass::permute_columns(size_t C1, size_t C2, bool destructive)
{
    before_each(this, "permute_columns");

    float temp;
    MatrixClass *result = destructive ? this : this->copy();

    for (size_t i = 0; i < result->n; i++)
    {
        temp = (*result)[i][C1];
        (*result)[i][C1] = (*result)[i][C2];
        (*result)[i][C2] = temp;
    }

    after_each(this, "permute_columns");

    return result;
}

/*not set*/
MatrixClass *MatrixClass::create_matrix(size_t n, size_t m)
{
    return new MatrixClass(n, m);
}

/*not set*/
MatrixClass *MatrixClass::matrix_from_array(MatrixType type, float *array, size_t n, size_t m)
{
    int counter = 0;
    MatrixClass *matrix = new MatrixClass(n, m);

    // check that the sizes are compatible

    matrix->map(
        type, [&array, &counter](size_t i, size_t j, float element) -> float
        { return array[counter++]; },
        true);

    return matrix;
}

/*not set*/
MatrixClass *MatrixClass::create_matrix_with(MatrixType type, size_t n, size_t m, float number)
{

    MatrixClass *matrix = new MatrixClass(n, m);

    matrix->map(
        type, [number](size_t i, size_t j, float element) -> float
        { return number; },
        true);

    return matrix;
}

/*not set*/
MatrixClass *MatrixClass::create_matrix_random_int(MatrixType type, size_t n, size_t m, int min, int max)
{
    MatrixClass *matrix = new MatrixClass(n, m);

    matrix->map(
        type, [min, max](size_t i, size_t j, float number) -> float
        { return (float)random_integer(min, max); },
        true);

    return matrix;
}

/*not set*/
MatrixClass *MatrixClass::create_matrix_random_float(MatrixType type, size_t n, size_t m, float min, float max)
{
    MatrixClass *matrix = new MatrixClass(n, m);

    matrix->map(
        type, [min, max](size_t i, size_t j, float number) -> float
        { return random_float(min, max); },
        true);

    return matrix;
}

MatrixClass::MatrixClass(size_t n, size_t m)
{
    before_each(this, "MatrixClass(size_t, size_t)");

    MatrixClass::created_matrices++;

    this->n = n;
    this->m = m;
    this->content = this->allocate_matrix(n, m);

    if (MatrixClass::is_debug_option_set(MatrixDebug::MATRIX_CREATION))
        printf(COLOR_GREEN "[MatrixClass]:" COLOR_RESET "matrix got created\n");

    this->matrices.push_front(this);

    after_each(this, "MatrixClass(size_t, size_t)");
}

float **MatrixClass::allocate_matrix(size_t n, size_t m)
{
    float **result = NULL;

    result = (float **)calloc(n, sizeof(float *));

    for (size_t i = 0; i < n; i++)
    {
        result[i] = (float *)calloc(m, sizeof(float));
        if (result[i] == NULL)
        {
            printf("[allocate_matrix]: NULL\n");
            // free all the others + free(matrix)
            return NULL;
        }
    }

    if (MatrixClass::is_debug_option_set(MatrixDebug::MATRIX_ALLOCATION))
        printf(COLOR_GREEN "[MatrixClass-allocate_matrix]:" COLOR_RESET "matrix-content allocation has been made\n");

    MatrixClass::allocated_matrices++;

    return result;
}

MatrixClass *MatrixClass::copy()
{
    before_each(this, "copy");

    auto result = MatrixClass::copy(this);

    after_each(this, "copy");

    return result;
}

/*not set*/
MatrixClass *MatrixClass::copy(MatrixClass *matrix)
{
    MatrixClass *result = new MatrixClass(matrix->n, matrix->m);

    result->map(
        NORMAL, [&matrix](size_t i, size_t j, float element) -> float
        { return matrix->get(i, j); },
        true);

    return result;
}

/*not set*/
void MatrixClass::srand(unsigned int seed)
{
    MatrixClass::seed = seed;
}

/*not set*/
MatrixClass *MatrixClass::add_matrix_matrix(MatrixClass *matrix_A, MatrixClass *matrix_B)
{
    MatrixClass *result = new MatrixClass(matrix_A->n, matrix_A->m);

    // TODO: check that matriceA and matriceB are of the same dimensions

    result->map(
        NORMAL, [&matrix_A, &matrix_B](size_t i, size_t j, float element) -> float
        { return matrix_A->get(i, j) + matrix_B->get(i, j); },
        true);

    return result;
}

/*not set*/
MatrixClass *MatrixClass::subtract_matrix_matrix(MatrixClass *matrix_A, MatrixClass *matrix_B)
{
    MatrixClass *result = new MatrixClass(matrix_A->n, matrix_A->m);

    // TODO: check that matriceA and matriceB are of the same dimensions

    result->map(
        NORMAL, [&matrix_A, &matrix_B](size_t i, size_t j, float element) -> float
        { return matrix_A->get(i, j) - matrix_B->get(i, j); },
        true);

    return result;
}

/*not set*/
MatrixClass *MatrixClass::add_matrix_float(MatrixClass *matrix, float a)
{
    MatrixClass *result = new MatrixClass(matrix->n, matrix->m);

    result->map(
        NORMAL, [&matrix, &a](size_t i, size_t j, float element) -> float
        { return matrix->get(i, j) + a; },
        true);

    return result;
}

/*not set*/
MatrixClass *MatrixClass::substract_matrix_float(MatrixClass *matrix, float a)
{
    return add_matrix_float(matrix, -a);
}

/*not set*/
MatrixClass *MatrixClass::multiply_matrix_float(MatrixClass *matrix, float a)
{
    MatrixClass *result = new MatrixClass(matrix->n, matrix->m);

    result->map(
        NORMAL, [&matrix, &a](size_t i, size_t j, float element) -> float
        { return matrix->get(i, j) * a; },
        true);

    return result;
}

/*not set*/
MatrixClass *MatrixClass::divide_matrix_float(MatrixClass *matrix, float a)
{
    MatrixClass *result = new MatrixClass(matrix->n, matrix->m);

    result->map(
        NORMAL, [&matrix, &a](size_t i, size_t j, float element) -> float
        { return matrix->get(i, j) / a; },
        true);

    return result;
}

void MatrixClass::for_each(MatrixType type, Consumer consumer)
{
    before_each(this, "for_each");

    if ((type == DIAGONAL || type == UPPER_TRIANGLE || type == LOWER_TRIANGLE) && !this->is_square())
    {
        printf("[for_each]: matrix must be a square matrix for this type\n");
        return;
    }

    switch (type)
    {
    case NORMAL:
        for (size_t i = 0; i < this->n; i++)
            for (size_t j = 0; j < this->m; j++)
                consumer(i, j, (*this)[i][j]);
        break;
    case DIAGONAL:
        for (size_t i = 0; i < this->n; i++)
            consumer(i, i, (*this)[i][i]);

        break;
    case UPPER_TRIANGLE:
        for (size_t i = 0; i < this->n; i++)

            for (size_t j = i; j < this->m; j++)
                consumer(i, j, (*this)[i][j]);

        break;
    case LOWER_TRIANGLE:
        for (size_t i = 0; i < this->n; i++)
            for (size_t j = 0; j <= i; j++)
                consumer(i, j, (*this)[i][j]);
        break;
    default:
        printf("[for_each]: invalid matrix type\n");
        return;
        break;
    }

    after_each(this, "fpr_each");
}
void MatrixClass::for_each_line(size_t line, Consumer consumer)
{
    before_each(this, "for_each_line");

    for (size_t j = 0; j < this->m; j++)
        consumer(line, j, (*this)[line][j]);

    after_each(this, "for_each_line");
}

void MatrixClass::for_each_column(size_t column, Consumer consumer)
{
    before_each(this, "for_each_column");

    for (size_t i = 0; i < this->n; i++)
        consumer(i, column, (*this)[i][column]);

    after_each(this, "for_each_column");
}

void MatrixClass::destroy()
{
    before_each(this, "destroy");

    delete this;

    after_each(this, "destroy");
}

MatrixClass *MatrixClass::map(MatrixType type, Producer producer, bool inplace)
{
    before_each(this, "map");

    MatrixClass *result;

    if ((type == DIAGONAL || type == UPPER_TRIANGLE || type == LOWER_TRIANGLE) && !this->is_square())
    {
        printf("[matrix_better_map]: matrix must be a square matrix for this type\n");
        return NULL;
    }

    result = inplace ? this : new MatrixClass(this->n, this->m);

    switch (type)
    {
    default:
    case NORMAL:
        for (size_t i = 0; i < this->n; i++)
            for (size_t j = 0; j < this->m; j++)
                (*result)[i][j] = producer(i, j, (*this)[i][j]);
        break;
    case DIAGONAL:
        for (size_t i = 0; i < this->n; i++)
            (*result)[i][i] = producer(i, i, (*this)[i][i]);

        break;
    case UPPER_TRIANGLE:
        for (size_t i = 0; i < this->n; i++)

            for (size_t j = i; j < this->m; j++)
                (*result)[i][j] = producer(i, j, (*this)[i][j]);

        break;
    case LOWER_TRIANGLE:
        for (size_t i = 0; i < this->n; i++)
            for (size_t j = 0; j <= i; j++)
                (*result)[i][j] = producer(i, j, (*this)[i][j]);
        break;
    }

    after_each(this, "map");

    return result;
}
MatrixClass *MatrixClass::map_line(size_t line, Producer producer, bool inplace)
{
    before_each(this, "map_line");

    MatrixClass *result = inplace ? this : new MatrixClass(this->n, this->m);

    for (size_t j = 0; j < this->m; j++)
        (*result)[line][j] = producer(line, j, (*this)[line][j]);

    after_each(this, "map_line");

    return result;
}

MatrixClass *MatrixClass::map_column(size_t column, Producer producer, bool inplace)
{
    before_each(this, "map_column");

    MatrixClass *result = inplace ? this : new MatrixClass(this->n, this->m);

    for (size_t i = 0; i < this->n; i++)
        (*result)[i][column] = producer(i, column, (*this)[i][column]);

    after_each(this, "map_column");

    return result;
}
