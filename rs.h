#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include"gettime.h"

#define SERV_PORT       1030
#define BACKLOG         10
#define BUF_SIZE        255
#define SERV_IP_ADDRESS "127.0.0.1"
#define TIME_LEN        25


void server_process(int sock_fd){
     char cur_time[TIME_LEN]="";
     char buf[BUF_SIZE]="", start[BUF_SIZE] = "";
     char send_buf[BUF_SIZE]="";
     int cnt,send_num;
     int i = 0;
     while(1){
          cnt = recv(sock_fd, buf, BUF_SIZE, 0);
          if(cnt == -1)
               sys_err("recv error!\n");
          else{
               printf("//-*-&-*-&- Begin Recieve -&-*-&-*-//\n");
               gettime(cur_time,TIME_LEN);
               printf("%s\n%s",cur_time, buf);
               printf("\n//-*-&-*-&- Recieved Over! -&-*-&-*-//\n");
               memcpy(buf, start, sizeof(char) * BUF_SIZE);
          }
          printf("//-*-&-*-&- Begin Send -&-*-&-*-//\n");
          printf("Please input words you want say:\n");
          fgets(send_buf,255,stdin);
          send_num = send(sock_fd,send_buf,sizeof(send_buf),0);
          if(send_num < 0)
               sys_err("send error!\n");
          else{
               gettime(cur_time,TIME_LEN);
               printf("%s\nsend success:%s\n",cur_time, send_buf);
               
          }
          memcpy(send_buf,start,sizeof(char) * BUF_SIZE);
     }
}

void client_process(int sock_fd){
     char cur_time[TIME_LEN];
     int cnt;
     int sendnum;
     char buf[BUF_SIZE];
     char endline[BUF_SIZE] = "";
     while(1){
          printf("Please input words you want say:");
          fgets(buf,BUF_SIZE,stdin);
          sendnum = strlen(buf);
          cnt=send(sock_fd, buf, sendnum, 0);
          if(cnt == -1)
               sys_err("send error!\n");
          else{
               gettime(cur_time,TIME_LEN);
               printf("%s\nsend success:%s\n",cur_time, buf);
               printf("//-*-&-*-&-  Send  -*-*-*-  Over!  -&-*-&-//\n");
               memcpy(buf , endline, sizeof(char)*BUF_SIZE); //恢复原始状态
          } 
          cnt = recv(sock_fd, buf, BUF_SIZE, 0);
          if(cnt == -1)
                sys_err("recv error!\n");
          else{
               printf("//-*-&-*-&- Begin Recieve -&-*-&-*-//\n");
               gettime(cur_time,TIME_LEN);
               printf("%s\n%s",cur_time, buf);
               printf("\n//-*-&-*-&- Recieved Over! -&-*-&-*-//\n");
               memcpy(buf, endline, sizeof(char) * BUF_SIZE);
          }
     }
}
