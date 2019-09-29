#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
  if (argc < 5) {
    fprintf(stderr, "client.c: Incorrect number of arguments\n");
    return 1;
  }

  int cfd;
  struct sockaddr_in c_addr;
	
	//creating a client socket
	cfd = socket(AF_INET, SOCK_STREAM, 0);

  memset(&c_addr, 0, sizeof(struct sockaddr_in));
	//define the client address
  c_addr.sin_family = AF_INET;
  unsigned short portNum = (unsigned short)strtoul(argv[2], NULL, 0);
  c_addr.sin_port = htons(portNum);
  if (inet_pton(AF_INET, argv[1], &c_addr.sin_addr) == 0){
    fprintf(stderr, "client.c: Argument 1 is not an IPv4 address\n");
    return 1;
  }

  if (connect(cfd, (struct sockaddr *)&c_addr, sizeof(struct sockaddr_in)) == -1){
    perror("Connect error");
    return 1;
  }
	
	char fileName[100];
	strcpy(fileName, argv[3]);
	//sending filename to server
	write(cfd, fileName, 100);
	
	//receiving file from server
	FILE *received, *newFile;
	char buf[9];
	received = fdopen(cfd, "r");
	if (fread(buf, 9, 1, received) && strcmp(buf, "ERROR404") == 0){
		fprintf(stderr, "File not received\n");
		return 1;
	}
	if ((newFile = fopen(argv[4], "w")) == NULL){
		fprintf(stderr, "Opening for writing failed\n");
		return 1;
	}
	
	//writing the buf that's already been read
	fwrite(buf, 9, 1, newFile); 
	
	//writing to new file to be saved
	char c;
	while (fread(&c, 1, 1, received)){
		fputc(c, newFile);
	}
	
	fclose(newFile);	
	fclose(received);
	printf("File %s received and saved as %s\n", argv[3], argv[4]);
	close(cfd);
	
	return 0;
}