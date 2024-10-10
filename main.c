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
    const short int bitsSize = 48;
    const short int hexadecimalBase = 4;
    char *bits = (char *)calloc(bitsSize, sizeof(char));
    bool isFirstHexadecimal = true;
    char hexdecimalTable[16][5] = {
        {"0000"}, // 0
        {"1000"}, // 1
        {"0100"}, // 2
        {"1100"}, // 3
        {"0010"}, // 4
        {"1010"}, // 5
        {"0110"}, // 6
        {"1110"}, // 7
        {"0001"}, // 8
        {"1001"}, // 9
        {"0101"}, // A
        {"1101"}, // B
        {"0011"}, // C
        {"1011"}, // D
        {"0111"}, // E
        {"1111"}  // F
    };
    char hexdecimal[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

    if (isUppercase)
    {
        for (int i = 10; i <= 16; i++)
        {
            hexdecimal[i] -= 32;
        }
    }

    if (isHasPrefix)
    {
        char prefix[3] = "0x";
        write(1, prefix, strlen(prefix));
    }

    for (int i = 0; i < bitsSize; i++)
    {
        bits[i] = variable % 2 + '0';
        variable /= 2;
    }

    for (int i = bitsSize / hexadecimalBase - 1; i >= 0; i--)
    {
        for (int j = 0; j < 16; j++)
        {
            if (hexdecimalTable[j][0] == bits[i * hexadecimalBase])
            {
                for (int m = 1; m < hexadecimalBase; m++)
                {
                    if (hexdecimalTable[j][m] != bits[i * hexadecimalBase + m])
                    {
                        break;
                    }
                    else if (m == hexadecimalBase - 1)
                    {
                        if (isFirstHexadecimal && j == 0)
                        {
                            break;
                        }

                        write(1, hexdecimal + j, 1);
                        isFirstHexadecimal = false;
                    }
                }
            }
        }
    }
    free(bits);
}

void my_printfHexdecimalUpperCase(int variable)
{
    my_printfHexdecimal(variable, true, false);
}

void my_printfHexdecimalLowerCase(int variable)
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

    for (int i = bitsSize / octalBase - 1; i >= 0; i--)
    {
        for (int j = 0; j < 8; j++)
        {
            if (octalTable[j][0] == bits[i * octalBase])
            {
                for (int m = 1; m < octalBase; m++)
                {
                    if (octalTable[j][m] != bits[i * octalBase + m])
                    {
                        break;
                    }
                    else if (m == octalBase - 1)
                    {
                        if (isFirstOctal && j == 0)
                        {
                            break;
                        }

                        write(1, octal + j, 1);
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
                my_printfHexdecimalLowerCase(va_arg(valist, int));
                break;
            case 'X':
                str++;
                my_printfHexdecimalUpperCase(va_arg(valist, int));
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
    float x = 10.4;
    printf("float hexdecimal: %f\n", x);
    my_printf("float hexdecimal: %f\n", x);

    return 0;
}