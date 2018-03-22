#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#define clear() printf("\033[H\033[J")
#define CMD_SIZE 20
#define ARG_SIZE 5


typedef struct cmd {
	int argno;
	char *argz[ARG_SIZE];
	char *cmd;
} CMD;
//typedef cmd CMD;

//CMD *comm;

char *arg[30];
char cwd[1024];
int flag = 0;
int envflag = 0;
int pos = 0;
char *environ[100];

int tokenize()
{
	//arg = malloc(sizeof(char) * 20);
	char temp[CMD_SIZE];
	int i = 0;
	char c[CMD_SIZE];
	fgets(c,CMD_SIZE,stdin);
	if(c[0] == '\n')
		return 0;
	c[strlen(c) - 1] =  '\0';
	//printf("%d\n",strlen(c));
	char *str = strtok(c," ");
	arg[i++] = str;
	//printf("%s",arg[0]);
	if(strcmp(arg[0],"quit") == 0)
		exit(1);
	while((str = strtok(NULL," ")) != NULL)
	{
		printf("str: %s\n",str);
		if(str[0] == '$' && strlen(str) > 1)
		{
			printf("foun\n");
			envflag = 1;
			char k[10];
			strcpy(k,str);
			arg[i] = getenv(strtok(k,"$"));
		}
		else
			arg[i] = str;
		i++;
		
	}
	arg[i++] = NULL;
	//printf("%s\n",arg[4]);
	if(strcmp(arg[0],"cd") == 0)
	{
		if(arg[1] == NULL)
		{
			if(chdir(getenv("HOME")) == 0)
				return 0;
		}
		else if(chdir(arg[1]) == 0)
			return 0;
	}
	else
		return 1;
}		
	
char *getcurrdir()
{
	//size_t size;
	if(getcwd(cwd,sizeof(cwd)) == NULL)
	{
		printf("Failed to initialize shell\n");
		exit(0);
	}
}		

void change_dir()
{
	if(chdir(arg[1]) < 0)
		printf("Could not exec command\n");
} 

			
void execute()
{
	pid_t id1,id2;
	int status;
	if((id1 = fork()) == -1)
		perror("Fork error\n");
	else if(id1 == 0)
	{
		if(execvp(arg[0],arg) == -1)
			printf("Could not execute command %s\n",arg[0]);
		exit(EXIT_FAILURE);
	}
	else
	{
		do
		{
			id2 = waitpid(id1,&status,WUNTRACED);
		}while(!WIFEXITED(status) && !WIFSIGNALED(status));
	}
}
				
void start_shell()
{
	clear();
	printf("Welcome to The Melody shell\n");
	do
	{
		printf("%s$",getcurrdir());
		flag = tokenize();
		if(flag == 0)
			continue;
		else
			execute();
	}while(1);
}



int main(int argc, char *argv[])
{
	start_shell();	
	return 0;
}




