#include <stdio.h>
#include <stdlib.h>
#include "gdbout_api.h"
int main(){
	if(clientInit(STDOUT_FILENO, STDERR_FILENO) == -1){
		return 1;
	}
	int fd = open("./test.log", O_WRONLY | O_CREAT | O_TRUNC);
	char buf[BUFSIZ] = {0};
	int n = 0;
	for(int i = 0; i < 3; i++){
		printf("stdout:%d\n", i);
		sleep(1);
		/*
		n = fprintf(stderr, "stderr:%d\n", i);
		sprintf(buf, "stderr:%d:%d\n", n, i);
		write(fd, buf, strlen(buf));
		n = fprintf(stdout, "stdout:%d\n", i);
		fsync(STDOUT_FILENO);
		sprintf(buf, "stdout:%d:%d\n", n, i);
		write(fd, buf, strlen(buf));
		sleep(1);
		*/
	}
	close(fd);
	return 0;
}
