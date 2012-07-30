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

int main()
{
  int ret;
  int sockfd;
  int clientsfd;
  struct sockaddr_in host_addr;
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
  host_addr.sin_family=AF_INET;
  host_addr.sin_port=htons(SERV_PORT);
  host_addr.sin_addr.s_addr=INADDR_ANY;
  bzero(&(host_addr.sin_zero),8);
  ret=bind(sockfd, (struct sockaddr_in *)&host_addr,
	   sizeof(host_addr));
  if(ret == -1)
    {
      printf("bind error!\n");
      exit(1);
    }
  while(1){
    ret=listen(sockfd, BACKLOG);
    if(ret == -1)
      {
	perror("listen error!\n");
	exit(1);
      }
    printf("Waiting for the Client connection.\n");
    addrlen=sizeof(struct sockaddr_in);
    
    while(1)
      {
	clientsfd=accept(sockfd, 
			 (struct sockaddr_in *)&client_addr,
			 &addrlen);
	if(clientsfd == -1)
          {
	    perror("accept error!\n");
	    continue;
          }
	printf("Client IP: %s \nRecived:\n",
	       inet_ntoa(client_addr.sin_addr));
	while(1){
	  cnt= recv(clientsfd, buf, BUF_SIZE, 0);
	  if(cnt == -1)
	    {
	      perror("recv error!\n");
	      exit(-1);
	      //break;
	    }	
	  else if(cnt < BUF_SIZE){
	    if(memcmp(buf,"close",5) == 0)
	      {
		printf("//-*-&-*-&-  Server  -*-*-*-  Closed!  -&-*-&-*//\n");
		close(clientsfd);
		exit(1);
	      }
	    else{
	      printf("%s\n//-*-&-*-&-  OVER!  -&-*-&-*-//\n",buf);
	      close(clientsfd);
	      memcpy(buf, start, sizeof(char) * BUF_SIZE);
	      break;
	    }
	  }
	  else if(cnt == BUF_SIZE)
	    //printf("%d\n",cnt);
	    printf("%s",buf);
	  //memcpy(buf, start, sizeof(char) * BUF_SIZE);
	}	
        
	//close(clientsfd);
      }
  }//************************************************
 /*   
     ret=listen(sockfd, BACKLOG);
     if(ret == -1)
     {
          perror("listen error!\n");
          exit(1);
     }
     printf("Waiting for the Client connection.\n");
     addrlen=sizeof(struct sockaddr_in);
       
     while(1)
     {
          clientsfd=accept(sockfd, 
                           (struct sockaddr_in *)&client_addr,
                           &addrlen);
          if(clientsfd == -1)
          {
               perror("accept error!\n");
               continue;
          }
          printf("Client IP: %s \nRecive:\n",
                 inet_ntoa(client_addr.sin_addr));
          while(1){
               
               cnt= recv(clientsfd, buf, BUF_SIZE, 0);
               if(cnt == -1)
               {
                    perror("recv error!\n");
                    exit(1);
               }
               else if(cnt < BUF_SIZE){
                    buf[cnt] = NULL;
                    printf("%s",buf);
                    exit(1);
               }
               
               printf("%s",buf);
          }
          
          close(clientsfd);
     }*/
     
     return 0;
}
