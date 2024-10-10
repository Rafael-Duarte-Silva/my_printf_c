#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>

void my_printfFloat(float variable)
{
    float temp = variable;
    int indexFloat = 0;
    while ((temp - (int)temp) != 0)
    {
        temp *= 10;
        indexFloat++;
    }

    unsigned int score = temp;

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
    free(string);
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

void my_printfHexdecimal(long long int variable, bool isUppercase, bool isHasPrefix)
{
    const short int BITS_SIZE = 48;
    const short int HEXDECIMAL_BASE = 4;
    const short int HEXDECIMAL_LENGTH = 16;
    const short int BIT_INDEX_INIT = BITS_SIZE / HEXDECIMAL_BASE - 1;
    bool isFirstHexadecimal = true;
    char *bits = (char *)calloc(BITS_SIZE, sizeof(char));
    /*
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

    if (isUppercase)
    {
        char hexdecimalUppercase[16] = {'F', '7', 'B', '3', 'D', '5', '9', '1', 'E', '6', 'A', '2', 'C', '4', '8', '0'};
        for (int i = 0; i <= HEXDECIMAL_LENGTH; i++)
        {
            hexdecimal[i] = hexdecimalUppercase[i];
        }
    }

    if (isHasPrefix)
    {
        char prefix[3] = "0x";
        write(1, prefix, strlen(prefix));
    }

    for (int i = 0; i < BITS_SIZE; i++)
    {
        bits[i] = variable % 2 + '0';
        variable /= 2;
    }

    for (int bitsIndex = BIT_INDEX_INIT; bitsIndex >= 0; bitsIndex--)
    {
        short int low = 0;
        short int high = HEXDECIMAL_LENGTH - 1;
        short int bitCount = 0;
        short int mid;
        while (bitCount <= HEXDECIMAL_BASE)
        {
            const short int BITS_HEXDECIMAL_BASE = bitsIndex * HEXDECIMAL_BASE + bitCount;

            mid = low + (high - low) / 2;

            if (bits[BITS_HEXDECIMAL_BASE] == '0')
            {
                low = mid + 1;
            }
            else
            {
                high = mid - 1;
            }

            bitCount++;
        }

        if (isFirstHexadecimal && mid == 15)
        {
            continue;
        }

        write(1, hexdecimal + mid, 1);
        isFirstHexadecimal = false;
    }
    free(bits);
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
    const short int bitsSize = sizeof(variable) * 8;
    const short int octalBase = 3;
    char *bits = (char *)calloc(bitsSize, sizeof(char));
    bool isFirstOctal = true;
    char octalTable[8][4] = {
        {"000"}, // 0
        {"100"}, // 1
        {"010"}, // 2
        {"110"}, // 3
        {"001"}, // 4
        {"101"}, // 5
        {"011"}, // 6
        {"111"}  // 7
    };
    char octal[8] = {'0', '1', '2', '3', '4', '5', '6', '7'};

    for (int i = 0; i < bitsSize; i++)
    {
        bits[i] = variable % 2 + '0';
        variable /= 2;
    }

    for (int bitsIndex = bitsSize / octalBase - 1; bitsIndex >= 0; bitsIndex--)
    {
        for (int tableIndexLevelOne = 0; tableIndexLevelOne < 8; tableIndexLevelOne++)
        {
            if (octalTable[tableIndexLevelOne][0] == bits[bitsIndex * octalBase])
            {
                for (int tableIndexLevelTwo = 1; tableIndexLevelTwo < octalBase; tableIndexLevelTwo++)
                {
                    if (octalTable[tableIndexLevelOne][tableIndexLevelTwo] != bits[bitsIndex * octalBase + tableIndexLevelTwo])
                    {
                        break;
                    }
                    else if (tableIndexLevelTwo == octalBase - 1)
                    {
                        if (isFirstOctal && tableIndexLevelOne == 0)
                        {
                            break;
                        }

                        write(1, octal + tableIndexLevelOne, 1);
                        isFirstOctal = false;
                    }
                }
            }
        }
    }
    free(bits);
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
    int x = 13;
    printf("pointer: %p\n", &x);
    my_printf("pointer: %p\n", &x);

    return 0;
}