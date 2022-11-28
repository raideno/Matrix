#ifndef __UTIL_H__
#define __UTIL_H__

#include <stdio.h>

int random_integer(int min = 0, int max = 10);
float random_float(float min = 0, float max = 1);

int prompt(const char *string);

size_t count_digits(int number);

float min(float a, float b);
float max(float a, float b);
// make it support the negatif power / exponent
float power(float n, size_t p);

bool print_float(float number, size_t size, size_t precision, char padding, bool sign);

#endif