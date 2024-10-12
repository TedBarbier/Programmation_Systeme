#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define TBUF 1000000

int main(int argc, char ** argv){

    int N = 10, i = 1;
    if (argc < 2){
        fprintf(stderr, "Usage: %s [-n number] file\n", argv[0]);
        return 1;
    }

    // Handle the -n option
    if (argc >= 3 && argv[1][0] == '-' && argv[1][1] == 'n') {
        if (argc < 4) {
            fprintf(stderr, "Usage: %s [-n number] file\n", argv[0]);
            return 1;
        }
        N = atoi(argv[2]);
        i = 3;
    } else if (argc == 2) {
        // Only file argument provided
        i = 1;
    } else {
        fprintf(stderr, "Usage: %s [-n number] file\n", argv[0]);
        return 1;
    }

    int fdsrc = open(argv[i], O_RDONLY);
    if (fdsrc < 0) {
        perror("open");
        return 1;
    }

    // Read the entire file into a buffer
    char BUFFER[TBUF];
    int nbread;
    int totalRead = 0;
    while ((nbread = read(fdsrc, BUFFER + totalRead, sizeof(BUFFER) - totalRead)) > 0) {
        totalRead += nbread;
    }
    if (nbread < 0) {
        perror("read");
        close(fdsrc);
        return 1;
    }

    // Find the start of the last N lines
    int lineCount = 0;
    for (i = totalRead - 1; i >= 0; i--) {
        if (BUFFER[i] == '\n') {
            lineCount++;
            if (lineCount == N + 1) {
                i++;
                break;
            }
        }
    }
    if (i < 0) i = 0; // If there are less than N lines, start from the beginning

    // Write the last N lines to stdout
    write(STDOUT_FILENO, BUFFER + i, totalRead - i);

    close(fdsrc);
    return 0;
}
