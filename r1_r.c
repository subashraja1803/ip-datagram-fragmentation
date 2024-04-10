#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define PORT 8082
#define PORT1 8083
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
	int routerfd, clientfd, len,sockfd;
	struct sockaddr_in router, client,client1;
	
	routerfd = socket(AF_INET, SOCK_STREAM, 0);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(routerfd == -1) {
		printf("\nSocket Creation Failed");
		exit(0);
	} else {
		printf("\nSocket Created Successfully");
	}

	bzero(&router, sizeof(router));
	router.sin_family = AF_INET;
	router.sin_addr.s_addr = htonl(INADDR_ANY);
	router.sin_port = htons(PORT);

	if((bind(routerfd, (struct sockaddr *) &router, sizeof(router))) != 0) {
		printf("\nError in binding socket");
		exit(0);
	} else {
		printf("\nSocket binded successfully");
	}

	if((listen(routerfd, BL)) != 0) {
		printf("\nError in listening");
		exit(0);
	} else {
		printf("\nrouter started listening...");
	}

	len = sizeof(client);
	clientfd = accept(routerfd, (struct sockaddr *) &client, &len);
	if(clientfd < 0) {
		printf("\nError in accepting client");
		exit(0);
	} else {
		printf("\nClient accepted successfully");
	}
	int dnf;
	int f;
	int seq=rand();
	char a[100];
	int MTU;
	read(clientfd,&a,sizeof(char[1000]));
	read(clientfd,&MTU,sizeof(int));
	
	bzero(&client1, sizeof(client1));
	client1.sin_family = AF_INET;
	client1.sin_addr.s_addr = inet_addr("127.0.0.1");
	client1.sin_port = htons(PORT1);
	if(connect(sockfd,(struct sockaddr *) &client1 ,sizeof(client1)) != 0) {
		printf("\nFailed to connect with the server");
        exit(0);
	} else {
		printf("\nConnected to server successfully");
	}
	if(MTU>strlen(a))
		dnf=1;
	write(sockfd, &dnf, sizeof(dnf));
	if(dnf==1)
	    write(sockfd, &a, sizeof(a));
	else
	{
		int f;
		if(strlen(a)%MTU!=0)
	        f=(strlen(a)/MTU)+1;
	    else
			f=strlen(a)/MTU;
		write(sockfd, &f, sizeof(f));
		printf("\n%d packets has to be sent",f);
	    int k=0;
		int a1=0;
		for(int i=0;i<f;i++)
		{
			for(int j=0;j<MTU;j++)
			{
				f1[i].data[j]=a[k];
				k++;
				if(i==(f-1))
				{
					f1[i].MF=0;
				}
				else
					f1[i].MF=1;
			}
			f1[i].off_set=a1;
			f1[i].seqno=seq;
			f1[i].pktl=sizeof(f1[i].pktl);
			printf("\n Sequence number is %d",f1[i].seqno);
			printf("\n Data is %s",f1[i].data);
			printf("\n MF is %d",f1[i].MF);
			printf("\n OFF_set is %d",f1[i].off_set);
			printf("\n Packet length is %d bytes\n",f1[i].pktl);
			write(sockfd, &f1[i].seqno, sizeof(int));
			write(sockfd, &f1[i].data, sizeof(char[1000]));
			write(sockfd, &f1[i].MF, sizeof(int));
			write(sockfd, &f1[i].off_set, sizeof(int));
			write(sockfd, &f1[i].pktl, sizeof(int));
			a1=a1+MTU;
		}
    }
	return 0;
}