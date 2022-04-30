//chat client
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define MAXLEN 100
#define FAIL -1
#define SUCCESS 0

void usage() {
	printf("syntax : ./client <ip address> <port number>\n");
	printf("sample : ./client 127.0.0.1 8080\n");
}

int main(int argc, char* argv[])
{
	int sd, len;
	char buff[MAXLEN];
	int i = 0;
	struct sockaddr_in client_addr;
	if (argc != 3) {
		usage();
		return FAIL;
	}
	uint16_t port = atoi(argv[2]);

	//create socket
	if ((sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		perror("socket creation error"); //socket creation error
		return FAIL;
	}

	//assign IP and PORT
	char* ip = argv[1];
	in_addr_t ip_addr = inet_addr(ip);
	client_addr.sin_family = AF_INET;
	client_addr.sin_port = htons(port);
	client_addr.sin_addr.s_addr = ip_addr;

	//connect socket
	if (connect(sd, (struct sockaddr*)&client_addr, sizeof(client_addr)) < 0) {
		perror("socket connect error"); //socket connect error
		return FAIL;
	}
	printf("Connected\n");

	//chat
	while (1) {
		//write
		printf("To server : ");
		memset(buff, 0, MAXLEN);
		i = 0;
		while ((buff[i++] = getchar()) != '\n');
		write(sd, buff, sizeof(buff));
		len = strlen(buff) - 1;
		if (strncmp("QUIT", buff, (len > 4? len : 4)) == 0) {
			printf("Disconnected\n");
			break;
		}

		//read
		memset(buff, 0, MAXLEN);
		read(sd, buff, sizeof(buff));
		printf("From server : %s", buff);
		len = strlen(buff) - 1;
		if (strncmp("QUIT", buff, (len > 4? len : 4)) == 0) {
			printf("Disconnected\n");
			break;
		}
	}

	//close socket
	close(sd);
	return SUCCESS;
}
