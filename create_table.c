#include<mysql/my_global.h>
#include<mysql.h>
#include<stdio.h>
#include<stdlib.h>

int main(int argc, char **argv){
  MYSQL *conn;

  unsigned int ret;
  conn =  mysql_init(NULL);
  ret = mysql_options(conn,MYSQL_SET_CHARSET_NAME,"utf8");
  if(ret == 0){
    printf("MYSQL_OPTIONS SUCCESS!\n");
  }
  //host user password dbname
  if(mysql_real_connect(conn,"localhost","root","","cmn_record",0,NULL,0) == NULL){
    printf("Error %u:%s\n",mysql_errno(conn),mysql_error(conn));
    exit(EXIT_FAILURE);
  }
  char *sql="CREATE TABLE IF NOT EXISTS user_info_test(username VARCHAR(24) NOT NULL,cmn_time DATETIME NOT NULL,cmn_info VARCHAR(255) NOT NULL) default charset=utf8";
  if(mysql_query(conn,sql)){
    printf("Error %u:%s\n",mysql_errno(conn),mysql_error(conn));
    exit(EXIT_FAILURE);
  }

  mysql_close(conn);
  exit(EXIT_SUCCESS);
}
