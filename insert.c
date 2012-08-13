#include<mysql/my_global.h>
#include<mysql.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

typedef struct data_list{
     char data[500];
     struct data_list *next;
}data_List;

data_List *init_linkl(){

     data_List *dt_list;
     dt_list = (data_List *)malloc(sizeof(data_List));
     dt_list->next = NULL;
     dt_list->data;
     
     return dt_list;
}

void add_node(char *name_str, char *time_str, char *info_str, data_List *dt_list){
     
     data_List *pNew;
     size_t str_len;

     pNew = init_linkl();
     dt_list->next = pNew;
     snprintf(pNew->data, sizeof(pNew->data) - 1, "INSERT INTO cmn_rcd_283113886 VALUES (\"%s\",\"%s\",\"%s\");", name_str, time_str, info_str);
     //printf("%s\n",pNew->data);
}

void delete_list(data_List *dt_list){
     data_List *pNode, tmp;
     pNode = dt_list->next;
     dt_list->next = NULL;
     while(pNode != NULL){
          tmp = pNode->next;
          free(pNode);
          pNode = tmp;
     }
}

void data_format(data_List *dt_list, FILE *fp){
     
     char name_str[26] = "";
     char time_str[26] = "";
     char info_str[256] = "";
     char *line = NULL;
     size_t len = 0;
     ssize_t read;
     char *pdest;
     int result;
     int info_flag=0;
     int line_len;
     data_List *pNode;
     
     pNode = dt_list;

     while((read = getline(&line, &len, fp)) != -1){
          
          pdest = strrchr(line, '\n');
          result = pdest - line;
          line_len = result;
          
          if(info_flag == 0){

               memset(name_str, '\0', 26);
               memset(time_str, '\0', 26);
               memset(info_str, '\0', 256);
               strncpy(time_str, line, 19);

               if(line_len > 20){
                    strncpy(name_str, "Server", 7);
                    info_flag = 1;
               }
               else{
                    strncpy(name_str, "Client", 7);
                    info_flag = 1;
               }
          }
          
          else if(info_flag == 1){
               if(read <= 2){
                    info_flag = 0;
                    add_node(name_str, time_str, info_str, pNode);
                    pNode = pNode->next;
                    //printf("pNode : %s\n",pNode->data);
                    //continue;
               }
               else{
                    strncpy(info_str, line, line_len);
               }
          }
     }

}

int main(int argc, char **argv){
     MYSQL *conn;
     FILE *fp;
     if(argc != 2){
          printf("Required 2 arguments!\n");
          exit(-1);
     }

     /*open data file*/
     fp = fopen(argv[1],"r");
     if(fp == NULL)
          exit(EXIT_FAILURE);

     conn = mysql_init(NULL);
     if(mysql_options(conn,MYSQL_SET_CHARSET_NAME,"utf8") != 0){
          printf("MYSQL_OPTIONS Error!\n");
          exit(-1);
     }
     if(mysql_real_connect(conn,"localhost","root","","cmn_record",0,NULL,0) == NULL){
          printf("Error %u:%s\n",mysql_errno(conn),mysql_error(conn));
          exit(EXIT_FAILURE);
     }
  
     data_List *dt_list, header;
     dt_list = init_linkl();
     data_format(dt_list, fp);
     header = dt_list;

     while(dt_list->next != NULL){
          dt_list = dt_list->next;
          if(mysql_query(conn,dt_list->data)){
               printf("Error %u:%s\n",mysql_errno(conn),mysql_error(conn));
               exit(EXIT_FAILURE);
          }
          printf("insert %s\n",dt_list->data);
     }
     
     delete_list(header);
     mysql_close(conn);
     exit(EXIT_SUCCESS);
}
