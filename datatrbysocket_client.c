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
#include"rs.h"

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
     char buf[BUF_SIZE];
     char endline[BUF_SIZE] = "";
     int cnt;
     int casenum;
     int sendnum;
     
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

     while(1){
          printf("Please input words you want say:");
          fgets(buf,BUF_SIZE,stdin);
          sendnum = strlen(buf);
          cnt=send(sockfd, buf, sendnum, 0);
          if(cnt == -1)
               sys_err("send error!\n");
          else{
               printf("%s\n",buf);
               printf("//-*-&-*-&-  Send  -*-*-*-  Over!  -&-*-&-//\n");
               memcpy(buf , endline, sizeof(char)*BUF_SIZE); //恢复原始状态
          } 
          cnt = recv(sockfd, buf, BUF_SIZE, 0);
          if(cnt == -1)
               sys_err("recv error!\n");
          else{
               printf("//-*-&-*-&- Begin Recieve -&-*-&-*-//\n");
               printf("%s",buf);
               printf("\n//-*-&-*-&- Recieved Over! -&-*-&-*-//\n");
               memcpy(buf, endline, sizeof(char) * BUF_SIZE);
          }
     } 
     return 0;
}
