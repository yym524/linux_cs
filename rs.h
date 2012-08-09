#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<arpa/inet.h>

#define SERV_PORT 1030
#define BACKLOG 10
#define BUF_SIZE 255
#define SERV_IP_ADDRESS "127.0.0.1"

int sys_err(const char *str){
     perror(str);
     exit(-1);
} 

void server_recv(int sock_fd){
     char buf[BUF_SIZE]="", start[BUF_SIZE] = "";
     int cnt;
     int i = 0;
     while(1){
          cnt = recv(sock_fd, buf, BUF_SIZE, 0);
          if(cnt == -1)
               sys_err("recv error!\n");
          else if(cnt < BUF_SIZE){
               if(memcmp(buf,"close",5) == 0){
                    printf("Recieved exit command!\n//-*-&-*-&- Server -*-*-*- Closed! -&-*-&-*//\n");
                    exit(1);
               }
               else{
                    printf("%s\n//-*-&-*-&- OVER! -&-*-&-*-//\n",buf);
                    exit(0);
               }
          }
          else if(cnt == BUF_SIZE){
               printf("%s",buf);
               memcpy(buf, start, sizeof(char) * BUF_SIZE);
          }      
     }
}
