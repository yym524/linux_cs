#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<errno.h>
#include<sys/types.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>
#include"rs_fdw.h"

#define SERV_PORT     1030
#define BACKLOG       10
#define BUF_SIZE      255
#define FILE_TRANS    1
#define WORDS_TRANS   2

int main(int argc, char **argv)
{
     int ret;
     int sockfd;
     struct sockaddr_in * hostaddr;
     struct sockaddr_in serveraddr;
     int addrlen = sizeof(struct sockaddr);
     
     if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) <0)
          sys_err("socket error!\n");
     else 
          printf ("socket success!\n");
     
     serveraddr.sin_family = AF_INET;
     serveraddr.sin_port   = htons(SERV_PORT);
     serveraddr.sin_addr.s_addr = INADDR_ANY;
     bzero(&(serveraddr.sin_zero), 8);
     
     if((ret = connect(sockfd,(struct sockaddr *)&serveraddr, addrlen)) <0)
          sys_err("connect error!\n");
     else 
          printf("connect success!\n");
     printf("Send:\n");

     client_process(sockfd);

     return 0;
}
