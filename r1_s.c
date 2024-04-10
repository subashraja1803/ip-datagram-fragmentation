#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define PORT 8083
#define BL 10
struct frag
	{
		char data[1000];
		int off_set;
		int MF;
		int pktl;
		int seqno;
	}f1[10];
int main() {
	int sockfd, routerfd1, routerfd, len;
	struct sockaddr_in server, client1;
	
    
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd == -1) {
		printf("\nSocket Creation Failed");
		exit(0);
	} else {
		printf("\nSocket Created Successfully");
	}

	bzero(&server, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_port = htons(PORT);

	if((bind(sockfd, (struct sockaddr *) &server, sizeof(server))) != 0) {
		printf("\nError in binding socket");
		exit(0);
	} else {
		printf("\nSocket binded successfully");
	}

	if((listen(sockfd, BL)) != 0) {
		printf("\nError in listening");
		exit(0);
	} else {
		printf("\nServer started listening...");
	}

	len = sizeof(client1);
	routerfd = accept(sockfd, (struct sockaddr *) &client1, &len);
	if(routerfd < 0) {
		printf("\nError in accepting router");
		exit(0);
	} else {
		printf("\nrouter accepted successfully");
	}
	int dnf;
	int f;
	char a[100];
	read(routerfd, &dnf, sizeof(dnf));
	if(dnf==1)
	{
		read(routerfd, &a, sizeof(a));
		printf("\n Data is : %s",a);
	}
	else
	{
		read(routerfd,&f,sizeof(f));
		for(int i=0;i<f;i++)
		{
			read(routerfd, &f1[i].seqno, sizeof(int));
			read(routerfd, &f1[i].data, sizeof(char[1000]));
			read(routerfd, &f1[i].MF, sizeof(int));
			read(routerfd, &f1[i].off_set, sizeof(int));
			read(routerfd, &f1[i].pktl, sizeof(int));
			printf("\n Sequence number: %d",f1[i].seqno);
			printf("\n Fragmented DATA : %s",f1[i].data);
			printf("\n OFFSET : %d",f1[i].off_set);
			printf("\n MF : %d",f1[i].MF);
			printf("\n Packet length is %d bytes\n",f1[i].pktl);
		}
	}
	return 0;
}