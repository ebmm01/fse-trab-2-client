#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>

#include <sys/types.h>    // AF_INET, SOCK_STREAM
#include <sys/socket.h>   // socket(), connect()
#include <arpa/inet.h>    // htons(), inet_addr()
#include <unistd.h>       // close()

#include "socket_handler.h"

#define MAX 80
#define PORT 9011
#define SA struct sockaddr
#define SERVER_IP "192.168.0.53"
#define MESSAGE_WAIT_TIME_USECONDS 100000


int sockfd;

void* handle_socket_received_messages()
{
    int dc_times = 0;

    char buff[MAX];

    for (;;) {
        bzero(buff, sizeof(buff));

        // Receive client's message:
        if (recv(sockfd, buff, sizeof(buff), 0) < 0){
            dc_times++;
        }
        else if (buff[0] == '\0') {
            dc_times++;
        }
        else {
            handle_socket_message(buff);
            printf("Msg from server: %s\n",  buff);
        }

        usleep(MESSAGE_WAIT_TIME_USECONDS);
    }

    return 0;
}

void send_message_to_server(char* message, int messageSize) {
    if (sockfd == -1) {
       printf("Erro ao enviar msg. Socket não conectado...\n"); 
    }
    else {
        int result = send(sockfd, message, messageSize, 0);

        if (!result) {
            printf("Erro ao enviar msg\n");
        }
            
    }
};

void close_socket() {
    close(sockfd);
}
  
int init_socket()
{
    struct sockaddr_in servaddr;
  
    // socket create and varification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));
  
    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(SERVER_IP);
    servaddr.sin_port = htons(PORT);
  
    // connect the client socket to server socket
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
        printf("connected to the server..\n");
  
   
    return 0;
}