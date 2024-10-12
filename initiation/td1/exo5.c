#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <errno.h>
#include <string.h>

int main(int argc, char ** argv){
    char * msg;
    printf("SYS_write:%i\n", SYS_write);
    if (syscall(SYS_write, STDOUT_FILENO, "hello\n", 6)){
        printf("Errno: %i\n", errno);
        perror("Erreur");
        msg = strerror(13);
        printf("%s\n",msg);
    }
    return 0;
}