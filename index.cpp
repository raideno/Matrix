#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "lib/sle/sle.hpp"
#include "lib/util/util.hpp"
#include "lib/math/math.hpp"
#include "lib/image/image.hpp"
#include "lib/matrix/matrix.cpp"

class Float : public MatrixValue
{

public:
    float data;
    Float()
    {
        // printf("[Float()]: have been called\n");
    }

    Float(float data)
    {
        // printf("[Float(float)]: have been called\n");
        this->data = data;
    }

    ~Float()
    {
        // printf("[~Float()]: have been called\n");
    }

    Float add(Float value)
    {
        Float result(this->data + value.data);
        return result;
    }

    Float sub(Float value)
    {
        Float result(this->data - value.data);
        return result;
    }
    Float mul(Float value)
    {
        Float result(this->data * value.data);
        return result;
    }
    Float div(Float value)
    {
        Float result(this->data / value.data);
        return result;
    }
    static Float rand()
    {
        Float result(56);
        return result;
    }
    void print()
    {
        printf("%f", data);
    }
};
int main()
{
    printf("[start]\n");

    MatrixClass<Float> *matrix_1 = MatrixClass<Float>::create_matrix_with(NORMAL, 3, 3, Float(5));
    MatrixClass<Float> *matrix_2 = MatrixClass<Float>::create_matrix_with(NORMAL, 3, 3, Float(10));
    MatrixClass<Float> *result = MatrixClass<Float>::add_matrix_matrix(matrix_1, matrix_2);

    matrix_1->print();
    matrix_2->print();

    result->print();

    /*
    BitMapFile *file = read_bit_map_file("image-2.bmp");

    for (size_t i = 0; i <= file->width; i++)
    {
        file->content[file->width * file->height - i].alpha = 255;
        file->content[file->width * file->height - i].red = 255;
        file->content[file->width * file->height - i].green = 99;
        file->content[file->width * file->height - i].blue = 71;
    }

    persist_changes(file);

    printf("size: %d\n", file->size);
    */
    printf("[end]\n");

    return 0;
}