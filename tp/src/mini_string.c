//include standart library
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/errno.h>

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
    int nb_carac = 0;
    char * buffer_tmp=buffer;
    while(nb_carac < size_buffer)
    {
        
        if (read(STDIN_FILENO, &c, 1) <= 0) {
            break;
        }
        if (c == '\n') {
            break;
        }
        *buffer_tmp = c;
        buffer_tmp++;
        nb_carac++;
    }
    *buffer_tmp = '\0';
    return nb_carac;
}

int mini_strlen(char* s){
    if (s == NULL) return -1;
    int count = 0;
    while (*s != '\0'){
        count++;
        s++;
    }
    return count;
}

int mini_strcopy(char* s, char *d){
    if (s == NULL || d == NULL) return -1;
    int count = 0;
    while (*s != '\0'){
        *d = *s;
        s++;
        d++;
        count++;
    }
    *d = '\0'; // Add null terminator
    return count;
}

int mini_strcmp(char* s1, char* s2){
    if (s1 == NULL || s2 == NULL) return -1;
    while (*s1 != '\0' && *s2 != '\0'){
        if (*s1 != *s2){
            return -1;
        }
        s1++;
        s2++;
    }
    return (*s1 == '\0' && *s2 == '\0') ? 0 : -1;
}

void mini_perror(char * message){
    mini_printf(message);
    mini_printf(" : ");
    char err_str[12];
    int err_num = errno;
    int i = 0;
    if (err_num == 0) {
        err_str[i++] = '0';
    } else {
        while (err_num > 0) {
            err_str[i++] = (err_num % 10) + '0';
            err_num /= 10;
        }
    }
    err_str[i] = '\0';

    // Reverse the string
    for (int j = 0; j < i / 2; j++) {
        char temp = err_str[j];
        err_str[j] = err_str[i - j - 1];
        err_str[i - j - 1] = temp;
    }

    mini_printf(err_str);
    mini_printf("\n");
}