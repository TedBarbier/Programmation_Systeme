#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define TBUF 1024

int main(int argc, char ** argv){
	int fddst=open(argv[2], O_WRONLY|O_CREAT|O_TRUNC, 0600);
	int fdsrc=open(argv [1], O_RDONLY);
	
	char * BUFFER[TBUF];
	int nbread;
	while((nbread=read(fdsrc,BUFFER,TBUF))>0){
		write(fddst,BUFFER,nbread);
	}
	close(fddst);
	close(fdsrc);
	return 0;
}
