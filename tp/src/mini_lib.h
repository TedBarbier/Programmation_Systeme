#ifndef MINI_LIB_H
#define MINI_LIB_H

typedef struct {
    int fd;
    void * buffer_read;
    void * buffer_write;
    int ind_read;
    int ind_write;
} MYFILE;
//mini_memory.c
extern void* mini_memset(void *ptr, int value, int num);
extern void* mini_calloc(int size_element, int number_element);
extern void mini_free(void *ptr);
extern void mini_exit();
//mini_string.c
extern void mini_printf(char *str);
extern void mini_exit_printf();
extern int mini_scanf(char* buffer, int size_buffer);
extern int mini_strlen(char* s);
extern int mini_strcopy(char* s, char *d);
extern int mini_strcmp(char* s1, char* s2);
extern void mini_perror(char * message);
//mini_io.c
extern void add_open_file(MYFILE* file);
extern void remove_open_file(MYFILE* file);
extern MYFILE* mini_fopen(char* file, char mode);
extern void* mini_memcpy(void* dest, const void* src, int n);
extern void* mini_memmove(void* dest, const void* src, int n);
extern int mini_fread(void* buffer, int size_element, int number_element, MYFILE* file);
extern int mini_fwrite(void* buffer, int size_element, int number_element, MYFILE* file);
extern int mini_fflush(MYFILE* file);
extern int mini_fclose(MYFILE* file);
extern void mini_exit_flush();


#endif // MINI_LIB_H