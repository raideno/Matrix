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

struct Pixel
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t alpha;
};
typedef struct Pixel Pixe;

struct BitMapFile
{
    FILE *file;
    Pixel *content;

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
void read_pixel(uint32_t value, uint8_t *alpha, uint8_t *red, uint8_t *green, uint8_t *blue);
uint32_t create_pixel(uint8_t alpha, uint8_t red, uint8_t green, uint8_t blue);

BitMapFile *read_bit_map_file(const std::string &name);
int read_byte_at(FILE *file, std::size_t offset, std::size_t size);
// if any changes have been made to the content proprety in BitMapFile structure it will persist them from memory to disk
void persist_changes(BitMapFile *file);

//  image structure
//  function to load an image
//  function to print an image

#endif