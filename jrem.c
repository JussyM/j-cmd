#define _GNU_SOURCE
#include "stdio.h"
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void add_cmd(int fd,char* arg)
{
	int arg_size = strlen(arg);
	write(fd,arg,arg_size);
	write(fd,"\n",1);
	close(fd);
}
void rm_cmd(int fd,int index)
{
	FILE *f_stream= fopen(fd,"w+");
	
}
void show_cmd(int fd,int index){}
void exec_cmd(int fd, int index){}
void find(int fd,char *){}

int main(int argc, char *argv[])
{
	int fd;
	if(argc>3)
	{
		printf("invalid input\n");
		exit(1);
	}
	fd= open(".jrem",O_WRONLY|O_CREAT|O_APPEND,0666);
	if(fd<0)
	{
		printf("can not open config file\n");
		exit(1);
	}
	char* cmd = argv[1];
	char* arg = argv[2];
	switch(cmd[0])
	{
		case 'a':
		add_cmd(fd,arg);
		break;
		case 'r':
		break;
		case 's':
		break;
		case 'e':
		break;
		case 'i':
		break;
		case 'f':
		break;
		default:
		printf("Unknow command\n");
		break;

	}

	return 0;
}
