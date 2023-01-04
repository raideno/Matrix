#ifndef __IMAGE_HPP__
#define __IMAGE_HPP__

#include <stdint.h>

#include "../matrix/matrix.hpp"
enum DibHeaderTypes
{
    BITMAPCOREHEADER = 12, // also called OS21XBITMAPHEADER
    OS22XBITMAPHEADER_64 = 64,
    OS22XBITMAPHEADER_16 = 16,
    BITMAPINFOHEADER = 40,
    BITMAPV2INFOHEADER = 52,
    BITMAPV3INFOHEADER = 56,
    BITMAPV4HEADER = 108,
    BITMAPV5HEADER = 124,
};

class Pixel : public MatrixValue
{
public:
    uint8_t alpha, red, green, blue;

    Pixel()
    {
        this->alpha = 0;
        this->red = 0;
        this->green = 0;
        this->blue = 0;
        // printf("[Pixel()]: have been called\n");
    }

    Pixel(uint32_t value)
    {
        Pixel::decompose_pixel(value, &this->alpha, &this->red, &this->green, &this->blue);
    }

    Pixel(uint8_t alpha, uint8_t red, uint8_t green, uint8_t blue)
    {
        // printf("[Pixel(Pixel)]: have been called\n");
        this->alpha = alpha;
        this->red = red;
        this->green = green;
        this->blue = blue;
    }

    ~Pixel()
    {
        // printf("[~Pixel()]: have been called\n");
    }

    Pixel add(Pixel value)
    {
        return Pixel(this->alpha + value.alpha, this->red + value.red, this->green + value.green, this->blue + value.blue);
    }

    Pixel sub(Pixel value)
    {
        return Pixel(this->alpha - value.alpha, this->red - value.red, this->green - value.green, this->blue - value.blue);
    }
    Pixel mul(Pixel value)
    {
        return Pixel(this->alpha * value.alpha, this->red * value.red, this->green * value.green, this->blue * value.blue);
    }
    Pixel div(Pixel value)
    {
        return Pixel(this->alpha / value.alpha, this->red / value.red, this->green / value.green, this->blue / value.blue);
    }
    static Pixel rand()
    {
        return Pixel(255, 255, 255, 255);
    }

    static std::pair<size_t, size_t> get_sdl_coordinates(size_t actual_i, size_t actuaul_j, size_t width, size_t height)
    {
        size_t i_pixel_position = 0;
        size_t j_pixel_position = height - 1;

        for (size_t i = 0; i < height; i++, i_pixel_position = 0, j_pixel_position--)
            for (size_t j = 0; j < width; j++, i_pixel_position++)
                if (i == actual_i && j == actuaul_j)
                    return std::pair<size_t, size_t>(i_pixel_position, j_pixel_position);

        return std::pair<size_t, size_t>(-1, -1);
    }

    static std::pair<size_t, size_t> get_actual_coordinates(size_t pixel_i, size_t pixel_j, size_t width, size_t height)
    {
        size_t i_pixel_position = 0;
        size_t j_pixel_position = height - 1;

        for (size_t i = 0; i < height; i++, i_pixel_position = 0, j_pixel_position--)
            for (size_t j = 0; j < width; j++, i_pixel_position++)
                if (i_pixel_position == pixel_i && j_pixel_position == pixel_j)
                    return std::pair<size_t, size_t>(i, j);

        return std::pair<size_t, size_t>(-1, -1);
    }

    void print()
    {
        printf("(%3u, %3u, %3u, %3u)", this->alpha, this->red, this->green, this->blue);
    }

    static void decompose_pixel(uint32_t value, uint8_t *alpha, uint8_t *red, uint8_t *green, uint8_t *blue)
    {
        *alpha = (value & 0XFF000000) >> 24;
        *red = (value & 0X00FF0000) >> 16;
        *green = (value & 0X0000FF00) >> 8;
        *blue = (value & 0X000000FF);
    }

    static uint32_t compose_32_bits_pixel(Pixel pixel)
    {
        return ((pixel.alpha & 0xff) << 24) + ((pixel.red & 0xff) << 16) + ((pixel.green & 0xff) << 8) + ((pixel.blue & 0xff));
    }

    static uint32_t compose_24_bits_pixel(Pixel pixel)
    {
        return ((pixel.red & 0xff) << 16) + ((pixel.green & 0xff) << 8) + ((pixel.blue & 0xff));
    }
};

struct BitMapFile
{
    FILE *file;
    MatrixClass<Pixel> *content;

    char header_field[2];
    int size;
    int offset;

    int dib_size;
    int width;
    int height;
    int planes_length;
    int bits_per_pixel;
    int compression_type;
    int image_size;
    int horizontal_resolution;
    int vertical_resolution;
    int palette_color_len;
    int num_colors_used;
};
typedef struct BitMapFile BitMapFile;

// BitMapFile *read_header(BitMapFile *file);
// BitMapFile *read_dib_header(BitMapFile *file);
// add option to tell wheter it's a 32bit pixel or 24 or 16 or 8 or ...
BitMapFile *read_bit_map_file(const std::string &name);
int read_byte_at(FILE *file, std::size_t offset, std::size_t size);
// if any changes have been made to the content proprety in BitMapFile structure it will persist them from memory to disk
void persist_changes(BitMapFile *file, const std::string &name);

//  image structure
//  function to load an image
//  function to print an image

#endif