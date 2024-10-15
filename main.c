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
    const short int BITS_LENGTH,
    const short int BASE_LENGTH_BLOCK,
    const short int BASE_LENGTH,
    char *prefix,
    char *baseChar)
{
    const short int BIT_INDEX_INIT = BITS_LENGTH / BASE_LENGTH_BLOCK - 1;
    bool isFirstBaseChar = true;
    char *bits = (char *)calloc(BITS_LENGTH, sizeof(char));

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
        write(1, prefix, strlen(prefix));
    }

    for (int i = 0; i < BITS_LENGTH; i++)
    {
        bits[i] = variable % 2 + '0';
        variable /= 2;
    }

    for (int bitIndexBlock = BIT_INDEX_INIT; bitIndexBlock >= 0; bitIndexBlock--)
    {
        short int low = 0;
        short int high = BASE_LENGTH - 1;
        short int mid;
        short int bitCount = 0;
        while (bitCount <= BASE_LENGTH_BLOCK)
        {
            const short int BIT_INDEX = bitIndexBlock * BASE_LENGTH_BLOCK + bitCount;

            mid = low + (high - low) / 2;

            if (bits[BIT_INDEX] == '0')
            {
                low = mid + 1;
            }
            else
            {
                high = mid - 1;
            }

            bitCount++;
        }

        if (isFirstBaseChar && mid == BASE_LENGTH - 1)
        {
            continue;
        }

        write(1, baseChar + mid, 1);
        isFirstBaseChar = false;
    }
    free(bits);
}

void my_printfHexdecimal(long long int variable, bool isUppercase, bool isHasPrefix)
{
    /* READ IN BIG ENDIAN
        F = 1111
        7 = 1110
        B = 1101
        3 = 1100
        D = 1011
        5 = 1010
        9 = 1001
        1 = 1000
        E = 0111
        6 = 0110
        A = 0101
        2 = 0100
        C = 0011
        4 = 0010
        8 = 0001
        0 = 0000
    */
    char hexdecimal[16] = {'f', '7', 'b', '3', 'd', '5', '9', '1', 'e', '6', 'a', '2', 'c', '4', '8', '0'};
    char prefix[3] = "0x";

    my_printfBaseConvert(variable, isUppercase, isHasPrefix, 48, 4, 16, prefix, hexdecimal);
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
    /* READ IN BIG ENDIAN
        7 = 111
        3 = 110
        5 = 101
        1 = 100
        6 = 011
        2 = 101
        4 = 001
        0 = 000
    */
    char octal[8] = {'7', '3', '5', '1', '6', '2', '4', '0'};
    char prefix[2] = "0";

    my_printfBaseConvert(variable, false, false, sizeof(variable) * 8, 3, 8, prefix, octal);
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
    int x = 82147128;
    printf("hexdecimal: %X\n", x);
    my_printf("hexdecimal: %X\n", x);

    return 0;
}