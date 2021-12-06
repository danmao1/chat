#define _GNU_SOURCE
#include <sys/types.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int sockFD;
int users[100];
int numUsers;
void chat(){
    char buffer[1000];
    int n;
    
    while(1){
        
       
        memset(buffer,0,sizeof(buffer));
        char buffer[ 1024 ];
	    memset( buffer, 0, 1024 );
        
        for(int i=0;i<numUsers;i++){
            int bytesRead = read( users[i], buffer, 1024 );
            buffer[bytesRead] = '\0';
        }

        printf("User: %s\n", buffer );
        if ((strncmp(buffer, "quit", 4)) == 0) {
            printf("Client Exit...\n");
            break;
        }
        users[numUsers]=accept4(sockFD,NULL,0);
        if(users[numUsers]==-1){
            if( errno == EAGAIN || errno==EWOULDBLOCK){
                
            }
            else {
                perror("Error in accept\n");
            }
        }
        else{
            numUsers++;
        }
    }
}

int main(int argc, char* argv[]){
    if (argc!=2){
        printf("Usage: ./server <port number> \n");
        return 0;
    }
    int portNum= atoi(argv[1]);
    numUsers=0;
    sockFD = socket(AF_INET, SOCK_STREAM  ,0);

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
    users[numUsers] = accept4(sockFD,NULL,0);
    printf("User has connected\n");
    numUsers++;
    chat();
    close(sockFD);
    


    return 0;
}
