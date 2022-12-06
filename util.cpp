#include <math.h>
#include <stdlib.h>

#include "util.hpp"

float random_float(float min, float max)
{
    float scale = rand() / (float)RAND_MAX; /* [0, 1.0] */
    return min + scale * (max - min);
}

int random_integer(int min, int max)
{
    return (rand() % (max - min)) + min;
}

int prompt(const char *string)
{
    int n;

    printf("%s", string);
    scanf("%d", &n);

    return n;
}

float power(float n, size_t p)
{
    float result = 1;

    for (size_t i = 0; i < p; i++)
        result *= n;

    return result;
}

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

    fraction = fraction * power(10, precision);

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