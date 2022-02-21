#define _GNU_SOURCE
#include <stdio.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
/**
 * @brief add_cmd
 * @param fd
 * @param arg
 */
void add_cmd(int fd,char* arg)
{
	int arg_size = strlen(arg);
	write(fd,arg,arg_size);
	write(fd,"\n",1);
	close(fd);
}
/**
 * @brief rm_cmd
 * @param index
 */
void rm_cmd(int index)
{
	char temp[60];
	int line =1;
	if(index!=0)
	{
		FILE *f_stream= fopen(".jrem","r");
		FILE *f_stream_dup = fopen(".jrem-dup","w");
		while(fgets(temp,60,f_stream)!=NULL)
		{
			if(line!=index)
			{
				fputs(temp,f_stream_dup);

			}
			++line;

		}
		fclose(f_stream);
		fclose(f_stream_dup);
		remove(".jrem");
		rename(".jrem-dup",".jrem");

	}else
	{
		printf("can't remove command without index\n");
	}


}
/**
 * @brief show_cmd
 * @param index
 */
void show_cmd(int index)
{
	FILE *f_stream= fopen(".jrem","r");
	if(f_stream==NULL){perror("fopen()"); exit(1);}
	int line =1;
	char str[60];
	bool empty=true;
	while(fgets(str,60,f_stream)!=NULL)
	{
		empty=false;
		if(index==0)
		{
			printf("[%d] ",line);
			puts(str);
			++line;
		}else
		{
			if(line==index)
			{
				printf("[%d] ",line);
				puts(str);
				break;
			}
			else
			{
				++line;
			}

		}
	}
	if(empty)printf("No command register\n");

	fclose(f_stream);
}
/**
 * @brief exec_cmd
 * @param fd
 * @param index
 */
void exec_cmd(int fd, int index){}
/**
 * @brief find
 * @param fd
 */
void find(int fd,char *){}
/**
 * @brief main
 * @param argc
 * @param argv
 * @return
 */
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
		{
			close(fd);
			int index = arg==NULL ? 0 : atoi(arg);
			rm_cmd(index);
		}
		break;
		case 's':
		{
			int index = arg==NULL ? 0 : atoi(arg);
			show_cmd(index);
		}
		break;
		case 'e':
		break;
		case 'i':
		break;
		case 'f':
		break;
		case 'h':
		break;
		default:
		printf("Unknow command\n");
		break;

	}

	return 0;
}
