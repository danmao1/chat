#include <sys/types.h>         
#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
int sockFD;
char buffer[1000];
void* writeChat(void* args){// separate thread functions by write and read
    int n;
    while(1){
        memset(buffer,0,sizeof(buffer));
        n=0;
        while((buffer[n++] = getchar()) != '\n');
        write(sockFD, buffer, sizeof(buffer));
        memset(buffer,0,sizeof(buffer));
        
        
    }
    
}
void* readChat(void* args){// separate thread functions by write and read
    
    int n;
    while(1){
        memset(buffer,0,sizeof(buffer));
        read(sockFD, buffer, sizeof(buffer));
        if ((strncmp(buffer, "quit", 4)) == 0) {
            printf("Client Exit...\n");
            break;
        }
        
    }
    
}

int main (int argc,char* argv[]){
    pthread_t tidRead[2];
    pthread_t tidWrite[2];
    if(argc != 3){
        printf("Usage: ./server (ip address) (port number)\n");
        return 0;

    }
    char* ipAddr = argv[1];
    int portNum = atoi( argv[2] );

    sockFD = socket( AF_INET, SOCK_STREAM , 0 );
    if( sockFD == -1 ){
        perror("Could not create socket\n");
        return -1;
    }
    struct sockaddr_in sockaddr;
    memset(&sockaddr,0,sizeof(struct sockaddr_in));
    sockaddr.sin_family=AF_INET;
    sockaddr.sin_port=htons(portNum);
    int retVal=inet_aton(ipAddr,&sockaddr.sin_addr);
    if( retVal == 0 ){
		printf("Error: IP address not valid\n");
		return -1;
	}
    retVal = connect( sockFD, (struct sockaddr*) &sockaddr, sizeof(struct sockaddr_in));
	if( retVal == -1 ){
		perror("Could not connect to server\n");
		return -1;
	}
    for(int i=0;i<2;i++){
        
        int pthread	= pthread_create( &tidWrite[i], NULL, writeChat, NULL );
        if (pthread!=0){
            printf("Unsuccessful thread\n");
        }
        

    }
    for(int i=0;i<2;i++){
        
        int pthread	= pthread_create( &tidRead[i], NULL, readChat, NULL );
        if (pthread!=0){
            printf("Unsuccessful thread\n");
        }
        

    }
    for(int i=0;i<2;i++){
        
        int join	= pthread_join( tidWrite[i], NULL);
        if (join!=0){
            printf("Thread not joined\n");
        }
    }
    
    for(int i=0;i<2;i++){
        
        int join	= pthread_join( tidRead[i], NULL);
        if (join!=0){
            printf("Thread not joined\n");
        }
    }
    


    return 0;
}
