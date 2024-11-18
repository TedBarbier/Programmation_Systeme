

#include <fcntl.h>
#include <sys/errno.h>
#include <stdlib.h>

#include "mini_lib.h"

#define IOBUFFER_SIZE 2048

#define init_myFile X 


MYFILE* mini_fopen(char* file, char mode){
    MYFILE* File = (MYFILE*)mini_calloc(sizeof(MYFILE), 1);
    File->buffer_read = NULL;
    File->buffer_write = NULL;
    File->ind_read = -1;
    File->ind_write = -1;
    int flags;
    switch (mode){
        case 'r':
            flags = O_RDONLY;
            break;
        case 'w':
            flags = O_WRONLY | O_CREAT | O_TRUNC;
            break;
        case 'b':
            flags = O_RDWR | O_CREAT;
            break;
        case 'a':
            flags = O_WRONLY | O_CREAT | O_APPEND;
            break;
        default:
            mini_free(File);
            errno = EINVAL;
            return NULL;
    }
    File->fd = open(file, flags, 0664);
    // 0664 is the permission descriptor in case of a newly created
    // 6 for owner (read + write)
    // 6 for groupe (read + write)
    // 4 for others (read)
    if (File->fd == -1) {
        mini_free(File);
        return NULL;
    }

    return File;
}

int mini_fread(void* buffer, int size_element, int number_element, MYFILE* file){

}