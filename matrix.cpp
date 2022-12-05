#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include <iostream>

#include "util.hpp"
#include "matrix.hpp"

bool allocations_debug = false;

MatrixType MatrixClass::type()
{
    // we check if it's a diagonal matrix
    // we check if it's a lower triangular / upper triangular matrix
    // otherwise it's a normal matrix

    // check if it's a all zero matrix and return NORMAL if it's the case
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

    return NORMAL;
}

void MatrixClass::set_allocations_debug(bool value)
{
    allocations_debug = value;
}

MatrixClass &select_lines_array(size_t *array, size_t size);
MatrixClass &select_columns_array(size_t *array, size_t size);
MatrixClass &select_array(size_t *lines, size_t n_lines, size_t *columns, size_t n_columns);

MatrixClass &MatrixClass::replace_lines(MatrixClass &matrixA, size_t Afrom, size_t Ato, MatrixClass &matrixB, size_t Bfrom, size_t Bto)
{
    MatrixClass *result;
    size_t size = Ato - Afrom + 1;

    if (matrixA.m != matrixB.m)
    {
        printf("[replace_lines]: the two matrices aren't compatible\n");
        return *(new MatrixClass());
    }

    if ((Ato - Afrom != Bto - Bfrom) || (Afrom > Ato || Bfrom > Bto) || (Afrom > matrixA.n - 1 || Bfrom > matrixB.n - 1 || Ato > matrixA.n - 1 || Bto > matrixB.n - 1))
    {
        printf("[replace_lines]: invalid params\n");
        return *(new MatrixClass());
    }

    result = &matrixA.copy();

    for (size_t i = Bfrom; i < size; i++)
        for (size_t j = 0; j < matrixA.m; j++)
            (*result)[i - Bfrom + Afrom][j] = matrixB[i][j];

    return *result;
}

MatrixClass &MatrixClass::replace_columns(MatrixClass &matrixA, size_t Afrom, size_t Ato, MatrixClass &matrixB, size_t Bfrom, size_t Bto)
{
    MatrixClass *result;
    size_t size = Ato - Afrom + 1;

    if (matrixA.n != matrixB.n)
    {
        printf("[replace_columns]: the two matrices aren't compatible\n");
        return *(new MatrixClass());
    }

    if ((Ato - Afrom != Bto - Bfrom) || (Afrom > Ato || Bfrom > Bto) || (Afrom > matrixA.m - 1 || Bfrom > matrixB.m - 1 || Ato > matrixA.m - 1 || Bto > matrixB.m - 1))
    {
        printf("[replace_columns]: invalid params\n");
        return *(new MatrixClass());
    }

    result = &matrixA.copy();

    for (size_t i = 0; i < matrixA.n; i++)
        for (size_t j = Bfrom; j < size; j++)
            (*result)[i][j - Bfrom + Afrom] = matrixB[i][j];

    return *result;
}

MatrixClass &MatrixClass::replace_lines_with(MatrixClass &matrixA, size_t Afrom, size_t Ato, float number)
{
    MatrixClass *result;

    if ((Afrom > Ato) || (Afrom > matrixA.n - 1 || Ato > matrixA.n - 1))
    {
        printf("[replace_lines]: invalid params\n");
        return *(new MatrixClass());
    }

    result = &matrixA.copy();

    for (size_t i = Ato; i <= Afrom; i++)
        for (size_t j = 0; j < matrixA.m; j++)
            (*result)[i][j] = number;

    return *result;
}

MatrixClass &MatrixClass::replace_columns_with(MatrixClass &matrixA, size_t Afrom, size_t Ato, float number)
{
    MatrixClass *result;

    if ((Afrom > Ato) || (Afrom > matrixA.m - 1 || Ato > matrixA.m - 1))
    {
        printf("[replace_columns]: invalid params\n");
        return *(new MatrixClass());
    }

    result = &matrixA.copy();

    for (size_t i = 0; i < matrixA.n; i++)
        for (size_t j = Afrom; j <= Ato; j++)
            (*result)[i][j] = number;

    return *result;
}

MatrixClass &MatrixClass::resize(size_t n, size_t m, bool inline_)
{
    // check that it's not negatif values
    if (inline_)
    {
        MatrixClass save = this->copy().set_name("resize-save");

        this->desallocate();

        this->allocate_matrix(n, m);

        for (size_t i = 0; i < std::min(this->n, n); i++)
            for (size_t j = 0; j < std::min(this->m, m); j++)
                (*this)[i][j] = save[i][j];

        this->n = n;
        this->m = m;

        // Todo: destroy save

        return *this;
    }
    else
    {
        MatrixClass *result = new MatrixClass(n, m);

        for (size_t i = 0; i < std::min(this->n, n); i++)
            for (size_t j = 0; j < std::min(this->m, m); j++)
                (*result)[i][j] = (*this)[i][j];

        return *result;
    }
}

void MatrixClass::desallocate_matrices(size_t n_args, ...)
{
    va_list ap;
    MatrixClass *matrix;

    va_start(ap, n_args);

    for (size_t i = 0; i < n_args; i++)
    {
        matrix = va_arg(ap, MatrixClass *);
        matrix->desallocate();
    }

    va_end(ap);
}

void MatrixClass::desallocate()
{

    if (this->content == NULL)
        return;

    for (size_t i = 0; i < this->n; i++)
        if (this->content[i] != NULL)
            free(this->content[i]);

    free(this->content);

    if (allocations_debug)
        printf(COLOR_RED "[MatrixClass-desallocate]:" COLOR_RESET "a matrix (%s) got desallocated\n", strlen(this->name) == 0 ? "/" : this->name);

    // Todo: recheck this one
    // Not sure about this one
    // free(this);
}

MatrixClass &MatrixClass::set_name(const char *name)
{
    for (size_t i = 0; i < 25; i++)
        this->name[i] = '\0';

    for (size_t i = 0; i < strlen(name); i++)
        this->name[i] = name[i];

    return *this;
}

MatrixClass::~MatrixClass()
{
    // clear allocation/content
    this->desallocate();

    this->n = 0;
    this->m = 0;
    this->content = NULL;
    if (allocations_debug)
        printf(COLOR_RED "[~MatrixClass]:" COLOR_RESET "a matrix (%s) got destroyed\n", strlen(this->name) == 0 ? "/" : this->name);
}

// make the print check the max in the column and not in the entire matrix
void MatrixClass::print(size_t precision, bool sign)
{
    size_t size;
    std::pair<size_t, size_t> max;

    if (strlen(this->name) > 0)
        printf("%s:\n", this->name);

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
    printf("]");
}

void MatrixClass::print_line(size_t line, size_t precision, bool sign)
{
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
}

void MatrixClass::print_col(size_t column, size_t precision, bool sign)
{
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
}

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

std::ostream &operator<<(std::ostream &os, MatrixClass &matrix)
{

    matrix.print();

    return os;
}

float MatrixClass::get(size_t i, size_t j)
{
    if (i > this->n - 1)
    {
        printf("[get]: invalid i provided\n");
        return 0;
    }

    if (j > this->m - 1)
    {
        printf("[get]: invalid j provided\n");
        return 0;
    }

    return this->content[i][j];
}

MatrixClass &MatrixClass::operator+(MatrixClass &matrix)
{
    return MatrixClass::add_matrix_matrix(*this, matrix);
}

MatrixClass &MatrixClass::operator-(MatrixClass &matrix)
{
    return MatrixClass::subtract_matrix_matrix(*this, matrix);
}

MatrixClass &MatrixClass::operator*(MatrixClass &matrix)
{
    return MatrixClass::multiply_matrix_with_matrix(*this, matrix);
}

MatrixClass &MatrixClass::operator/(MatrixClass &matrix)
{
}

MatrixClass &MatrixClass::operator+(float number)
{
    return MatrixClass::add_matrix_with_float(*this, number);
}

MatrixClass &MatrixClass::operator-(float number)
{
    return MatrixClass::substract_matrix_with_float(*this, number);
}

MatrixClass &MatrixClass::operator*(float number)
{
    return MatrixClass::multiply_matrix_with_float(*this, number);
}

MatrixClass &MatrixClass::operator/(float number)
{
    return MatrixClass::divide_matrix_with_float(*this, number);
}

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

MatrixClass &MatrixClass::operator++()
{
}

MatrixClass &MatrixClass::operator--()
{
}

float *MatrixClass::operator[](size_t i)
{
    return this->content[i];
}

std::pair<size_t, size_t> MatrixClass::max(MatrixType type, bool absolute)
{
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
    return max;
}

size_t MatrixClass::max_line(size_t line, bool abs)
{
    size_t mJ = 0;

    this->for_each_line(line, [&matrix = *this, &line, &mJ](size_t i, size_t j, float element) -> void
                        {
        if (element > matrix[line][mJ])
            mJ = j; });

    return mJ;
}

size_t MatrixClass::max_column(size_t column, bool absolute)
{
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

    return mI;
}

std::pair<size_t, size_t> MatrixClass::min(MatrixType type, size_t *maxI, size_t *maxJ, bool absolute)
{
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
    return max;
}

size_t MatrixClass::min_line(size_t line, bool absolute)
{
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

    return mJ;
}

size_t MatrixClass::min_column(size_t column, bool absolute)
{
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

    return mI;
}

MatrixClass &MatrixClass::dot(MatrixClass &matrix)
{
    return MatrixClass::multiply_matrix_with_matrix(*this, matrix);
}

MatrixClass &MatrixClass::multiply_matrix_with_matrix(MatrixClass &matrixA, MatrixClass &matrixB)
{
    MatrixClass *result;

    if (matrixA.m != matrixB.n)
    {
        printf("[multiply_matrix_matrix]: number of lines and number of columns must be equal\n");
        return (*new MatrixClass());
    }

    result = new MatrixClass(matrixA.n, matrixB.m);

    for (size_t i = 0; i < matrixA.n; i++)
        for (size_t j = 0; j < matrixB.m; j++)
            (*result)[i][j] = multiply_matrix_line_matrix_column(matrixA, matrixB, i, j);

    return *result;
}

float MatrixClass::multiply_matrix_line_matrix_column(MatrixClass &matrixA, MatrixClass &matrixB, size_t LINE, size_t COLUMN)
{
    float result = 0;

    if (LINE > matrixA.n - 1 || COLUMN > matrixB.m - 1)
    {
        printf("[multiply_matrix_line_matrix_column]: given an invalid LINE or COLUMN\n");
        return 0;
    }

    for (size_t i = 0; i < matrixA.m; i++)
        result += (matrixA[LINE][i] * matrixB[i][COLUMN]);

    return result;
}

/*optimiser la fonction pour selectionner la ligne avec le plus de zero !*/
/*faire une fonction qui trouve la ligne ou il y ale plus de zero*/
/*faire une fonction qui trouve la cologne ou il y a le plus de zero*/
float MatrixClass::determinent()
{
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
        MatrixClass cofact = this->cofactor_of(0, i).set_name("cofactor-matrix");
        /*0 in 0 + i in power fonction is for the j (i) but since we selected the first line*/
        /*should we let the "+1" or no ? or replace it with a "+1" ? and why exactly "+2" ? why it wouldn't work normaly ?*/
        result += power(-1, 0 + i + 2) * (*this)[0][i] * cofact.determinent();
        // TODO: fix the matrix destruction
        // delete &cofact;
        // destroy_matrix(cofact);
    }

    return result;
}

float MatrixClass::trace()
{
    float result = 0;

    if (this->is_square())
    {
        printf("[trace]: matrix is not square\n");
        return 0;
    }

    this->for_each(DIAGONAL, [&result](size_t i, size_t j, float element) -> void
                   { result += element; });

    return result;
}

MatrixClass &MatrixClass::select_lines(size_t start, size_t end)
{
    if (start > end)
    {
        printf("[select_lines]: start is bigger than end\n");
        return (*new MatrixClass());
    }

    if (start > this->n - 1 || end > this->n - 1)
    {
        printf("[select_lines]: received invalid startLine or endLine\n");
        return (*new MatrixClass());
    }

    size_t lines = end - start + 1;
    MatrixClass *result = new MatrixClass(lines, this->m);

    for (size_t i = start; i <= end; i++)
        for (size_t j = 0; j < this->m; j++)
            (*result)[i - start][j] = (*this)[i][j];

    return *result;
}

MatrixClass &MatrixClass::select_columns(size_t start, size_t end)
{
    // check that start < end

    if (start > end)
    {
        printf("[select_lines]: start is bigger than end\n");
        return (*new MatrixClass());
    }

    if (start > this->m - 1 || end > this->m - 1)
    {
        printf("[select_columns]: received invalid startColumn or endColumn\n");
        return (*new MatrixClass());
    }

    size_t columns = end - start + 1;
    MatrixClass *result = new MatrixClass(this->n, columns);

    for (size_t i = 0; i < this->n; i++)
        for (size_t j = start; j <= end; j++)
            (*result)[i][j - start] = (*this)[i][j];

    return *result;
}

MatrixClass &MatrixClass::select_(size_t startLine, size_t endLine, size_t startColumn, size_t endColumn)
{
    MatrixClass temp;
    MatrixClass result;

    temp = this->select_lines(startLine, endLine);

    result = temp.select_columns(startColumn, endColumn);

    // TODO:

    delete &temp;
    // destroy_matrix(temp);

    return result;
}

MatrixClass &MatrixClass::delete_lines(size_t start, size_t end)
{

    if (start > this->n - 1 || end > this->n - 1)
    {
        printf("[delete_lines]: received invalid startLine or endLine\n");
        return (*new MatrixClass());
    }

    size_t lines = this->n - (end - start + 1);
    MatrixClass *result = new MatrixClass(lines, this->m);

    for (size_t i = 0; i < start; i++)
        for (size_t j = 0; j < this->m; j++)
            (*result)[i][j] = (*this)[i][j];

    for (size_t i = end + 1; i < this->n; i++)
        for (size_t j = 0; j < this->m; j++)
            (*result)[i - end][j] = (*this)[i][j];

    return *result;
}

MatrixClass &MatrixClass::delete_columns(size_t start, size_t end)
{

    if (start > this->m - 1 || end > this->m - 1)
    {
        printf("[delete_columns]: received invalid startColumn or endColumn\n");
        return (*new MatrixClass());
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

    return *result;
}

MatrixClass &MatrixClass::crop(size_t startLine, size_t endLine, size_t startColumn, size_t endColumn)
{
    MatrixClass temp;
    MatrixClass result;

    temp = this->delete_lines(startLine, endLine);

    // printf("intermediare:\n");
    // print_matrix(temp);

    result = temp.delete_columns(startColumn, endColumn);

    // TODO: fix the delete process
    delete &temp;

    // destroy_matrix(temp);

    return result;
}

bool MatrixClass::is_diagonal()
{
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

    return true;
}

bool MatrixClass::is_triangular()
{
    if (!this->is_square())
        return false;

    return this->is_lower_triangular() || this->is_upper_triangular();
}

bool MatrixClass::is_lower_triangular()
{
    if (!this->is_square())
        return false;

    for (size_t i = 0; i < this->n; i++)
        for (size_t j = i + 1; j < this->m; j++)
            if ((*this)[i][j] != 0)
                return false;

    return true;
}

bool MatrixClass::is_upper_triangular()
{
    if (!this->is_square())
        return false;

    for (size_t i = 0; i < this->n; i++)
        for (size_t j = 0; j < i; j++)
            if ((*this)[i][j] != 0)
                return false;

    return true;
}

bool MatrixClass::is_null()
{
    return this->content == NULL;
}

bool MatrixClass::is_square()
{
    return this->n == this->m;
}

void MatrixClass::read(MatrixType type)
{
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
}

// static
MatrixClass &MatrixClass::concatenate_vertical(size_t n_args, ...)
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

    return *result;
}

/*static*/
MatrixClass &MatrixClass::concatenate_horizontal(size_t n_args, ...)
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

    return *result;
}

bool MatrixClass::one(MatrixType type, BooleanProducer boolean_producer)
{
    bool result = false;

    this->for_each(type, [&boolean_producer, &result](size_t i, size_t j, float element) -> void
                   {
        if (boolean_producer(i, j, element))
            result = true; });

    return result;
}

bool MatrixClass::line_one(size_t line, BooleanProducer boolean_producer)
{
    for (size_t j = 0; j < this->m; j++)
        if (boolean_producer(line, j, (*this)[line][j]))
            return true;

    return false;
}

bool MatrixClass::column_one(size_t column, BooleanProducer boolean_producer)
{
    for (size_t i = 0; i < this->n; i++)
        if (boolean_producer(i, column, (*this)[i][column]))
            return true;

    return false;
}

bool MatrixClass::all(MatrixType type, BooleanProducer boolean_producer)
{
    bool result = true;

    this->for_each(type, [&boolean_producer, &result](size_t i, size_t j, float element) -> void
                   {
        if (!boolean_producer(i, j, element))
            result = false; });

    return result;
}

bool MatrixClass::line_all(size_t line, BooleanProducer boolean_producer)
{
    for (size_t j = 0; j < this->m; j++)
        if (!boolean_producer(line, j, (*this)[line][j]))
            return false;

    return true;
}

bool MatrixClass::column_all(size_t column, BooleanProducer boolean_producer)
{
    for (size_t i = 0; i < this->n; i++)
        if (!boolean_producer(i, column, (*this)[i][column]))
            return false;

    return true;
}

MatrixClass &MatrixClass::inverse(bool destructive)
{
    float det;
    MatrixClass result;
    MatrixClass cofactor_matrix;
    MatrixClass transpose_cofactor_matrix;

    det = this->determinent();

    if (det == 0)
    {
        printf("[inverse_matrix]: determinant is equal to 0\n");
        return (*new MatrixClass());
    }

    cofactor_matrix = this->cofactor();

    transpose_cofactor_matrix = cofactor_matrix.transpose();

    result = transpose_cofactor_matrix * (1 / det);

    // TODO: clean the remaining and unused matrices

    /*
    destroy_matrix(cofactor_matrix);
    destroy_matrix(transpose_cofactor_matrix);
    */

    return result;
}

MatrixClass &MatrixClass::transpose(bool destructive)
{
    if (destructive)
    {
        MatrixClass save = this->copy().set_name("save-for-transpose");

        this->n = save.m;
        this->m = save.n;
        // TODO: destroy the old matrice

        this->desallocate();

        this->content = allocate_matrix(this->n, this->m);

        for (size_t i = 0; i < this->n; i++)
            for (size_t j = 0; j < this->m; j++)
                (*this)[j][i] = save[i][j];

        // TODO: destroy the save matrice

        return *this;
    }
    else
    {
        MatrixClass *result = new MatrixClass(this->m, this->n);

        result->set_name("transpose");

        for (size_t i = 0; i < this->n; i++)
            for (size_t j = 0; j < this->m; j++)
                (*result)[j][i] = (*this)[i][j];

        return *result;
    }
}

MatrixClass &MatrixClass::cofactor()
{
    // TODO: check that matrix is a square matrix

    MatrixClass result = *(new MatrixClass(this->n, this->m));

    for (size_t i = 0; i < this->n; i++)
    {
        for (size_t j = 0; j < this->n; j++)
        {
            MatrixClass cofact = this->cofactor_of(i, j);
            float det_cofact = cofact.determinent();
            result[i][j] = power(-1, i + 1 + j + 1) * det_cofact;
            delete &cofact;
        }
    }

    return result;
}

MatrixClass &MatrixClass::cofactor_of(size_t line, size_t column)
{
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

    result = &MatrixClass::matrix_from_array(NORMAL, array, this->n - 1, this->m - 1);

    free(array);

    return *result;
}

MatrixClass::MatrixClass()
{
    this->m = 0;
    this->n = 0;
    this->content = NULL;

    if (allocations_debug)
        printf(COLOR_GREEN "[MatrixClass]:" COLOR_RESET "empty matrix got created\n");
}

MatrixClass &MatrixClass::permute_lines(size_t L1, size_t L2, bool destructive)
{
    float temp;
    MatrixClass result = destructive ? *this : this->copy();

    for (size_t j = 0; j < result.m; j++)
    {
        temp = result[L1][j];
        result[L1][j] = result[L2][j];
        result[L2][j] = temp;
    }

    return result;
}

MatrixClass &MatrixClass::permute_columns(size_t C1, size_t C2, bool destructive)
{
    float temp;
    MatrixClass result = destructive ? *this : this->copy();

    for (size_t i = 0; i < result.n; i++)
    {
        temp = result[i][C1];
        result[i][C1] = result[i][C2];
        result[i][C2] = temp;
    }

    return result;
}

MatrixClass &MatrixClass::create_matrix(size_t n, size_t m)
{
    MatrixClass *matrix = new MatrixClass(n, m);
    return *matrix;
}

MatrixClass &MatrixClass::matrix_from_array(MatrixType type, float *array, size_t n, size_t m)
{
    int counter = 0;
    MatrixClass *matrix = new MatrixClass(n, m);

    // check that the sizes are compatible

    matrix->map(
        type, [&array, &counter](size_t i, size_t j, float element) -> float
        { return array[counter++]; },
        true);

    return *matrix;
}

MatrixClass &create_matrix_with(MatrixType type, size_t n, size_t m, float number)
{
    MatrixClass *matrix = new MatrixClass(n, m);

    matrix->map(
        type, [number](size_t i, size_t j, float element) -> float
        { return number; },
        true);

    return *matrix;
}

MatrixClass &MatrixClass::create_matrix_random_int(MatrixType type, size_t n, size_t m, int min, int max)
{
    MatrixClass *matrix = new MatrixClass(n, m);

    matrix->map(
        type, [min, max](size_t i, size_t j, float number) -> float
        { return (float)random_integer(min, max); },
        true);

    return *matrix;
}

MatrixClass &create_matrix_random_float(MatrixType type, size_t n, size_t m, float min, float max)
{
    MatrixClass *matrix = new MatrixClass(n, m);

    matrix->map(
        type, [min, max](size_t i, size_t j, float number) -> float
        { return random_float(min, max); },
        true);

    return *matrix;
}

MatrixClass::MatrixClass(size_t n, size_t m)
{
    this->n = n;
    this->m = m;
    this->content = this->allocate_matrix(n, m);

    if (allocations_debug)
        printf(COLOR_GREEN "[MatrixClass]:" COLOR_RESET "matrix got created\n");
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

    if (allocations_debug)
        printf(COLOR_GREEN "[MatrixClass-allocate_matrix]:" COLOR_RESET "matrix-content allocation has been made\n");

    return result;
}

MatrixClass &MatrixClass::copy()
{
    return MatrixClass::copy(*this);
}

MatrixClass &MatrixClass::copy(MatrixClass &matrix)
{
    MatrixClass *result = new MatrixClass(matrix.n, matrix.m);

    result->map(
        NORMAL, [&matrix](size_t i, size_t j, float element) -> float
        { return matrix[i][j]; },
        true);

    return *result;
}

/*static*/ void MatrixClass::srand(unsigned int seed)
{
    std::srand(seed);
}

/*static*/ MatrixClass &MatrixClass::add_matrix_matrix(MatrixClass &matrixA, MatrixClass &matrixB)
{
    MatrixClass *result = new MatrixClass(matrixA.n, matrixA.m);

    // TODO: check that matriceA and matriceB are of the same dimensions

    result->map(
        NORMAL, [&matrixA, &matrixB](size_t i, size_t j, float element) -> float
        { return matrixA[i][j] + matrixB[i][j]; },
        true);

    return *result;
}

/*static*/ MatrixClass &MatrixClass::subtract_matrix_matrix(MatrixClass &matrixA, MatrixClass &matrixB)
{
    MatrixClass *result = new MatrixClass(matrixA.n, matrixA.m);

    // TODO: check that matriceA and matriceB are of the same dimensions

    result->map(
        NORMAL, [&matrixA, &matrixB](size_t i, size_t j, float element) -> float
        { return matrixA[i][j] - matrixB[i][j]; },
        true);

    return *result;
}

/*static*/ MatrixClass &MatrixClass::add_matrix_with_float(MatrixClass &matrix, float a)
{
    MatrixClass *result = new MatrixClass(matrix.n, matrix.m);

    result->map(
        NORMAL, [&matrix, &a](size_t i, size_t j, float element) -> float
        { return matrix[i][j] - a; },
        true);

    return *result;
}

/*static*/ MatrixClass &MatrixClass::substract_matrix_with_float(MatrixClass &matrix, float a)
{
    return add_matrix_with_float(matrix, -a);
}

/*static*/ MatrixClass &MatrixClass::multiply_matrix_with_float(MatrixClass &matrix, float a)
{
    MatrixClass *result = new MatrixClass(matrix.n, matrix.m);

    result->map(
        NORMAL, [&matrix, &a](size_t i, size_t j, float element) -> float
        { return matrix[i][j] * a; },
        true);

    return *result;
}

/*static*/ MatrixClass &MatrixClass::divide_matrix_with_float(MatrixClass &matrix, float a)
{
    MatrixClass *result = new MatrixClass(matrix.n, matrix.m);

    result->map(
        NORMAL, [&matrix, &a](size_t i, size_t j, float element) -> float
        { return matrix[i][j] / a; },
        true);

    return *result;
}

void MatrixClass::for_each(MatrixType type, Consumer consumer)
{
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
}
void MatrixClass::for_each_line(size_t line, Consumer consumer)
{
    for (size_t j = 0; j < this->m; j++)
        consumer(line, j, (*this)[line][j]);
}

void MatrixClass::for_each_column(size_t column, Consumer consumer)
{
    for (size_t i = 0; i < this->n; i++)
        consumer(i, column, (*this)[i][column]);
}

MatrixClass &MatrixClass::map(MatrixType type, Producer producer, bool inplace)
{
    MatrixClass *result;

    if ((type == DIAGONAL || type == UPPER_TRIANGLE || type == LOWER_TRIANGLE) && !this->is_square())
    {
        printf("[matrix_better_map]: matrix must be a square matrix for this type\n");
        return *(new MatrixClass());
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

    return *result;
}
MatrixClass &MatrixClass::map_line(size_t line, Producer producer, bool inplace)
{
    MatrixClass *result = inplace ? this : new MatrixClass(this->n, this->m);

    for (size_t j = 0; j < this->m; j++)
        (*result)[line][j] = producer(line, j, (*this)[line][j]);

    return *result;
}

MatrixClass &MatrixClass::map_column(size_t column, Producer producer, bool inplace)
{
    MatrixClass *result = inplace ? this : new MatrixClass(this->n, this->m);

    for (size_t i = 0; i < this->n; i++)
        (*result)[i][column] = producer(i, column, (*this)[i][column]);

    return *result;
}