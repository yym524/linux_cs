#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#define SERV_PORT 1030
#define BACKLOG 10
#define BUF_SIZE 255
#define SERV_IP_ADDRESS "127.0.0.1"

void server_recv(int sock_fd){
  char buf[BUF_SIZE]="", start[BUF_SIZE] = "";
  int cnt;
  int i = 0;
  while(1){
    cnt = recv(sock_fd, buf, BUF_SIZE, 0);
    if(cnt == -1){
      perror("recv error!\n");
      exit(-1);
    }
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
    else if(cnt == BUF_SIZE)
      printf("%s",buf);
    memcpy(buf, start, sizeof(char) * BUF_SIZE);
  }
  
}



int main()
{
  int ret;
  int sockfd,  clientsfd;
  struct sockaddr_in serv_addr,  client_addr;
  unsigned int addrlen;
  pid_t pid;
  
  sockfd=socket(AF_INET,SOCK_STREAM,0);
  if(sockfd == -1)
    {
      printf("socket error!\n");
      exit(1);
    }
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
    {
      perror("bind error!\n");
      exit(1);
    }
  printf("bind success!\n");

  ret=listen(sockfd, BACKLOG);
  if(ret == -1)
    {
      perror("listen error!\n");
      exit(1);
    }
  printf("listen success!\n");
  printf("Waiting for the Client connection.\n");
  
  while(1)
    {
      clientsfd=accept(sockfd, 
		       (struct sockaddr *)&client_addr,
		       &addrlen);
      if(clientsfd == -1)
	{
	  perror("accept error!\n");
	  exit(-1);
          }
      printf("accept success!\n");
      printf("Client IP: %s \nBegin recieve:\n",
	     inet_ntoa(client_addr.sin_addr));

      pid = fork();
      if(pid < 0){
	perror("fork error!\n");
	exit(-1);
      }
      else if(pid == 0){
	close(sockfd);
	server_recv(clientsfd);
      }
      else{
	close(clientsfd);
      }
    }//************************************************
}
