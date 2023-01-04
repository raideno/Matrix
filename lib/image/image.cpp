#include "image.hpp"
#include "../matrix/matrix.hpp"
#include "../matrix/matrix.cpp"

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

BitMapFile *read_bit_map_file(const std::string &name)
{
    FILE *file = NULL;
    BitMapFile *result = NULL;

    file = fopen(name.c_str(), "rb");

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
    //(Pixel *)malloc(((result->size - result->offset) / (result->bits_per_pixel / 8)) * sizeof(Pixel));

    result->content = MatrixClass<Pixel>::create_matrix(result->height, result->width);

    fseek(file, result->offset, SEEK_SET);
    for (size_t i = 0; i < (result->size - result->offset) / (result->bits_per_pixel / 8); i++)
    {
        // here it's the pixel number i
        uint32_t buffer;
        fread(&buffer, result->bits_per_pixel / 8, 1, file);
        std::size_t line = i / result->width;
        std::size_t column = i % result->width;
        result->content->set(line, column, Pixel(buffer));
        // read_pixel(buffer, &result->content[i].alpha, &result->content[i].red, &result->content[i].green, &result->content[i].blue);
    }

    return result;
}

void persist_changes(BitMapFile *bitmap_file, const std::string &name)
{
    // open in again here in read mode and save the changes
    int long save = ftell(bitmap_file->file);

    FILE *file = fopen(name.c_str(), "wb");

    fseek(bitmap_file->file, 0, SEEK_SET);

    for (size_t i = 0; i < bitmap_file->offset; i++)
    {
        char buffer;
        fread(&buffer, 1, 1, bitmap_file->file);
        fwrite(&buffer, 1, 1, file);
    }

    fseek(bitmap_file->file, save, SEEK_SET);
    fseek(bitmap_file->file, bitmap_file->offset, SEEK_SET);

    bitmap_file->content->for_each(NORMAL, [bitmap_file, file](size_t i, size_t j, Pixel pixel) -> void
                                   {
                                       switch (bitmap_file->bits_per_pixel)
                                       {
                                        case 1:
                                            break;
                                        case 2:
                                            break;
                                        case 4:
                                            break;
                                        case 8:
                                            break;
                                        case 16:
                                            break;
                                       case 24:
                                       {

                                           uint32_t pixel_value = Pixel::compose_24_bits_pixel(pixel);
                                           fwrite(&pixel_value, 3, 1, file);
                                           break;
                                       }

                                       case 32:
                                       {

                                           uint32_t pixel_value = Pixel::compose_32_bits_pixel(pixel);
                                           fwrite(&pixel_value, 4, 1, file);
                                           break;
                                       }
                                       } });
}