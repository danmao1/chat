//group9
#define _GNU_SOURCE
#include <sys/types.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

//GRADING NOTE: Successfully transmits messages to multiple clients.
//GRADING NOTE: Successfully connects three clients.
//GRADING NOTE: Successfully connects new clients after others quit.
//GRADING NOTE: Successfully allows name changes and transmits names
// with messages.
//GRADING NOTE: Successfully allows clients to quit with keyword.
//Does not respond to close messages such as "quite right"

//GRADING NOTE: After a short conversation, the system appeared to become 
//confused over which name belonged to which client, and would send messages
//as though they were coming from multiple client simultaneously. -5 points

//GRADING NOTE: After a client crashed, some other clients were unable
//to communicate. Newly connected clients were fine. This plus the above
//makes me think that your linked list / client tracking has got a bug
//somewhere.

//GRADING NOTE: If the server crashes then the clients do not notice
// or disconnect. If a client crashes then the server terminates.
// The server in particular should survive client errors. -5 points

//boradcast for all function
int sockFD;
char names[50][50];
int users[50];
int numUsers;
char buffer[1000];
char toSend[1051];
void broadcast_for_all(int noprint_to_this_User){
    for (int i=0;i<numUsers;i++){
        if(users[i]!=0 && i!=noprint_to_this_User){
            write(users[i],toSend,1000);
        }
    }
    
}
void chat(){
    char* str;
    int n;
    
    while(1){
        
       
        memset(buffer,0,sizeof(buffer));
	    
        
        for(int i=0;i<numUsers;i++){
            if(users[i]!=0){
                int bytesRead = read( users[i], buffer, 1000 );
                buffer[bytesRead] = '\0';
                if(bytesRead==-1){
                    if(errno != EAGAIN && errno != EWOULDBLOCK){
                        users[i]=0;
                    }
                }
                if(buffer[0]!=0){
                    if((strncmp(buffer,"name",4))==0){
                        char temp[50];
                        int index=0;
                        
                        memcpy(temp,&buffer[5],50);
                        for(int t=0;t<50;t++){
                            if(temp[t]=='\n'){
                                temp[t]='\0';
                                break;
                            }

                        }
                        //snprintf(&(buffer[5]),max_size,"%s",username);
                        snprintf(toSend,1051,"%s change its name to %s\n",names[i],temp);
                        printf("%s\n",toSend);
                        broadcast_for_all(i);
                        strcpy(names[i],temp);
                        memset(buffer,0,sizeof(buffer));
                        
                    }
                    else if ((strncmp(buffer, "quit", 4)) == 0) {
                        users[i]=0;
                        snprintf(toSend,1051,"%s has disconnected\n",names[i]);
                        broadcast_for_all(i);
                        printf("%s has disconnected\n",names[i]);
                    }
                    else{
                        snprintf(toSend,1051,"%s: %s\n",names[i],buffer);
                        printf("%s\n",toSend);
                        broadcast_for_all(i);
                    }
                    
                }
            }
        }
        
        
        
        users[numUsers]=accept(sockFD,NULL,0);
        
        if(users[numUsers]==-1){
            if( errno != EAGAIN && errno != EWOULDBLOCK){
                users[numUsers]=0;
                perror("Error in accept\n");
            }
            
        }
        else{
            fcntl(users[numUsers],F_SETFL,O_NONBLOCK);
            snprintf(toSend,1051,"User has connected\n");
            printf("User has connected\n");
            broadcast_for_all(-1);
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
    sockFD = socket(AF_INET,SOCK_STREAM ,0);
    fcntl(sockFD,F_SETFL,O_NONBLOCK);

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
    users[numUsers] = accept(sockFD,NULL,0);
    numUsers++;
    chat();
    close(sockFD);
    


    return 0;
}
