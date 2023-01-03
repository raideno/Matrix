#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "lib/sle/sle.hpp"
#include "lib/util/util.hpp"
#include "lib/math/math.hpp"
#include "lib/image/image.hpp"
#include "lib/matrix/matrix.hpp"

int main()
{
    printf("[start]\n");

    BitMapFile *file = read_bit_map_file("image-2.bmp");

    for (size_t i = 0; i <= file->width; i++)
    {
        file->content[file->width * file->height - i].alpha = 255;
        file->content[file->width * file->height - i].red = 255;
        file->content[file->width * file->height - i].green = 0;
        file->content[file->width * file->height - i].blue = 0;
    }

    persist_changes(file);

    printf("size_pixel: %ld\n", sizeof(Pixel));
    printf("size: %d\n", file->size);
    printf("bits_per_pixel: %d\n", file->bits_per_pixel);

    printf("[end]\n");

    return 0;
}