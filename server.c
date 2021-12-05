#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

void chat(int sockfd){
    char buffer[1000];
    
}

int main(int argc, char* argv[]){
    if (argc!=2){
        printf("Usage: ./server <port number> \n");
        return 0;
    }
    int portNum= atoi(argv[1]);

    int sockFD = socket(AF_INET, SOCK_STREAM,0);
    if(sockFD== -1){
        perror("Could not create socket\n");
        return -1;
    }
    struct sockaddr_in sockaddr;
    memset(&sockaddr,0,sizeof(struct sockaddr_in));
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(portNum);
    inet_aton("INADRR_ANY",&sockaddr.sin_addr);
    int ret = bind(sockFD,&sockaddr,sizeof(struct sockaddr_in));
    if(ret ==-1){
        perror("Could not bind socket\n");
        return -1;

    }
    ret= listen(sockFD,5);
    printf("Waiting for incoming connection...\n");
    int clientFD = accept(sockFD,NULL,0);
    printf("User has connected\n");
    


    return 0;
}