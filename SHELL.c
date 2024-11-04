/******************************************************************************

  @file         SHELL.c

  @author       Anuj Bagewadi

  @date         Friday,  4 September 2024

  @brief        Sheldor (Sheldor The Conqueror)

*******************************************************************************/
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//Funtion Declaration for builtin shell commands:

int sheldor_cd(char **args);
int sheldor_help(char **args);
int sheldor_exit(char **args);

//List of builtin commands, followed by their corresponding functions.

char *builtin_str[]={
  "cd",
  "help",
  "exit"
};

int (*builtin_func[])(char **)={
  &sheldor_cd,
  &sheldor_help,
  &sheldor_exit
};

int sheldor_num_builtins() {
  return sizeof(builtin_str) / sizeof(char *);
}

int sheldor_cd(char **args){
  if(args[1]==NULL){
    fprintf(stderr, "sheldor: expected argument to \"cd\"\n");
  }else{
    if(chdir(args[1]) !=0){
      perror("sheldor");
    }
  }
  return 1;
}

int sheldor_help(char **args){
  int i;
  printf("Anuj Bagewadi's SHELDOR\n");
  printf("Type program neams and arguments, and hit enter.\n");
  printf("The following are built in:\n");

  for(i=0;i<sheldor_num_builtins();i++){
    printf(" %s\n",builtin_str[i]);
  }

  printf("Use the man command for information of other programs.\n");
  return 1;
}

int sheldor_exit(char **args){
  return 0;
}

int sheldor_launch(char **args){
  pid_t pid, wpid;
  int status;

  pid =fork();
  if(pid==0){
    if(execvp(args[0],args)== -1){
      perror("sheldor");
    }
    exit(EXIT_FAILURE);
  }else if(pid<0){
    perror("sheldor");
  }else{
    do{
      wpid=waitpid(pid,&status,WUNTRACED);
    }while(!WIFEXITED(status)&&!WIFSIGNALED(status));
  }
  return 1;
}

int sheldor_execute(char **args){
  int i;
  if(args[0]==NULL){
    //An empty command was entered.
    return 1;
  }

  for(i=0;i<sheldor_num_builtins();i++){
    if(strcmp(args[0],builtin_str[i])==0){
      return (*builtin_func[i])(args);
    }
  }
  return sheldor_launch(args);

}

#define SHELDOR_RL_BUFSIZE 1024

char *sheldor_read_line(void)
{
    int bufsize = SHELDOR_RL_BUFSIZE;
    int postion = 0;
    char *buffer = malloc(sizeof(char)*bufsize);
    int c;

    if(!buffer){
      fprintf(stderr, "sheldor: allocation error\n");
      exit(EXIT_FAILURE);
    }

    while(1){
      //Read character
      c = getchar();

      //If we hit EOF, replace it with a null character and return.
      if(c==EOF || c=='\n'){
        buffer[postion]='\0';
        return buffer;
      }else{
        buffer[postion]=c;
      }
      postion++;

      //If we have exceeded the buffer, reallocate.
      if(postion >= bufsize){
        bufsize += SHELDOR_RL_BUFSIZE;
        buffer = realloc(buffer,bufsize);
        if(!buffer){
          fprintf(stderr, "sheldor: allocation error.\n");
          exit(EXIT_FAILURE);
        }
      }
    }
}

#define SHELDOR_TOK_BUFSIZE 64
#define SHELDOR_TOK_DELIM " \t\r\n\a"

char **sheldor_split_line(char *line){
    int bufsize =SHELDOR_TOK_BUFSIZE,position=0;
    char **tokens =malloc(bufsize * sizeof(char*));
    char *token;

    if(!tokens){
      fprintf(stderr,"sheldor: allocation error\n");
      exit(EXIT_FAILURE);
    }

    token = strtok(line, SHELDOR_TOK_DELIM);
    while(token!=NULL){
      tokens[position]=token;
      position++;

      if(position>=bufsize){
        bufsize+=SHELDOR_TOK_BUFSIZE;
        tokens=realloc(tokens,bufsize*sizeof(char*));
        if(!tokens){
          fprintf(stderr,"sheldor: allocation error\n");
          exit(EXIT_FAILURE);
        }
      }
      token=strtok(NULL,SHELDOR_TOK_DELIM);
    }
    tokens[position]=NULL;
    return tokens;
}

void sheldor_loop(void){
    char *line;
    char **args;
    int status;

    do{
      printf("> ");
      line = sheldor_read_line();
      args = sheldor_split_line(line);
      status = sheldor_execute(args);

      free(line);
      free(args);
    }while(status);
}

int main(int argc, char **argv){
    //Run command loop.
    sheldor_loop();
    //Perform any shutdown/cleanup.
    return EXIT_SUCCESS;
}
