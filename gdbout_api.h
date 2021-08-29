#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <sys/types.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#ifndef GDBOUT_REDIRECT_H
#define GDBOUT_REDIRECT_H
#define SRV_SOCK_STDOUT "/tmp/gdbsrvstdout_Afeather.socket"
#define SRV_SOCK_STDERR "/tmp/gdbsrvstderr_Afeather.socket"
#define CLI_SOCK_STDOUT "/tmp/gdbclistdout_Afeather.socket"
#define CLI_SOCK_STDERR "/tmp/gdbclistderr_Afeather.socket"
#define SRV_LISTEN_MAX 1
#define PLINE() printf("%d\n", __LINE__)
#define RECONNECT_TIME 1
int clientInit(int outfd, int errfd);
#define clientAutoInit(STDOUT_FILENO, STDERR_FILENO);
#define sys_error(str) {fprintf(stderr, "%s:%s:%d:",__FILE__, __func__, __LINE__);perror(str);}





int makeSockaddrUn(struct sockaddr_un *addr, char *path){
	if(sizeof(addr->sun_path) - 1 <= strlen(path)){
		return -1;
	}
	addr->sun_family = AF_LOCAL;
	return strcpy(addr->sun_path, path) == addr->sun_path ? 0 : -1;
}
int openSocketUn(char *path){
	struct sockaddr_un addr;
	if(makeSockaddrUn(&addr, path) == -1){
		return -1;
	}
	int fd = 0;
	for(int i = 0; i < 3; i++){
		fd = socket(AF_LOCAL, SOCK_STREAM, 0);
		if(fd != -1){
			break;
		}
		sleep(RECONNECT_TIME);
	}
	unlink(path);
	if(fd != -1){
		if(bind(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1){
			return -1;
		}
		else
			return fd;
	}
	else
		return -1;
}
int connectSockUn(int fd, char *path){
	/*
	 * require:
	 * 	fd:	Socket which created by socket
	 */
	struct sockaddr_un addr;
	if(makeSockaddrUn(&addr, path) == -1){
		return -1;
	}
	int sfd;
	for(int i = 0; i < 3; i++){
		if((sfd = connect(fd, (struct sockaddr*)&addr,
					sizeof(addr))) != -1)
			return sfd;
		sleep(RECONNECT_TIME);
	}
	return -1;
}
int serverAcceptUn(int fd){
	struct sockaddr_un addr;
	socklen_t len = sizeof(addr);
	int cfd = 0;
	for(int i = 0; i < 3; i++){
		if((cfd = accept(fd, (struct sockaddr*)&addr, &len)) != -1){
			return cfd;
		}
	}
	return -1;
}
void clientClose(int *fds){
	for(int i = 0; i < 2; i++){
		if(fds[i] != -1){
			close(fds[i]);
		}
	}
}
int clientInit(int outfd, int errfd){
	int cfdo = openSocketUn(CLI_SOCK_STDOUT);
	int cfde = openSocketUn(CLI_SOCK_STDERR);
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);
	if(cfdo == -1 || cfde == -1){
		sys_error("");
		clientClose((int[]){cfdo, cfde});
		return -1;
	}
	if(connectSockUn(cfdo, SRV_SOCK_STDOUT) == -1
			|| connectSockUn(cfde, SRV_SOCK_STDERR) == -1){
		sys_error("");
		clientClose((int[]){cfdo, cfde});
		return -1;
	}
	if(dup2(cfdo, outfd) == -1 || dup2(cfde, errfd) == -1){
		sys_error("");
		clientClose((int[]){cfdo, cfde});
		return -1;
	}
	return 0;
}
#endif
