#include <stdio.h>
#include <netdb.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include "gettime.h"

#define BACKLOG    10
#define BUF_SIZE   255
#define TIME_LEN   25
#define FILE_MODE  (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH | O_SYNC)

int max(int i, int j){
     if(i > j)
          return i;
     else
          return j;
}

void server_process(int sockfd){

     int maxfdp1;
     int fd;
     int recv_num, send_num;
     fd_set rset;
     char filename[11]       = "";
     char cur_time[TIME_LEN] = "";
     char buf[BUF_SIZE]      = "";
     char start[BUF_SIZE]    = "";
     char send_buf[BUF_SIZE] = "";
     
     FD_ZERO(&rset);
     gettime(cur_time, TIME_LEN);
     strncpy(filename, cur_time, 10);
     filename[10] = '\0';
     fd = open(filename, O_WRONLY | O_APPEND | O_CREAT, FILE_MODE);

     for(;;){
          FD_SET(sockfd, &rset);
          FD_SET(STDIN_FILENO, &rset);
          
          maxfdp1 = max(sockfd, STDIN_FILENO) + 1;
          select(maxfdp1, &rset, NULL, NULL, NULL);

          if(FD_ISSET(sockfd, &rset)){
               if((recv_num = recv(sockfd, buf, BUF_SIZE, 0)) <= 1){
                    sys_err("recv error!\n");
               }
               printf("//-*-&-*-&- Begin Recieve -&-*-&-*-//\n");
               gettime(cur_time,TIME_LEN);
               printf("%s\n%s",cur_time, buf);
               write(fd, cur_time, 19);
               write(fd, " from client.\n", 14);
               write(fd, buf, recv_num);
               write(fd, "\n", 1);
               printf("\n//-*-&-*-&- Recieved Over! -&-*-&-*-//\n");
               memcpy(buf, start, sizeof(char) * BUF_SIZE);

          }

          if(FD_ISSET(STDIN_FILENO, &rset)){
               if(fgets(send_buf, BUF_SIZE-1, stdin) == NULL){
                    sys_err("fgets error!\n");
               }
               printf("//-*-&-*-&- Begin Send -&-*-&-*-//\n");
               send_num = send(sockfd, send_buf, strlen(send_buf), 0);
               if(send_num < 0)
                    sys_err("send error!\n");
               gettime(cur_time,TIME_LEN);
               write(fd, cur_time, 19);
               write(fd, "\n", 1);
               write(fd, send_buf, send_num);
               write(fd, "\n", 1);
               printf("%s\nsend success:%s\n",cur_time, send_buf);
               memcpy(send_buf,start,sizeof(char) * BUF_SIZE);
          }
     }
}

/*
当在服务器端输入^d 或者^c时候，客户端出现不停的接受的情况。
应该是个不正常的字符传输。不论是在客户端还是服务器端
都会出现乱码情况。
*/
void client_process(int sockfd){
    
     int maxfdp1;
     int fd;
     int recv_num, send_num;
     fd_set rset;
     char filename[11]       = "";
     char cur_time[TIME_LEN] = "";
     char buf[BUF_SIZE]      = "";
     char start[BUF_SIZE]    = "";
     char send_buf[BUF_SIZE] = "";
     
     FD_ZERO(&rset);
     gettime(cur_time, TIME_LEN);
     strncpy(filename, cur_time, 10);
     filename[10] = '\0';
     fd = open(filename, O_WRONLY | O_APPEND | O_CREAT, FILE_MODE);

     for(;;){
          FD_SET(sockfd, &rset);
          FD_SET(STDIN_FILENO, &rset);
          
          maxfdp1 = max(sockfd, STDIN_FILENO) + 1;
          select(maxfdp1, &rset, NULL, NULL, NULL);

          if(FD_ISSET(sockfd, &rset)){
               if((recv_num = recv(sockfd, buf, BUF_SIZE, 0)) <= 1){
                    sys_err("recv error!\n");
               }
               printf("//-*-&-*-&- Begin Recieve -&-*-&-*-//\n");
               gettime(cur_time,TIME_LEN);
               printf("%s\n%s",cur_time, buf);
               write(fd, cur_time, 19);
               write(fd, " from client.\n", 14);
               write(fd, buf, recv_num);
               write(fd, "\n", 1);
               printf("\n//-*-&-*-&- Recieved Over! -&-*-&-*-//\n");
               memcpy(buf, start, sizeof(char) * BUF_SIZE);

          }

          if(FD_ISSET(STDIN_FILENO, &rset)){
               if(fgets(send_buf, BUF_SIZE-1, stdin) == NULL){
                    sys_err("fgets error!\n");
               }
               printf("//-*-&-*-&- Begin Send -&-*-&-*-//\n");
               send_num = send(sockfd, send_buf, strlen(send_buf), 0);
               if(send_num < 0)
                    sys_err("send error!\n");
               gettime(cur_time,TIME_LEN);
               write(fd, cur_time, 19);
               write(fd, "\n", 1);
               write(fd, send_buf, send_num);
               write(fd, "\n", 1);
               printf("%s\nsend success:%s\n",cur_time, send_buf);
               memcpy(send_buf,start,sizeof(char) * BUF_SIZE);
          }
     }
}
