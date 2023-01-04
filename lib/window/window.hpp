#ifndef __WINDOW_HPP__
#define __WINDOW_HPP__

#include <string>
#include <iostream>

#include "../image/image.hpp"

#include <SDL2/SDL.h>

#include <functional>

typedef const std::function<void(SDL_Event event)> &Callback;

class Window
{
public:
    Pixel color;
    SDL_Event event;
    SDL_Window *window;
    bool running = false;
    SDL_Renderer *renderer;

    Window(const std::string &name, std::size_t width, std::size_t height);
    ~Window();

    static Window *create(const std::string &name, std::size_t width, std::size_t height);

    void set_draw_color(Pixel color);
    void set_draw_color(uint8_t red, uint8_t green, uint8_t blue);
    void set_draw_color(uint8_t alpha, uint8_t red, uint8_t green, uint8_t blue);

    Pixel get_draw_color();

    void draw_point(size_t i, size_t j);
    void draw_point(size_t i, size_t j, Pixel color);

    void refresh();

    void listen(Callback callback);
};

#endif