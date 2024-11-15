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
    if (str == NULL)
    {
        return;
    }
    if (ind == -1){
        buffer = (char*)mini_calloc(sizeof(char), BUF_SIZE);
        ind = 0;
    }
    while (*str) {
        buffer[ind] = *str;
        ind++;
        if (ind == BUF_SIZE || *str == '\n'){
            int valid = write(STDOUT_FILENO, buffer, BUF_SIZE);
            if (valid < 0){
                write(STDERR_FILENO, "write", 5);
            }
            ind = 0;
            mini_memset(buffer,0,BUF_SIZE);
        }
        str++;
    }
}

void mini_exit_printf(void){
    if (ind > 0){
        int valid = write(STDOUT_FILENO, buffer, ind);
        if (valid < 0){
            write(STDERR_FILENO, "write", 5);
        }
        ind = -1;
    }
}

int mini_scanf(char* buffer, int size_buffer){
    if (buffer == NULL || size_buffer <=0){
        return -1;
    }
    char c;
    read(STDIN_FILENO,buffer,size_buffer);
    return strlen(buffer);
}