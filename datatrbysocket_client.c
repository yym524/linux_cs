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

#define SERV_PORT     1030
#define BACKLOG       10
#define BUF_SIZE      255
#define FILE_TRANS    1
#define WORDS_TRANS   2

int file_trans(char *filename){
  int fd;
  if((fd = open(filename, O_RDONLY)) <0){
    printf("open file %s error!\n", filename);
    exit(-1);
  }
  else
    return fd;
}

int main(int argc, char **argv)
{
     int ret;
     int sockfd;
     int fd;
     struct sockaddr_in * hostaddr;
     struct sockaddr_in serveraddr;
     int addrlen = sizeof(struct sockaddr);
     char buf[BUF_SIZE];
     char endline[BUF_SIZE] = "";
     int cnt;
     int casenum;
     int sendnum;
     
     if(argc != 3){
          printf("argument error!\n");
          return 1;
     }

     if(strcmp(argv[1],"file") == 0)
       casenum = FILE_TRANS;
     else if(strcmp(argv[1], "words") == 0)
       casenum = WORDS_TRANS;
     else{
       printf("wrong type choose!\n");
       exit(1);
     }

     switch(casenum){
     case FILE_TRANS:
       fd = file_trans(argv[2]);
       break;
     case WORDS_TRANS:
       strcpy(buf,argv[2]);
       break;
     default:
       printf("argument error!\nYou can input \'file\' or \'words\'\n");
     }
     
     if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) <0){
          printf("socket error!\n");
          return 1;
     }
     
     serveraddr.sin_family = AF_INET;
     serveraddr.sin_port   = htons(SERV_PORT);
     serveraddr.sin_addr.s_addr = INADDR_ANY;
     bzero(&(serveraddr.sin_zero), 8);
     
     if((ret = connect(sockfd,(struct sockaddr_in *)&serveraddr, addrlen)) <0){
          printf("connect error!\n");
          exit(-1);
     }
     
     off_t lastpos = 0;
     int wdnum = 0;
     printf("Send:\n");

     while(1){
       switch(casenum){
       case FILE_TRANS:
	 lseek(fd, lastpos, SEEK_SET);
	 wdnum = read(fd, buf, 255);
	 if(wdnum < 0){
	   printf("read file %s error!\n", argv[2]);
	   exit(-1);
	 } 
	 else if(wdnum > 0){
	   if(cnt = send(sockfd, buf, wdnum,0) <0){
	     printf("send error!\n");
	     exit(-1);
	   }
	   lastpos += wdnum;
	   printf("%s", buf);
	   //break;
	 }
	 else if(wdnum == 0){
	   close(fd);
	   close(sockfd);
	   exit(0);
	 }
	 memcpy(buf , endline, sizeof(char)*BUF_SIZE); //恢复原始状态
	 break;

       case WORDS_TRANS:
	 sendnum = strlen(buf);
	 cnt=send(sockfd, buf, sendnum, 0);
	 if(cnt == -1)
	   {
	     perror("send error\n");
	     exit(-1);
	   }
	 else{
	   printf("%s\n",buf);
	   close(sockfd);
	   exit(0);
	 }  
       }
     }
     
     close(sockfd);
     return 0;
}
