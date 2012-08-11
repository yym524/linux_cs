#include<stdio.h>
#include<unistd.h>
#include<time.h>
#include<stdlib.h>
#include<string.h>
#include"sys_err.h"

void gettime(char *str, int len){
     struct tm *cur_tm;
     time_t today;
     
     today = time(NULL);
     if(today == -1)
          sys_err("time functon error!\n");
     else
          cur_tm = localtime((const time_t *)&today);
     
     strftime(str, len, "%F  %T", cur_tm);
     //we can get format like this: 2012-08-10  13:07:08
}

/*testing

void main(){
     char cur_time[25];
     gettime(cur_time,25);
     printf("%s\n",cur_time);
}
*/
