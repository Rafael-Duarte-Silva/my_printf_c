#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>

void my_printfFloat(float variable)
{
    int indexFloat = 0;
    while ((variable - (int)variable) != 0)
    {
        indexFloat++;
        variable *= 10;
        printf("variable number: %d\n", (int)variable);
        printf("variable float: %f\n", variable);
    }

    /*unsigned int score = variable;

    unsigned int div = 1;
    unsigned int digitCount = 1;

    char *string;
    int i = 0;

    while (div <= score / 10)
    {
        digitCount++;
        div *= 10;
    }

    string = (char *)calloc(8, sizeof(char));

    while (i < 8)
    {
        if (i == indexFloat)
        {
            string[i] = '.';
            i++;
            continue;
        }
        else if (digitCount > 0)
        {
            string[i] = score / div + '0';
            score %= div;
            div /= 10;
            digitCount--;
            i++;
            continue;
        }

        string[i] = '0';
        i++;
    }
    write(1, string, strlen(string));
    free(string);*/
}

void my_printfNumber(int variable)
{
    unsigned int score = variable < 0 ? variable * -1 : variable;

    unsigned int div = 1;
    unsigned int digitCount = 1;

    char *string;
    int i = 0;

    while (div <= score / 10)
    {
        digitCount++;
        div *= 10;
    }

    if (variable < 0)
    {
        string = (char *)calloc(digitCount + 1, sizeof(char));
        string[0] = '-';
        i++;
    }
    else
    {
        string = (char *)calloc(digitCount, sizeof(char));
    }

    while (digitCount > 0)
    {
        string[i] = score / div + '0';
        score %= div;
        div /= 10;
        digitCount--;
        i++;
    }

    write(1, string, strlen(string));
    free(string);
}

void my_printfUnsignedNumber(unsigned int variable)
{
    unsigned int div = 1;
    unsigned int digitCount = 1;

    char *string;
    int i = 0;

    while (div <= variable / 10)
    {
        digitCount++;
        div *= 10;
    }

    string = (char *)calloc(digitCount, sizeof(char));

    while (digitCount > 0)
    {
        string[i] = variable / div + '0';
        variable %= div;
        div /= 10;
        digitCount--;
        i++;
    }

    write(1, string, strlen(string));
    free(string);
}

void my_printfChar(char variable)
{
    write(1, &variable, strlen(&variable));
}

void my_printfString(char *variable)
{
    write(1, variable, strlen(variable));
}

void my_printfBaseConvert(
    long long int variable,
    bool isUppercase,
    bool isHasPrefix,
    const short int BASE_LENGTH,
    char *prefix,
    char *baseChar)
{
    char *string = (char *)calloc(21, sizeof(char));
    short int stringCount = 0;
    short int prefixCount = 0;

    if (isUppercase)
    {
        for (int i = 0; i < BASE_LENGTH; i++)
        {
            if (baseChar[i] >= 97 && baseChar[i] <= 122)
            {
                baseChar[i] -= 32;
            }
        }
    }

    if (isHasPrefix)
    {
        while (*prefix)
        {
            *string = *prefix;

            string++;
            prefixCount++;
            prefix++;
        }
    }

    while (variable)
    {
        *string = baseChar[variable % BASE_LENGTH];

        variable /= BASE_LENGTH;

        string++;
        stringCount++;
    };

    string -= stringCount + prefixCount;

    for (int i = prefixCount; i < stringCount / 2 + prefixCount; i++)
    {
        char swap = string[i];
        string[i] = string[(stringCount + prefixCount - 1) - i + prefixCount];
        string[(stringCount + prefixCount - 1) - i + prefixCount] = swap;
    }

    write(1, string, strlen(string));
    free(string);
}

void my_printfHexdecimal(long long int variable, bool isUppercase, bool isHasPrefix)
{
    char hexdecimal[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
    char prefix[3] = "0x";

    my_printfBaseConvert(variable, isUppercase, isHasPrefix, 16, prefix, hexdecimal);
}

void my_printfHexdecimalUppercase(int variable)
{
    my_printfHexdecimal(variable, true, false);
}

void my_printfHexdecimalLowercase(int variable)
{
    my_printfHexdecimal(variable, false, false);
}

void my_printfPointer(long long int variable)
{
    my_printfHexdecimal(variable, false, true);
}

void my_printfOctal(unsigned int variable)
{
    char octal[8] = {'0', '1', '2', '3', '4', '5', '6', '7'};
    char prefix[2] = "0";

    my_printfBaseConvert(variable, false, false, 8, prefix, octal);
}

void my_printf(char *str, ...)
{
    va_list valist;
    va_start(valist, str);

    char *character;

    while (*str)
    {
        if (*str == '%')
        {
            switch (*(str + 1))
            {
            case 'd':
                str++;
                my_printfNumber(va_arg(valist, int));
                break;
            case 'i':
                str++;
                my_printfNumber(va_arg(valist, int));
                break;
            case 'u':
                str++;
                my_printfUnsignedNumber(va_arg(valist, unsigned int));
                break;
            case 'c':
                str++;
                my_printfChar(va_arg(valist, int));
                break;
            case 's':
                str++;
                my_printfString(va_arg(valist, char *));
                break;
            case 'f':
                str++;
                my_printfFloat(va_arg(valist, double));
                break;
            case 'p':
                str++;
                my_printfPointer(va_arg(valist, long long int));
                break;
            case 'x':
                str++;
                my_printfHexdecimalLowercase(va_arg(valist, int));
                break;
            case 'X':
                str++;
                my_printfHexdecimalUppercase(va_arg(valist, int));
                break;
            case 'o':
                str++;
                my_printfOctal(va_arg(valist, unsigned int));
                break;

            default:
                str--;
                break;
            }
        }
        else
        {
            character = str;
            write(1, character, 1);
        }

        str++;
    }

    va_end(valist);
}

int main()
{
    int x = 12718712;
    printf("convert: %o\n", x);
    my_printf("convert: %o\n", x);

    return 0;
}