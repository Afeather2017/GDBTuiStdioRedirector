#include <stdio.h>
#include <stdlib.h>
#include "gdbout_api.h"
int main(){
	int sfd = openSocketUn(SRV_SOCK_STDERR);
	if(sfd == -1){
		return 1;
	}
	if(listen(sfd, SRV_LISTEN_MAX) == -1){
		sys_error("Listen");
		return 2;
	}
	int cfd = serverAcceptUn(sfd);
	if(cfd == -1){
		sys_error("Accept");
	}
	fprintf(stderr, "Socket '%s'Opened\n", SRV_SOCK_STDERR);
	char buf[BUFSIZ] = {0};
	ssize_t size = 0;
	while(true){
		size = read(cfd, buf, sizeof(buf) - 1);
		if(size == -1)
			continue;
		else if(size == 0)
			break;
		printf("%*s", (int)size, buf);
	}
	printf("%d", (int)size);
	close(sfd);
	close(cfd);
	unlink(SRV_SOCK_STDERR);
	return 0;
}


