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
char names[50][50];
int users[50];
int numUsers;
void chat(){
    char buffer[1000];
    int n;
    
    while(1){
        
       
        memset(buffer,0,sizeof(buffer));
	    memset( buffer, 0, 1000 );
        
        for(int i=0;i<numUsers;i++){
            if(users[i]!=0){
                int bytesRead = read( users[i], buffer, 1000 );
                buffer[bytesRead] = '\0';
                if(bytesRead==-1){
                    
                }
                if(bytesRead > sizeof(buffer[0])){
                    printf("%s: %s\n",names, buffer );
                }
            }
        }

        
        if ((strncmp(buffer, "quit", 4)) == 0) {
            printf("Client Exit...\n");
            break;
        }
        users[numUsers]=accept4(sockFD,NULL,0);
        if(users[numUsers]==-1){
            if( errno != EAGAIN || errno != EWOULDBLOCK){
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
    memset(users,0,sizeof(users));
    for(int i=0;i<50;i++){
        strcpy(names[i], "User");
    }
    sockFD = socket(AF_INET, SOCK_STREAM  ,0);
    fcntl( sockFD, F_SETFL, O_NONBLOCK );

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
