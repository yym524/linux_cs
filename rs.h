#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<fcntl.h>
#include"gettime.h"

#define BACKLOG         10
#define BUF_SIZE        255
#define TIME_LEN        25
#define	FILE_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH | O_SYNC)

void server_process(int sock_fd){
     char filename[11];
     char cur_time[TIME_LEN]="";
     char buf[BUF_SIZE]="", start[BUF_SIZE] = "";
     char send_buf[BUF_SIZE]="";
     int cnt,send_num;
     int fd;

     gettime(cur_time, TIME_LEN);
     strncpy(filename, cur_time, 10);
     filename[10] = '\0';
     fd = open(filename, O_WRONLY | O_APPEND | O_CREAT, FILE_MODE);

     while(1){
          cnt = recv(sock_fd, buf, BUF_SIZE, 0);
          if(cnt == -1)
               sys_err("recv error!\n");
          else{
               printf("//-*-&-*-&- Begin Recieve -&-*-&-*-//\n");
               gettime(cur_time,TIME_LEN);
               printf("%s\n%s",cur_time, buf);
               write(fd, cur_time, 20);
               write(fd, " from client.\n", 14);
               write(fd, buf, cnt);
               write(fd, "\n", 1);
               printf("\n//-*-&-*-&- Recieved Over! -&-*-&-*-//\n");
               memcpy(buf, start, sizeof(char) * BUF_SIZE);
          }
          printf("//-*-&-*-&- Begin Send -&-*-&-*-//\n");
          printf("Please input words you want say:\n");
          fgets(send_buf,255,stdin);
          send_num = strlen(send_buf);
          cnt = send(sock_fd,send_buf,send_num, 0);
          if(cnt < 0)
               sys_err("send error!\n");
          else{
               gettime(cur_time,TIME_LEN);
               write(fd, cur_time, 20);
               write(fd, "\n", 1);
               write(fd, send_buf, cnt);
               write(fd, "\n", 1);
               printf("%s\nsend success:%s\n",cur_time, send_buf);
               
          }
          memcpy(send_buf,start,sizeof(char) * BUF_SIZE);
     }
}

void client_process(int sock_fd){
     char filename[11];
     char cur_time[TIME_LEN];
     int cnt, fd;
     int sendnum;
     char buf[BUF_SIZE];
     char endline[BUF_SIZE] = "";

     gettime(cur_time,TIME_LEN);
     strncpy(filename, cur_time, 10);
     filename[10] = '\0';
     fd = open(filename, O_WRONLY | O_APPEND | O_CREAT, FILE_MODE);
     
     while(1){
          printf("Please input words you want say:");
          fgets(buf,BUF_SIZE,stdin);
          
          sendnum = strlen(buf);
          printf("%d\n",sendnum);
          cnt=send(sock_fd, buf, sendnum, 0);
          if(cnt == -1)
               sys_err("send error!\n");
          else{
               gettime(cur_time,TIME_LEN);
               write(fd, cur_time, 20);
               write(fd, "\n", 1);
               printf("%s\nsend success:%s\n",cur_time, buf);
               write(fd, buf, cnt);
               write(fd, "\n", 1);
               printf("//-*-&-*-&-  Send  -*-*-*-  Over!  -&-*-&-//\n");
               memcpy(buf , endline, sizeof(char)*BUF_SIZE); //恢复原始状态
          } 
          cnt = recv(sock_fd, buf, BUF_SIZE, 0);
          if(cnt == -1)
                sys_err("recv error!\n");
          else{
               printf("//-*-&-*-&- Begin Recieve -&-*-&-*-//\n");
               gettime(cur_time,TIME_LEN);
               write(fd, cur_time, 20);
               write(fd, " from server.\n", 14);
               printf("%s\n%s",cur_time, buf);
               write(fd, buf, cnt);
               write(fd, "\n", 1);
               printf("\n//-*-&-*-&- Recieved Over! -&-*-&-*-//\n");
               memcpy(buf, endline, sizeof(char) * BUF_SIZE);
          }
     }
}
