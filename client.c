#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <ctype.h>
#define port 4444

int main(int argc, char const *argv[])
{
    FILE *fp;
    char filename[100], c;
    int clientsocket;
    struct sockaddr_in server_address;
    char message[1024], Reply[200], String[256];
    clientsocket = socket(AF_INET, SOCK_STREAM, 0);

    //check connection
    if(clientsocket < 0){
        printf("Socket creation failed.\n");
        exit(1);
    }
    //using the socket server address
    //soocket family
    server_address.sin_family = AF_INET;
    //socket type
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    //socket port
    server_address.sin_port = htons(port);

    //Initializing connection to the server
    int connection_status = connect(clientsocket, (struct sockaddr *)&server_address,sizeof(server_address));
    if(connection_status < 0){
        printf("Connection Failed\n");
        exit(1);
    }


    printf("Connected to Server\n");
    printf("Enter Command to Execute\n\":check\" to check Progress of busylist\n\":exit\" to Close connection\n");
        int number = 0;
        int mainly = 0;
        
     //infinite loop for the client so that the client is able to enter input at any momen
    while(1){
        
        printf("\n\nClient: ");
        gets(message);
        int qp = 0;
        int count = 0;
        char *p = strtok(message, ";");
        while(p!=NULL){
            send(clientsocket, p, 1024, 0);
            p = strtok(NULL, ";");
            number++;
        }
        //if the input by the client is exit, then disconnect from the server   
        if(strcmp(message, ":exit")==0){
            printf("Disconnected from the server\n");
            exit(1);
        }
        if(strcmp(message, ":check")==0){
            fp = fopen("/var/www/html/Recess/waiting.txt", "r");
            for(c= getc(fp);c!=EOF; c=getc(fp))
                if(c=='\n')
                    count = count + 1;
            fclose(fp);

            if(count>2){
                printf("\n");
                for(qp;qp<3;qp++){
                    recv(clientsocket, Reply, 1024, 0);
                    printf("Feedback:  %s\n", Reply);
                }  
            }
            
             goto A;
        }
        A:
        for(mainly;mainly<number;mainly++){
            recv(clientsocket, Reply, 1024, 0);
            char marvelo[500];
            strcpy(marvelo, Reply);
            char none = Reply[0];
            if(isdigit(none)){
                int mok = atoi(Reply);
                printf("\nFeedback:  ");             
                for(int counter = 0;counter<mok;counter++){
                    recv(clientsocket, Reply, 1024, 0);
                    printf("%s", Reply);
                }    
            }else{ 
                printf("\nFeedback:  %s", marvelo);
            }           
        }
              
    }
    return 0;
}