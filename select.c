#include<mysql/my_global.h>
#include<mysql.h>
#include<stdio.h>
#include<stdlib.h>

int main(int argc, char **argv){
  MYSQL *conn;
  MYSQL_RES *result;
  MYSQL_ROW row;
  MYSQL_FIELD *field;

  int num_fields;
  int i;
  int j = 0;

  conn = mysql_init(NULL);
  if(mysql_real_connect(conn,"localhost","root","","chd_user_info",0,NULL,0) == NULL){
    printf("Error %u:%s\n",mysql_errno(conn),mysql_error(conn));
    exit(EXIT_FAILURE);
  }

  char *sql = "SELECT * FROM user_info_1";

  if(mysql_query(conn, sql)){
    printf("Error %u:%s\n",mysql_errno(conn),mysql_error(conn));
    exit(EXIT_FAILURE);
  }
  
  result = mysql_store_result(conn);
  num_fields = mysql_num_fields(result);//行数
  printf("%d\n",num_fields);

  while((row = mysql_fetch_row(result))){//检索结果集的下一行
    for(i = 0; i< num_fields; i++){
      if(j == 0){
	while(field = mysql_fetch_field(result)){//返回采用MYSQL_FIELD结构的结果集的列
	  /*if(i == 6 || i == 7)
	    printf("%-15s",field->name);
	  else if(i == 8 || i == 9)
	    printf("%-11s",field->name);
	    else*/
	  printf("%-15s",field->name);
	}
	printf("\n");
	}

      j++;
      /*if(i == 6 || i == 7)
	printf("%-9s",row[i] ? row[i]:"NULL");
      else if(i == 8 || i == 9)
	printf("%-11s",row[i] ? row[i]:"NULL");
	else*/
      printf("%-15s",row[i] ? row[i]:"NULL");
    }
    printf("\n");
  }
  printf("\n");
  mysql_free_result(result);
  
  mysql_close(conn);
  exit(EXIT_SUCCESS);
}
