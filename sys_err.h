#include<stdio.h>
#include<stdlib.h>

int sys_err(const char *str){
     perror(str);
     exit(-1);
} 
