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

    uint8_t colors[3][4] = {{255, 47, 80, 30},
                            {255, 175, 255, 0},
                            {255, 222, 50, 255}};

    BitMapFile *file = read_bit_map_file("./images/image-2.bmp");

    file->content->for_each(NORMAL, [](size_t i, size_t j, Pixel pixel) -> void
                            { 
        printf("pixel[%ld][%ld]: ", i, j);
        pixel.print();
        printf("\n"); });

    for (size_t i = 0; i < file->height; i++)
    {
        file->content->map_line(
            i, [](size_t i, size_t j, Pixel pixel) -> Pixel
            { return i % 2 == 0 ? Pixel(255, 255, 255, 255) : Pixel(255, 0, 0, 0); },
            true);
    }

    persist_changes(file);

    printf("size: %d\n", file->size);

    printf("[end]\n");

    return 0;
}