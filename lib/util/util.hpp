#ifndef __UTIL_HPP__
#define __UTIL_HPP__

#include <stdio.h>

#define COLOR_RESET "\033[0m"
#define COLOR_BLACK "\033[30m"
#define COLOR_RED "\033[31m"
#define COLOR_GREEN "\033[32m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_BLUE "\033[34m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_CYAN "\033[36m"
#define COLOR_WHITE "\033[37m"
#define COLOR_BOLD_BLACK "\033[1m\033[30m"
#define COLOR_BOLD_RED "\033[1m\033[31m"
#define COLOR_BOLD_GREEN "\033[1m\033[32m"
#define COLOR_BOLD_YELLOW "\033[1m\033[33m"
#define COLOR_BOLD_BLUE "\033[1m\033[34m"
#define COLOR_BOLD_MAGENTA "\033[1m\033[35m"
#define COLOR_BOLD_CYAN "\033[1m\033[36m"
#define COLOR_BOLD_WHITE "\033[1m\033[37m"
#define COLOR_UNDERLINE "\033[4m"

int random_integer(int min = 0, int max = 10, unsigned int seed = 45);
float random_float(float min = 0, float max = 1, unsigned int seed = 45);

size_t count_digits(int number);

float min(float a, float b);
float max(float a, float b);

// float power(float n, size_t p);

bool print_float(float number, size_t size, size_t precision, char padding, bool sign);

#endif