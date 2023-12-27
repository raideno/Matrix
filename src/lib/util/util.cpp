#include <math.h>
#include <stdlib.h>

#include "util.hpp"

float random_float(float min, float max, unsigned int seed)
{
    float scale = rand() / (float)RAND_MAX; /* [-1.0, 1.0] */
    float result = min + scale * (max - min);
    return result;
}

double random_double(double min, double max, unsigned int seed)
{
    double scale = rand() / (double)RAND_MAX; /* [-1.0, 1.0] */
    double result = min + scale * (max - min);
    return result;
}

int random_integer(int min, int max, unsigned int seed)
{
    int result = (rand() % (max - min)) + min;
    return result;
}

bool random_bool()
{
    return random_integer(0, 10) % 2 == 0;
}

/*
float power(float n, size_t p)
{
    float result = 1;

    for (size_t i = 0; i < p; i++)
        result *= n;

    return result;
}
*/

float max(float a, float b)
{
    return a > b ? a : b;
}

float min(float a, float b)
{
    return a < b ? a : b;
}

size_t count_digits(int number)
{
    size_t result = 0;

    if (number == 0)
        return 1;

    number = abs(number);

    while (number != 0)
    {
        number /= 10;
        result++;
    }

    return result;
}

bool print_float(float number, size_t size, size_t precision, char padding, bool sign)
{
    bool isPositive = number > 0;
    size_t iPartDigits, fractionPartDigits;
    int entier, virgule;

    float iPart;
    float fraction = modff(number, &iPart);

    iPartDigits = count_digits((int)iPart);

    // TODO: fix it

    /*
    if (iPartDigits > size)
    {
        printf("\n[print_float]: ERROR\n");
        return false;
    }
    */

    if (sign)
        printf("%c", number > 0 ? '+' : '-');

    for (size_t i = iPartDigits; i < size; i++)
        printf("%c", padding);

    fraction = fraction * std::pow(10, precision);

    fractionPartDigits = count_digits((int)fraction);

    printf("%d", abs((int)iPart));

    if (precision == 0)
        return true;

    // remove fraction sign
    fraction = isPositive ? fraction : fraction * -1;

    printf(".");
    printf("%d", (int)fraction);

    for (size_t i = fractionPartDigits; i < precision; i++)
        printf("0");

    return true;
}