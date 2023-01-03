#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// 86,454 bytes
// 200 x 144

#define COLOR_RED 0xFF0000
#define COLOR_GREEN 0x00FF00
#define COLOR_BLUE 0x0000FF

struct Color
{
    int alpha;
    int r;
    int g;
    int b;
};
typedef struct Color Color;

struct BitMapFile
{
    FILE *file;
    int *content;

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

int read_bytes_at(FILE *file, int offset, int number)
{
    int result, cursor_save;

    cursor_save = ftell(file);

    fseek(file, offset, SEEK_SET);

    fread(&result, number, 1, file);

    fseek(file, cursor_save, SEEK_SET);

    return result;
}

void decimal_rgb(int decimal, int *r, int *g, int *b)
{
    *r = (decimal & 0XFF0000) >> 16;
    *g = (decimal & 0X00FF00) >> 8;
    *b = (decimal & 0X0000FF);
}

void test()
{
    Color *pixels;
    FILE *file = fopen("image-2.bmp", "rb+");

    int size;
    fseek(file, 2, SEEK_SET);
    fread(&size, 4, 1, file);
    printf("byte[02-06](size): %d\n", size);

    int beginning;
    fseek(file, 10, SEEK_SET);
    fread(&beginning, 4, 1, file);
    printf("byte[10-14](beginning): %d\n", beginning);

    int bits_per_pixel;
    fseek(file, 28, SEEK_SET);
    fread(&bits_per_pixel, 2, 1, file);
    printf("byte[28-30](bits_per_pixel): %d\n", bits_per_pixel);

    pixels = (Color *)malloc(((size - beginning) / (bits_per_pixel / 8)) * sizeof(Color));

    fseek(file, beginning, SEEK_SET);
    for (size_t i = 0; i < (size - beginning) / (bits_per_pixel / 8); i++)
    {
        int buffer;
        int r, g, b;
        fread(&buffer, bits_per_pixel / 8, 1, file);
        decimal_rgb(buffer, &r, &g, &b);
        pixels[i].r = r;
        pixels[i].g = g;
        pixels[i].b = b;
        printf("pixel[%ld]: (%d %d %d) %d\n", i, r, g, b, buffer);
    }

    uint32_t data[] = {0xFFFF0000, 0xFF00FF00, 0xFF0000FF};

    fseek(file, beginning + 84, SEEK_SET);
    fwrite(data, sizeof(data), 1, file);
    fclose(file);

    free(pixels);
}

int main()
{
    FILE *fileptr;
    // char *buffer;
    long filelen;

    fileptr = fopen("image-2.bmp", "rb");
    fseek(fileptr, 0, SEEK_END);
    filelen = ftell(fileptr);
    rewind(fileptr);
    // buffer = (char *)malloc((filelen + 1) * sizeof(char));

    char header_field[2];
    fseek(fileptr, 0, SEEK_SET);
    fread(&header_field, 2, 1, fileptr);
    printf("byte[00-02](header_field): %s\n", header_field);

    int size;
    fseek(fileptr, 2, SEEK_SET);
    fread(&size, 4, 1, fileptr);
    printf("byte[02-06](size): %d\n", size);

    int beginning;
    fseek(fileptr, 10, SEEK_SET);
    fread(&beginning, 4, 1, fileptr);
    printf("byte[10-14](beginning): %d\n", beginning);

    int dib_size;
    fseek(fileptr, 14, SEEK_SET);
    fread(&dib_size, 4, 1, fileptr);
    printf("byte[14-18](dib_size): %d\n", dib_size);

    int width;
    fseek(fileptr, 18, SEEK_SET);
    fread(&width, 4, 1, fileptr);
    printf("byte[18-22](width): %d\n", width);

    int height;
    fseek(fileptr, 22, SEEK_SET);
    fread(&height, 4, 1, fileptr);
    printf("byte[22-26](height): %d\n", height);

    int planes_length;
    fseek(fileptr, 26, SEEK_SET);
    fread(&planes_length, 2, 1, fileptr);
    printf("byte[26-28](planes_length): %d\n", planes_length);

    int bits_per_pixel;
    fseek(fileptr, 28, SEEK_SET);
    fread(&bits_per_pixel, 2, 1, fileptr);
    printf("byte[28-30](bits_per_pixel): %d\n", bits_per_pixel);

    int compression_type;
    fseek(fileptr, 30, SEEK_SET);
    fread(&compression_type, 4, 1, fileptr);
    printf("byte[30-34](compression_type): %d\n", compression_type);

    int image_size;
    fseek(fileptr, 34, SEEK_SET);
    fread(&image_size, 2, 1, fileptr);
    printf("byte[34-38](image_size): %d\n", image_size);

    int horizontal_resolution;
    fseek(fileptr, 38, SEEK_SET);
    fread(&horizontal_resolution, 4, 1, fileptr);
    printf("byte[38-42](horizontal_resolution): %d\n", horizontal_resolution);

    int vertical_resolution;
    fseek(fileptr, 42, SEEK_SET);
    fread(&vertical_resolution, 4, 1, fileptr);
    printf("byte[42-46](vertical_resolution): %d\n", vertical_resolution);

    int palette_color_len;
    fseek(fileptr, 46, SEEK_SET);
    fread(&palette_color_len, 4, 1, fileptr);
    printf("byte[46-50](palette_color_len): %d\n", palette_color_len);

    int num_colors_used;
    fseek(fileptr, 50, SEEK_SET);
    fread(&num_colors_used, 4, 1, fileptr);
    printf("byte[50-54](num_colors_used): %d\n", num_colors_used);

    /*
    fseek(fileptr, beginning, SEEK_SET);
    for (size_t i = 0; i < (filelen - beginning) / (32 / 8); i++)
    {
        int buffer;
        fread(&buffer, (32 / 8), 1, fileptr);
        printf("pixel[%ld]: %d\n", i, buffer);
    }
    */

    printf("File len: %ld\n", filelen);
    printf("Pixels len: %d\n", width * height);

    fclose(fileptr); // Close the file

    test();

    return 0;
}