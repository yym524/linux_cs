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


int server_recv(int sock_fd, char *buf, char *start){
  int cnt;
  cnt = recv(sock_fd, buf, BUF_SIZE, 0);
  if(cnt == -1){
    perror("recv error!\n");
    exit(-1);
  }
  else{
    if(cnt == 0){
      printf("//-*-&-*-&-  OVER!  -&-*-&-*-//\n");
      close(sock_fd);
    }
    else if(cnt < BUF_SIZE){
      if(memcmp(buf,"close",5) == 0)
	{
	  printf("Recieved exit command!\n//-*-&-*-&-  Server  -*-*-*-  Closed!  -&-*-&-*//\n");
	  close(sock_fd);
	  exit(0);
	}
      else{
	printf("%s\n",buf);
	//close(sock_fd);
	memcpy(buf, start, sizeof(char) * BUF_SIZE);
      }
    }
    else if(cnt == BUF_SIZE)
      printf("%s",buf); 
    return cnt;
  }
}



int main()
{
  int ret;
  int sockfd;
  int clientsfd;
  struct sockaddr_in serv_addr;
  struct sockaddr_in client_addr;
  unsigned int addrlen;
  char buf[BUF_SIZE]="";
  char start[BUF_SIZE] = "";
  int cnt;
  
  sockfd=socket(AF_INET,SOCK_STREAM,0);
  if(sockfd == -1)
    {
      printf("socket error!\n");
      exit(1);
    }
  printf("socket success!\n");
  serv_addr.sin_family=AF_INET;
  serv_addr.sin_port=htons(SERV_PORT);
  serv_addr.sin_addr.s_addr=INADDR_ANY;
  bzero(&(serv_addr.sin_zero),8);
  addrlen=sizeof(struct sockaddr_in);

  ret=bind(sockfd, (struct sockaddr *)&serv_addr,
	   addrlen);
  if(ret == -1)
    {
      perror("bind error!\n");
      exit(1);
    }
  printf("bind success!\n");

  while(1){
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
	    continue;
          }
	printf("accept success!\n");
	printf("Client IP: %s \nRecived:\n",
	       inet_ntoa(client_addr.sin_addr));
	while(1){
	  cnt = server_recv(clientsfd, (char *)buf, (char *)start);
	  if(cnt == 0)
	    break;
	}	
      }
  }//************************************************
     return 0;
}
