#include <SDL2/SDL.h>

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
        return Float(random_float());
    }
    void print()
    {
        printf("%f", data);
    }
};

#define WIDTH 720
#define HEIGHT 720

#define THICKNESS 1

int main()
{
    printf("[start]\n");

    uint8_t colors[3][4] = {{255, 47, 80, 30},
                            {255, 175, 255, 0},
                            {255, 222, 50, 255}};

    BitMapFile *bitmap_file = read_bit_map_file("./images/image-1.bmp");

    /*
    bitmap_file->content->for_each(NORMAL, [](size_t i, size_t j, Pixel pixel) -> void
                            {
        printf("pixel[%ld][%2ld]: ", i, j);
        pixel.print();
        printf("\n"); });
    */

    SDL_Event event;
    SDL_Renderer *renderer;
    SDL_Window *window;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(bitmap_file->width, bitmap_file->height, 0, &window, &renderer);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

    SDL_RenderClear(renderer);

    bool running = true;
    bool listening_to_events = true;
    unsigned int t1 = SDL_GetTicks();

    auto [n, m] = bitmap_file->content->size();

    size_t i_pixel_position = 0;
    size_t j_pixel_position = n - 1;

    for (size_t i = 0; i < n; i++, i_pixel_position = 0, j_pixel_position--)
    {
        for (size_t j = 0; j < m; j++, i_pixel_position++)
        {
            SDL_SetRenderDrawColor(renderer, bitmap_file->content->get(i, j).red, bitmap_file->content->get(i, j).green, bitmap_file->content->get(i, j).blue, bitmap_file->content->get(i, j).alpha);
            SDL_RenderDrawPoint(renderer, i_pixel_position, j_pixel_position);
        }
    }

    printf("[bits_per_pixel]: %d\n", bitmap_file->bits_per_pixel);
    printf("[system]: click on \"D\" to start drawing\n");

    SDL_RenderPresent(renderer);

    bool is_drawing = false;

    while (running)
    {

        SDL_Event event;

        while (SDL_PollEvent(&event) && listening_to_events)
        {
            if (event.type == SDL_QUIT)
            {
                listening_to_events = false;
                running = false;
                break;
            }

            switch (event.type)
            {
            case SDL_KEYDOWN:
                if (event.key.keysym.scancode == SDL_SCANCODE_D)
                {
                    is_drawing = !is_drawing;
                    printf("[system]: %s\n", is_drawing ? "Enabled Drawing" : "Disabled Drawing");
                }
                break;

            case SDL_MOUSEMOTION:
            {
                if (!is_drawing)
                    break;

                auto [actual_i, actual_j] = Pixel::get_actual_coordinates(event.motion.x, event.motion.y, bitmap_file->width, bitmap_file->height);

                bitmap_file->content->set(actual_i, actual_j, Pixel(255, 255, 0, 0));

                bitmap_file->content->set(actual_i + 1, actual_j, Pixel(255, 255, 0, 0));
                bitmap_file->content->set(actual_i - 1, actual_j, Pixel(255, 255, 0, 0));
                bitmap_file->content->set(actual_i, actual_j + 1, Pixel(255, 255, 0, 0));
                bitmap_file->content->set(actual_i, actual_j - 1, Pixel(255, 255, 0, 0));

                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

                SDL_RenderDrawPoint(renderer, event.motion.x, event.motion.y);

                try
                {
                    SDL_RenderDrawPoint(renderer, event.motion.x + 1, event.motion.y);
                    SDL_RenderDrawPoint(renderer, event.motion.x - 1, event.motion.y);
                    SDL_RenderDrawPoint(renderer, event.motion.x, event.motion.y + 1);
                    SDL_RenderDrawPoint(renderer, event.motion.x, event.motion.y - 1);
                }
                catch (const std::exception &e)
                {
                    std::cerr << "[error]" << '\n';
                }

                SDL_RenderPresent(renderer);
                break;
            }
            }

            unsigned int t2 = SDL_GetTicks();
            float delta = (t2 - t1) / 1000.0f;

            // printf("(%u, %u, %f)\n", t2, t1, delta);
            t1 = t2;
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    persist_changes(bitmap_file, "new_bitmap_file.bmp");

    printf("[end]\n");

    return EXIT_SUCCESS;

    /*
    for (size_t i = 0; i < bitmap_file->height; i++)
    {
        bitmap_file->content->map_line(
            i, [](size_t i, size_t j, Pixel pixel) -> Pixel
            { return i % 2 == 0 ? Pixel(255, 255, 255, 255) : Pixel(255, 0, 0, 0); },
            true);
    }

    persist_changes(bitmap_file);
    */
}