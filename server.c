//chat server
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define MAXLEN 100
#define QUEUELIMIT 3
#define FAIL -1
#define SUCCESS 0

void usage() {
	printf("syntax : ./server <port number>\n");
	printf("sample : ./server 8080\n");
}

int main(int argc, char* argv[])
{
	int sd, cd, len;
	char buff[MAXLEN];
	int i = 0;
	struct sockaddr_in server_addr, client_addr;
	if (argc != 2) {
		usage();
		return FAIL;
	}
	uint16_t port = atoi(argv[1]);

	//create socket
	if ((sd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		perror("socket creation error"); //socket creation error
		return FAIL;
	}

	//assign IP and PORT
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	//bind socket
	if (bind(sd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
		perror("socket bind error"); //socket bind error
		return FAIL;
	}

	//listen
	if (listen(sd, QUEUELIMIT) < 0) {
		perror("server listen error"); //server listen error
		return FAIL;
	}
	len = sizeof(client_addr);

	//accept
	if ((cd = accept(sd, (struct sockaddr*)&client_addr, &len)) < 0) {
		perror("server accept error");
		return FAIL;
	}

	printf("Connection from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

	//chat
	while (1) {
		//read
		memset(buff, 0, MAXLEN);
		recv(cd, buff, sizeof(buff), 0);
		printf("From client : %s", buff, 0);
		len = strlen(buff) - 1;
		if (strncmp("QUIT", buff, (len > 4? len: 4)) == 0) {
			printf("Disconnected\n");
			break;
		}

		//write
		printf("To client : ");
		memset(buff, 0, MAXLEN);
		i = 0;
		while ((buff[i++] = getchar()) != '\n');
		send(cd, buff, sizeof(buff), 0);
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
