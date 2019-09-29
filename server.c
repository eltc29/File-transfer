#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	if (argc < 2){
		fprintf(stderr, "server.c: Need port number\n");
		return 1;
	}
	
	int sfd;
	struct sockaddr_in s_addr;
	
	//creating a server socket
	sfd = socket(AF_INET, SOCK_STREAM, 0);
	
	memset(&s_addr, 0, sizeof(struct sockaddr_in));
	//define the server address
  s_addr.sin_family = AF_INET;
	unsigned short portNum = (unsigned short) strtoul(argv[1], NULL, 0);
  s_addr.sin_port = htons(portNum);
  s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(sfd, (struct sockaddr *)&s_addr, sizeof(struct sockaddr_in)) == -1){
    perror("Bind error");
    return 1;
  }
	
	//waiting for response from client
	if (listen(sfd, 2) == -1) {
    perror("Listen error");
    return 1;
  }
	
	while (1){
		int cfd, n;
		struct sockaddr_in c_addr;
		socklen_t sinlen = sizeof(struct sockaddr_in);
		char fileName[100];
		FILE *f;
		
		cfd = accept(sfd, (struct sockaddr *)&c_addr, &sinlen);
		
		//receiving filename from client
		n = read(cfd, fileName, 100);
		f = fopen(fileName, "r");
		printf("\nFile name received: %s\n", fileName);
		
		if (f == NULL){
			fprintf(stderr, "File not found\n");
			char buf[9] = "ERROR404";
			write(cfd, buf, 9);
		}
		else {
			//sending file back to client
			char c;
			while (fread(&c, 1, 1, f)){
				write(cfd, &c, 1);
			}
			printf("Sending %s back to client\n", fileName);
		}
		close(cfd);
	}
	close(sfd);
	
	return 0;
}