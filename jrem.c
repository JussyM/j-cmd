#define _GNU_SOURCE
#include <stdio.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
FILE * get_file_stream();
char* file_name()
{

	char * home_dir = getenv("HOME");
	if((home_dir=getenv("HOME"))!=NULL)
	{
		char file_name[]="/.jrem";
		strcat(home_dir,file_name);
	}
	return home_dir;
}

/**
 * @brief add_cmd
 * @param fd
 * @param arg
 */
void add_cmd(int size,char* arg[])
{
	char * home_dir = file_name();
	int fd= open(home_dir,O_WRONLY|O_CREAT|O_APPEND,0666);
	if(fd<0)
	{
		printf("can not open config file\n");
		exit(1);
	}else
	{
		int i=2;
		while(i<size)
		{
			char* input = arg[i];
			int arg_size = strlen(input);
			if(size>3)
				{
					write(fd,input,arg_size);
					write(fd," ",1);
				}else
				{
					write(fd,input,arg_size);
				}
			++i;
		}
		write(fd,"\n",1);
		close(fd);

	}
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
		FILE *f_stream= get_file_stream();
		if(f_stream==NULL){printf("Error find not found to create the file use add command\n"); exit(1);}
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
	FILE*f_stream= get_file_stream();
	if(f_stream==NULL){ perror("fopen()");printf("Nothing to show file do not exist add command to show\n"); exit(1);}
	int line =1;
	char str[60];
	bool empty=true;
	while(fgets(str,60,f_stream)!=NULL)
	{
		empty=false;
		if(index==0)
		{
			printf("[%d] %s",line,str);
			++line;
		}else
		{
			if(line==index)
			{
				printf("[%d] %s",line,str);
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
void exec_cmd(int index, char* arg)
{
	char temp[60];
	int line =1;
	if(index!=0)
	{
		FILE *f_stream= get_file_stream();
		if(f_stream==NULL){printf("Error find not found to create the file use add command\n"); exit(1);}
		while(fgets(temp,60,f_stream)!=NULL)
		{
			if(line==index)
			{
				if(arg!=NULL && arg[0]!='\0')
				{
					strcat(temp," ");
					strcat(temp,arg);
				}
				system(temp);
			}
			++line;
		}
	}
}
/**
 * @brief find
 * @param fd
 */
void find(char *arg)
{
	FILE *f_stream= get_file_stream();
	if(f_stream==NULL){printf("Error find not found to create the file use add command\n"); exit(1);}
	char temp[60];
	int line=1;
	int cpt=0;
	while(fgets(temp,60,f_stream)!=NULL)
	{
		if(strstr(temp,arg)!=NULL)
		{
				printf("[%d] %s",line,temp);
				cpt++;
		} 
		++line;
	}
	fclose(f_stream);
	printf("We found %d occurrence(s).\n",cpt);
}
FILE * get_file_stream()
{
	char * home_dir = file_name();
	FILE *f_stream= fopen(home_dir,"r");
	return f_stream;
}
/**
 * @brief main
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[])
{
	char* cmd = argv[1];
	char *arg=argv[2];
	char *arg_2="";
	if(argc>3)
		arg_2=argv[3];
	int index = arg==NULL ? 0 : atoi(arg);
	switch(cmd[0])
	{
		case 'a':
		add_cmd(argc,argv);
		break;
		case 'r':
		rm_cmd(index);
		break;
		case 's':
		show_cmd(index);
		break;
		case 'e':
		exec_cmd(index,arg_2);
		break;
		case 'f':
			find(arg);
		break;
		case 'h':
		break;
		default:
		printf("Unknow command\n");
		break;

	}

	return 0;
}
