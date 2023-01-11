#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <inttypes.h>

#include "lib/sle/sle.hpp"
#include "lib/util/util.hpp"
#include "lib/math/math.hpp"
#include "lib/image/image.hpp"

#include "lib/window/window.hpp"

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
        return Float(random_float());
    }
    void print()
    {
        printf("%f", data);
    }
};

#define THICKNESS 1

int main()
{
    printf("[start]\n");

    std::string filename;

    std::cout << "filename:";
    std::getline(std::cin, filename);

    BitMapFile *bitmap_file = read_bit_map_file(filename);

    Window *window = Window::create(filename.c_str(), bitmap_file->width, bitmap_file->height);

    SDL_Event event;

    SDL_Init(SDL_INIT_VIDEO);

    bool running = true;

    auto [n, m] = bitmap_file->content->size();

    size_t i_pixel_position = 0;
    size_t j_pixel_position = n - 1;

    for (size_t i = 0; i < n; i++, i_pixel_position = 0, j_pixel_position--)
        for (size_t j = 0; j < m; j++, i_pixel_position++)
            window->draw_point(i_pixel_position, j_pixel_position, bitmap_file->content->get(i, j));

    printf("[bits_per_pixel]: %d\n", bitmap_file->bits_per_pixel);
    printf("[system]: click on \"D\" to start drawing\n");
    printf("[system]: click on \"C\" to change color\n");
    printf("[system]: click on \"S\" to save image\n");

    window->refresh();

    Pixel color;
    bool is_drawing = false;

    window->listen([window, bitmap_file, &is_drawing, &color](SDL_Event event) -> void
                   {
        switch (event.type)
        {
        case SDL_QUIT:
                window->running = false;
            break;
        case SDL_KEYDOWN:
                if (event.key.keysym.scancode == SDL_SCANCODE_D)
                {
                    is_drawing = !is_drawing;
                    printf("[system]: %s\n", is_drawing ? "Enabled Drawing" : "Disabled Drawing");
                }

                if (event.key.keysym.scancode == SDL_SCANCODE_C)
                {
                    /*
                    printf("[system]: color alpha(0-255):");
                    scanf("%hhd", &color.alpha);
                    printf("[system]: color red(0-255):");
                    scanf("%hhd", &color.red);
                    printf("[system]: color green(0-255):");
                    scanf("%hhd", &color.green);
                    printf("[system]: color blue(0-255):");
                    scanf("%hhd", &color.blue);
                    */
                    window->set_draw_color(color);
                }

                if (event.key.keysym.scancode == SDL_SCANCODE_S)
                {
                    std::string save_filename;
                    printf("[system]: save to:");
                    std::getline(std::cin, save_filename);

                    persist_changes(bitmap_file, save_filename);
                }
                break;

            case SDL_MOUSEMOTION:
            {
                if (!is_drawing)
                    break;

                auto [actual_i, actual_j] = Pixel::get_actual_coordinates(event.motion.x, event.motion.y, bitmap_file->width, bitmap_file->height);

                bitmap_file->content->set(actual_i, actual_j, color);

                bitmap_file->content->set(actual_i + 1, actual_j, color);
                bitmap_file->content->set(actual_i - 1, actual_j, color);
                bitmap_file->content->set(actual_i, actual_j + 1, color);
                bitmap_file->content->set(actual_i, actual_j - 1, color);

                bitmap_file->content->set(actual_i + 1, actual_j + 1, color);
                bitmap_file->content->set(actual_i - 1, actual_j - 1, color);
                bitmap_file->content->set(actual_i - 1, actual_j + 1, color);
                bitmap_file->content->set(actual_i + 1, actual_j - 1, color);

                window->draw_point(event.motion.x, event.motion.y);

                try
                {

                    int r = 5;

                    for(int i = event.motion.x - r; i <= event.motion.x + r; i++)
                    {
                    for(int j = event.motion.y - r; j <= event.motion.y + r; j++)
                    {
                        if((i-r)*(i-r) + (j-r)*(j-r) <= r*r)
                        {
                            printf("drawn\b");
                            window->draw_point(i, j);
                        }
                    }
                    }
                    /*
                    window->draw_point(event.motion.x + 1, event.motion.y);
                    window->draw_point(event.motion.x - 1, event.motion.y);
                    window->draw_point(event.motion.x, event.motion.y + 1);
                    window->draw_point(event.motion.x, event.motion.y - 1);

                    window->draw_point(event.motion.x + 1, event.motion.y + 1);
                    window->draw_point(event.motion.x - 1, event.motion.y - 1);
                    window->draw_point(event.motion.x - 1, event.motion.y + 1);
                    window->draw_point(event.motion.x + 1, event.motion.y - 1);
                    */
                }
                catch (const std::exception &e)
                {
                    std::cerr << "[error]" << '\n';
                }

                window->refresh();

                break;
            }
        } });

    delete window;

    SDL_Quit();

    printf("[end]\n");

    return EXIT_SUCCESS;
}