#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

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

    uint8_t colors[3][4] = {{255, 255, 0, 0},
                            {255, 0, 255, 0},
                            {255, 0, 0, 255}};

    BitMapFile *file = read_bit_map_file("./images/image-2.bmp");

    for (size_t i = 0; i < file->height; i++)
    {
        for (size_t j = 0; j < file->width; j++)
        {
            size_t index = i + j * file->width;
            file->content[index].alpha = colors[i % 3][0];
            file->content[index].red = colors[i % 3][1];
            file->content[index].green = colors[i % 3][2];
            file->content[index].blue = colors[i % 3][3];
        }
    }

    persist_changes(file);

    printf("size: %d\n", file->size);

    printf("[end]\n");

    return 0;
}