#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char* argv[]){
	
	if(argc < 2) {
		printf("Podaj liczbe.\n");
		exit(1);
	}
	
	long msg = strtol(argv[1], NULL, 10);
	
	struct sockaddr_in myaddr;
	myaddr.sin_family = AF_INET;
	myaddr.sin_port = htons(5000);
	inet_aton("153.19.1.202", &myaddr.sin_addr.s_addr);

	int length = sizeof(myaddr);

	int s = socket(AF_INET, SOCK_DGRAM, 0);

	int buff = htonl(msg);
	sendto(s, &buff, sizeof(buff), 0, &myaddr, length);

	recvfrom(s, &buff, sizeof(buff), 0, &myaddr, &length);
	buff = ntohl(buff);
	
	printf("(%ld^2 + 1) = %d\n", msg, buff);
}           
