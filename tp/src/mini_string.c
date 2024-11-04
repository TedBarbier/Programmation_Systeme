//include standart library
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//include personal library


//define constant
#define BUF_SIZE 1024

char buffer[BUF_SIZE];
int ind = -1;

void mini_printf(char *str)
{
    if (ind == -1)
    {
        ind = 0;
    }

    while (*str)
    {
        buffer[ind++] = *str;
        if (ind == BUF_SIZE || *str == '\n')
        {
            write(STDOUT_FILENO, buffer, ind);
            ind = 0;
        }
        str++;
    }
}
