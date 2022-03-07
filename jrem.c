/**
 * jrem is a shell command to store
 * any other command that you don't want to forget
 * and use it at any time. The basic argument of jrem
 * is jrem add [cmd], jrem rm [index],
 * jrem show | jrem show [index], jrem exec [index],
 * jrem find [cmd]. More combination will be added with
 * time.
 * A great thanks to Maciej who help me to correct the code
 * and make it nice and functionnal.
 */
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
FILE * get_dup_file_stream();
/**
 * return an allocator of char* pointing to the home directory of the user
 * @brief file_name return the complet path of where to store the file.
 * @param file_id if the file_id ==0 simply mean the path to store a normal
 * file while if not the function will return the path to store the temporaire file
 * @return a path of the default file location.
 */
char* file_name(int file_id)
{
	char * home_dir = getenv("HOME");
	char* file_name;
	if(file_id==0)file_name= "/.jrem";
	else file_name="/.jrem-dup";
	char * home_dir_ptr = NULL; 
	if(home_dir!=NULL)
	{
		home_dir_ptr=(char*)malloc((strlen(home_dir)+strlen(file_name))+1);
		strcpy(home_dir_ptr,home_dir);
		strcat(home_dir_ptr,file_name);
	}
	return home_dir_ptr;
}

/**
 * @brief add_cmd simply store the input of the user to the config file
 * @param size number of argument given by the main
 * @param arg char** where all the user input are store
 */
void add_cmd(int size,char* arg[])
{
	char * home_dir = file_name(0);
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
 * @brief rm_cmd delete an input from the default file.
 * The algorithm creat a temporaly file where to store
 * all the line that's not mention by the user this file
 * will now be rename into the real default file while
 * the old_file will be removed.
 * @param index the line where the command will be removed
 */
void rm_cmd(int index)
{
	char temp[60];
	int line =1;
	if(index!=0)
	{
		FILE*f_stream= get_file_stream();
		FILE *f_stream_dup=get_dup_file_stream();
		char* file_name_ =file_name(0);
		char * file_name_dup = file_name(1); 
		if(f_stream==NULL)
		{
			printf("Error file not found to create the file use add command\n");
			exit(1);
		}
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
		remove(file_name_);
		rename(file_name_dup,file_name_);
		free(file_name_);
		free(file_name_dup);

	}else
	{
		printf("can't remove command without index\n");
	}


}
/**
 * @brief show_cmd print to stdout all the command in the file.
 * if the index==0 the function print all the command but if not
 * simply print the index ask by the user.
 * @param index line to be print.
 */
void show_cmd(int index)
{
	FILE*f_stream= get_file_stream();
	if(f_stream==NULL)
	{
		printf("Nothing to show file do not exist add command to show\n");
		exit(1);
	}
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
 * @brief exec_cmd This function execute the command ask by the user by given the index.
 * Warning : This function use system to execute the command, no command
 * verification is done so be careful don't put command that you can no handle
 * @param index the specifiq line that will be execute.
 * @param arg the command arg if useful.
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
					strtok(temp,"\n");
					strcat(temp,arg);
				}
				system(temp);
			}
			++line;
		}
	}
}
/**
 * @brief find in the file if a command exist in the file.
 * It print all the occurrence seen in the file
 * @param arg the input given for searching in the file.
 */
void find(char *arg)
{
	FILE *f_stream= get_file_stream();
	if(f_stream==NULL)
	{
		printf("Error find not found to create the file use add command\n");
		exit(1);
	}
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
/**
 * @brief get_file_stream return the stream of the default file
 * @return FILE*
 */
FILE * get_file_stream()
{
	char * home_dir = file_name(0);
	FILE *f_stream= fopen(home_dir,"r");
	free(home_dir);
	return f_stream;
}
/**
 * @brief get_dup_file_stream return the stream of the temporary file
 * @return FILE*
 */
FILE * get_dup_file_stream()
{
	char* home_dir=file_name(1);
	FILE *f_stream= fopen(home_dir,"w+");
	free(home_dir);
	return f_stream;
}

/**
 * @brief main Entry point of the function
 * @param argc size of argv
 * @param argv user input
 * @return 0
 */
int main(int argc, char *argv[])
{
	char* cmd = argv[1];
	char *arg=argv[2];
	char *arg_2="";
	if(argc>3)arg_2=argv[3];
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
		printf("How to use:"
			"\n==>program_name program_cmd arg"
			"\n Ex: jrem add..."
			"\n     jrem show..."
			"\n==>program_name : jrem"
			"\n==>program_cmd: add, show, rm, exec, find."
			"\n==>Each program_cmd has his specific argument."
			"\n==>add or find take string as argument"
			"\n $jrem add ls"
			"\n $jrem find ls"
			"\n==>rm or exec take integer as argument"
			"\n $jrem rm 1"
			"\n $jrem exec 2"
			"\n==>default cmd which do not necessary take argument"
			"\n $jrem show or "
			"\n $jrem show 1"
			"\n");
		break;
		default:
		printf("Unknow command\n");
		break;

	}

	return 0;
}
