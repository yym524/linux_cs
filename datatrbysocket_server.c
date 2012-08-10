#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include"rs.h"

#define SERV_PORT 1030
#define BACKLOG 10
#define BUF_SIZE 255
#define SERV_IP_ADDRESS "127.0.0.1"

int main()
{
     int ret;
     int sockfd,  clientsfd;
     struct sockaddr_in serv_addr,  client_addr;
     unsigned int addrlen;
     pid_t pid;
  
     sockfd=socket(AF_INET,SOCK_STREAM,0);
     if(sockfd == -1)
          sys_err("sockfd error!\n");
     else
          printf("socket success!\n");
     
     addrlen=sizeof(struct sockaddr_in);
     memset(&serv_addr, 0, addrlen);
     serv_addr.sin_family=AF_INET;
     serv_addr.sin_port=htons(SERV_PORT);
     serv_addr.sin_addr.s_addr=inet_addr(SERV_IP_ADDRESS);
     bzero(&(serv_addr.sin_zero),8);
     
     ret=bind(sockfd, (struct sockaddr *)&serv_addr,
              addrlen);
     if(ret == -1)
          sys_err("bind error!\n");
     else
          printf("bind success!\n");
     
     ret=listen(sockfd, BACKLOG);
     if(ret == -1)
          sys_err("listen error!\n");
     else
          printf("listen success!\n");
     printf("Waiting for the Client connection.\n");
     
     while(1)
     {
          clientsfd=accept(sockfd, 
                           (struct sockaddr *)&client_addr,
                           &addrlen);
          if(clientsfd == -1)
               sys_err("accept error!\n");
          else
               printf("accept success!\n");
          printf("Client IP: %s \nBegin recieve:\n",
                 inet_ntoa(client_addr.sin_addr));
          
          pid = fork();
          if(pid < 0)
               sys_err("fork error!\n");
          else if(pid == 0){
               close(sockfd);
               server_process(clientsfd);
          }
          else{
               close(clientsfd);
          }
     }//**********************while**************************
}
