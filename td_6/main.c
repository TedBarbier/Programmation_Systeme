// include standart library
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <assert.h>

int exercice1() {
    pid_t pid = fork();
    switch (pid) {
        case -1 :
            perror("Creation processus");
            exit(EXIT_FAILURE);
        case 0 :
            printf("[%d] Je viens de naitre\n", getpid());
            printf("[%d] Mon pere est %d\n", getpid(), getppid());
            break;
        default:
            printf("[%d] J’ai engendre\n", getpid());
            printf("[%d] Mon fils est %d\n", getpid(), pid);
    }
    printf("[%d] Je termine\n", getpid());
    exit(EXIT_SUCCESS);
}

int main(int argc, char * argv[]){
    int fd;
    char buf[10];
    fd = open(argv[1], O_RDWR);
    assert(fd != -1);
    read(fd, buf, 2);
    switch (fork()) {
    case -1 :
        perror("Creation processus");
        exit(EXIT_FAILURE);
    case 0 :
        write(fd, "foo", 3);
        sleep(2);
        read(fd, buf, 3);
        buf[3]='\0';
        printf("[%d] fils a lu ’%s’\n", getpid(), buf); break;
    default:
        write(fd, "bar", 3);
        sleep(1);
        read(fd, buf, 3); buf[3]=’\0’;
        printf("[%d] pere a lu ’%s’\n", getpid(), buf);
    }
    close(fd);
    exit(EXIT_SUCCESS);
}

int main(int argc, int ** argv[]) {
    exercice1();
    exercice2();
    return 0;
}