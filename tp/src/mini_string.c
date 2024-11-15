//include standart library
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//include personal library
#include "mini_lib.h"

//define constant
#define BUF_SIZE 1024

char* buffer = NULL;
int ind = -1;

void mini_printf(char *str)
{
    if (str == NULL) {
        return;
  }
    if (ind == -1) {
        buffer = (char *)mini_calloc(sizeof(char), BUF_SIZE);
    if (buffer == NULL) {
      return;
    }
     ind = 0;
  }

    while (*str)
    {
        buffer[ind++] = *str;
        if (ind == BUF_SIZE || *str == '\n')
        {
            int valid = write(STDOUT_FILENO, buffer, ind);
            if (valid < 0)
            {
                return;
            }
            mini_memset(buffer, 0, ind);
            ind = 0;
        }
    }
    ind = 0;
}


