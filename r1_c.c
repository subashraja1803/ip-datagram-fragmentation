#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h> 
#define PORT 8082

struct frag
    {
		char data[100];
		int off_set;
		int MF;
	}f1[10];
int main() 
{
	int routerfd;
	struct sockaddr_in client;
    char a[1000];
	int MTU;
	int dnf;

	routerfd = socket(AF_INET, SOCK_STREAM, 0);
	if(routerfd == -1) 
	{
		printf("Socket creation failed");
		exit(0);
	} 
	else 
	{
		printf("Socket Created Successfully");
	}

	bzero(&client, sizeof(client));
	client.sin_family = AF_INET;
	client.sin_addr.s_addr = htonl(INADDR_ANY);
	client.sin_port = htons(PORT);

	if(connect(routerfd, (struct sockaddr *) &client, sizeof(client)) != 0) {
		printf("\nFailed to connect with the router");
		exit(0);
	} else {
		printf("\nConnected to router successfully");
	}
    
	printf("\nEnter the message\n");
	scanf("%s",a);
	write(routerfd,&a,sizeof(char[1000]));
	printf("\nEnter the MTU\n");
	scanf("%d",&MTU);
	write(routerfd,&MTU,sizeof(char[1000]));
	return 0;
}