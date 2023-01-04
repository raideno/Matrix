#include "window.hpp"

Window::Window(const std::string &name, std::size_t width, std::size_t height)
{
    this->window = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
    this->renderer = SDL_CreateRenderer(window, -1, 0);
}

Window::~Window()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

Window *Window::create(const std::string &name, std::size_t width, std::size_t height)
{
    return new Window(name, width, height);
}

void Window::set_draw_color(Pixel color)
{
    this->color = color;

    SDL_SetRenderDrawColor(renderer, this->color.red, this->color.green, this->color.blue, this->color.alpha);
}

void Window::set_draw_color(uint8_t red, uint8_t green, uint8_t blue)
{
    this->color.alpha = 255;
    this->color.red = red;
    this->color.green = green;
    this->color.blue = blue;

    SDL_SetRenderDrawColor(renderer, this->color.red, this->color.green, this->color.blue, this->color.alpha);
}

void Window::set_draw_color(uint8_t alpha, uint8_t red, uint8_t green, uint8_t blue)
{
    this->color.alpha = alpha;
    this->color.red = red;
    this->color.green = green;
    this->color.blue = blue;

    SDL_SetRenderDrawColor(renderer, this->color.red, this->color.green, this->color.blue, this->color.alpha);
}

Pixel Window::get_draw_color()
{
    return this->color;
}

void Window::draw_point(size_t i, size_t j)
{
    SDL_RenderDrawPoint(this->renderer, i, j);
}

void Window::draw_point(size_t i, size_t j, Pixel color)
{
    SDL_SetRenderDrawColor(renderer, color.red, color.green, color.blue, color.alpha);
    SDL_RenderDrawPoint(this->renderer, i, j);
    SDL_SetRenderDrawColor(renderer, this->color.red, this->color.green, this->color.blue, this->color.alpha);
}

void Window::refresh()
{
    SDL_RenderPresent(this->renderer);
}

void Window::listen(Callback callback)
{
    this->running = true;

    while (running)
        while (SDL_PollEvent(&this->event) && this->running)
            callback(event);
}
