#ifndef MINI_LIB_H
#define MINI_LIB_H
extern void* mini_memset(void *ptr, int value, int num);
extern void* mini_calloc(int size_element, int number_element);
extern void mini_free(void *ptr);
extern void mini_exit();
extern void mini_printf(char *str);

#endif // MINI_LIB_H