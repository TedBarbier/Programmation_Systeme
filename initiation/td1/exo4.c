#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>

int main(int argc, char ** argv){
    printf("SYS_write:%i\n", SYS_write);
    syscall(SYS_write, STDOUT_FILENO, "hello\n", 6);
    return 0;
}