#include "image.hpp"

void read_pixel(uint32_t value, uint8_t *alpha, uint8_t *red, uint8_t *green, uint8_t *blue)
{
    *alpha = (value & 0XFF000000) >> 24;
    *red = (value & 0X00FF0000) >> 16;
    *green = (value & 0X0000FF00) >> 8;
    *blue = (value & 0X000000FF);
}

uint32_t create_pixel(uint8_t alpha, uint8_t red, uint8_t green, uint8_t blue)
{
    return ((alpha & 0xff) << 24) + ((red & 0xff) << 16) + ((green & 0xff) << 8) + ((blue & 0xff));
}

BitMapFile *read_bit_map_file(const std::string &name)
{
    FILE *file = NULL;
    BitMapFile *result = NULL;

    file = fopen(name.c_str(), "rb+");

    if (file == NULL)
        return NULL;

    result = (BitMapFile *)malloc(sizeof(BitMapFile));

    result->file = file;

    /*reading file header*/

    result->header_field[0] = read_byte_at(file, 0, 1);
    result->header_field[1] = read_byte_at(file, 1, 1);

    result->size = read_byte_at(file, 2, 4);

    result->offset = read_byte_at(file, 10, 4);

    result->dib_size = read_byte_at(file, 14, 4);

    result->width = read_byte_at(file, 18, 4);
    result->height = read_byte_at(file, 22, 4);
    result->planes_length = read_byte_at(file, 26, 2);
    result->bits_per_pixel = read_byte_at(file, 28, 2);
    result->compression_type = read_byte_at(file, 30, 4);
    result->image_size = read_byte_at(file, 34, 4);
    result->horizontal_resolution = read_byte_at(file, 38, 4);
    result->vertical_resolution = read_byte_at(file, 42, 4);
    result->palette_color_len = read_byte_at(file, 46, 4);
    result->num_colors_used = read_byte_at(file, 50, 4);

    // reading it's content

    // change the size with width x height ?
    //  compare the size to width x height to see if there is a problem
    result->content = (Pixel *)malloc(((result->size - result->offset) / (result->bits_per_pixel / 8)) * sizeof(Pixel));

    fseek(file, result->offset, SEEK_SET);
    for (size_t i = 0; i < (result->size - result->offset) / (result->bits_per_pixel / 8); i++)
    {
        uint32_t buffer;
        fread(&buffer, result->bits_per_pixel / 8, 1, file);
        read_pixel(buffer, &result->content[i].alpha, &result->content[i].red, &result->content[i].green, &result->content[i].blue);
    }

    return result;
}

int read_byte_at(FILE *file, int offset, int size)
{
    // size can't be more than 4, otherwise we can't return it

    int result;
    int long save = ftell(file);

    fseek(file, offset, SEEK_SET);

    fread(&result, size, 1, file);

    fseek(file, save, SEEK_SET);

    return result;
}

void persist_changes(BitMapFile *file)
{
    int long save = ftell(file->file);

    int size = file->width * file->height * sizeof(Pixel);
    printf("writing size: %d\n", size);

    fseek(file->file, file->offset, SEEK_SET);
    // fwrite(file->content, sizeof(Pixel) * (file->width * file->height), 1, file->file);

    for (size_t i = 0; i < (file->size - file->offset) / (file->bits_per_pixel / 8); i++)
    {
        uint32_t data = create_pixel(file->content[i].alpha, file->content[i].red, file->content[i].green, file->content[i].blue);
        fwrite(&data, sizeof(data), 1, file->file);
    }

    fseek(file->file, save, SEEK_SET);
}