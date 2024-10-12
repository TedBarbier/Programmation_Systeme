#include <stdio.h>

#define SIZE 1000000

int main(int argc, char ** argv){
    FILE * file1 = fopen(argv[1], "r");
    FILE * file2 = fopen(argv[2], "r");
    if (file1 == NULL || file2 == NULL){
        perror("fopen");
        return 1;
    }
    char buffer[SIZE];
    size_t bytesRead1 = fread(buffer, 1, sizeof(*buffer), file1);
    size_t bytesRead2 = fread(buffer, 1, sizeof(*buffer), file2);

}