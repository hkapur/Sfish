#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <readline/readline.h>
    #include <sys/wait.h>
    #include <fcntl.h>
    #include <time.h>
    #include <limits.h>
    #include <sys/stat.h>
    #include <signal.h>
#include "sfish.h"
#include "debug.h"


static char *pipe_array[128];
int fd;
int len;
pid_t pid2;
int pid2;
pid_t shell_pid;
char *buffer_for_pipe[128];
static char *curr_dir = NULL;
static char *prev_dir = NULL;
bool CheckForIo(char *inp);
void Redirection(char *inp);
char *deblank(char *source);
int file_exist (char *filename);
static int splitting(char *buffer_for_pipe, int input, int first, int last);
static int command_for_pipe(int input, int first, int last, char *buffer_for_pipe);
void myintHandler(int signo);
void myintHandler_z(int signo);
char *replace_str(char *str, char *orig, char *rep, int start);
void SIG_IGNN(int signo);
char job_ids[1024];
int j_id_int = 1;
char j_id_char[128];
char *jobs[1024];
char *final_id[1024];
int count_jobs = 0;
char* myitoa(int number,char* buffer,int radix);
void colorize(char *inp);


int main(int argc, char *argv[], char* envp[]) {
    char* input;
    bool exited = false;
    shell_pid = getpid();

    if(!isatty(STDIN_FILENO)) {
        if((rl_outstream = fopen("/dev/null", "w")) == NULL){
            perror("Failed trying to open DEVNULL");
            exit(EXIT_FAILURE);
        }
    }
        signal(SIGINT, myintHandler);
        signal(SIGTSTP, myintHandler_z);
        char cwd2[1024];
        getcwd(cwd2, sizeof(cwd2));
        curr_dir = cwd2;
        prev_dir = cwd2;

    do {
        char Address[1024];
        char cwdir[1024];

        if (getcwd(cwdir, sizeof(cwdir)) != NULL)
        {
        if(strstr(cwdir,getenv("HOME"))!=NULL)
        {
            int dposfound1 = 0;;
            char *pfound = strstr(cwdir, getenv("HOME")); //pointer to the first character found 's' in the string saux
            if (pfound != NULL)
            {
                dposfound1 = (pfound - cwdir); //saux is already pointing to the first string character 't'.
            }
          replace_str(cwdir,getenv("HOME"),"~",dposfound1);
          strcpy(Address, cwdir);

          printf("%s", Address);
        }
        else printf("%s", cwdir);
    }
        else
        perror("getcwd() error");



        // time_t rawtime;
        // struct tm *info;
        // time( &rawtime );
        // info = localtime( &rawtime );
        // char buffer[80];
        // strftime(buffer,80,STRFTIME_RPRMT, info);
        // printf("%s",buffer);

        input = readline(" :: hkapur >> ");

        myitoa(j_id_int,j_id_char,10);
        strcpy(job_ids,input);
        strcat(j_id_char," ");
        strcat(j_id_char,job_ids);
        final_id[j_id_int-1] = strdup(job_ids);
        j_id_int++;
                                                                                            //////////////////
        if(CheckForIo(input)==true)
        {
            Redirection(input);
            continue;
        }

        int i = 0;
        char *p = strtok (input, " ");
        char *array[128];
        int count = 0;
        while (p != NULL)
    {
        array[i++] = p;
        p = strtok (NULL, " ");
        count++;
    }
        // write(1, "\e[s", strlen("\e[s"));
        // write(1, "\e[20;10H", strlen("\e[20;10H"));
        // write(1, "SomeText", strlen("SomeText"));
        // write(1, "\e[u", strlen("\e[u"));
        if(array[0] == NULL) {
            continue;
        }
        else if(strcmp(array[0],"help")==0)
        {
            printf("This is the help section, where you will see a list of builtin functions");
        }
        else if(strcmp(array[0],"exit")==0)
        {
            exited = true;
        }
        else if(strcmp(array[0],"color")==0&&array[1]!=NULL)
        {
            colorize(array[1]);
        }
        else if(strcmp(array[0],"pwd")==0)
        {
                char cwd[1024];
            if (getcwd(cwd, sizeof(cwd)) != NULL)
                fprintf(stdout, "%s \n", cwd);
            else
               perror(EXIT_SUCCESS);
        }
        else if(strcmp(array[0],"cd")==0)
        {
            if(count==1)
            {
                char try[100];
                prev_dir = curr_dir;
                chdir(getenv("HOME"));
                getcwd(try, sizeof(try));
                curr_dir = try;
            }
            else if(count==2)
          {
            if(strcmp(array[1],".")==0)
            {

            }
            else if(strcmp(array[1],"..")==0)
            {
                char try[100];
                prev_dir = strdup(curr_dir);
                chdir("..");
                getcwd(try, sizeof(try));
                curr_dir = try;
            }
            else if(strcmp(array[1],"-")==0)
            {
                char *temp;
                chdir(prev_dir);
                temp = strdup(curr_dir);
                curr_dir = prev_dir;                /////////////
                prev_dir = strdup(temp);
            }
            else
            {
                int ret;
                ret = chdir(array[1]);
                if(ret==0)
                {
                    char try[100];
                    prev_dir = strdup(curr_dir);
                    getcwd(try, sizeof(try));
                    curr_dir = try;
                }
                else if(ret==-1)
                {
                    perror(EXEC_NOT_FOUND);
                }
            }
          }
        }                                                                   //// Part 1 is implemented till here
        else
        {
            pid_t pid = fork();

            if (pid == -1)
            {
                printf("Failed to fork");
                // error, failed to fork()
                }
                else if (pid > 0)
                {
                    int status;
                    waitpid(pid, &status, WUNTRACED);
                    if(WIFSTOPPED(status)!=0)
                    {
                        kill(pid,SIGTSTP);
                    }
                    tcsetpgrp(0,getpgid(0));
                }
                else
                {
                    array[count] = NULL;                //Make the array of count null or else seg fault will occur
                    execvp(array[0],array);
                    // we are the child
                    printf(EXEC_NOT_FOUND,array[0]);
                    exit(0);                 // exec never returns
                }
         }
         int j = 0;
         while(j<128)
         {
            array[j] = NULL;            //Clear the array to read new line from null as it doesn't overwrite
            j++;
          }
        rl_free(input);

    } while(!exited);

    debug("%s", "user entered 'exit'");

    return EXIT_SUCCESS;
}


bool CheckForIo(char *input)
{
    if(strstr(input,"<")!=NULL||strstr(input,">")!=NULL||strstr(input,"|")!=NULL)
    {
        return true;
    }
    else return false;
}
void Redirection(char *input)
{
    if(strstr(input,">")!=NULL&&strstr(input,"<")==NULL&&strstr(input,"|")==NULL)   ///////         3.1
    {
        int i = 0;
        char *tok = strtok (input, ">");
        char *arr2[128];
        arr2[1] = NULL;
        arr2[1] = NULL;
        int counter = 0;
        while (tok != NULL)
    {
        arr2[i++] = tok;
        tok = strtok (NULL, ">");
        counter++;
    }
      int k = 0;
        char *p = strtok (arr2[0], " ");
        char *array[128];
        int count = 0;
        while (p != NULL)
    {
        array[k++] = p;
        p = strtok (NULL, " ");
        count++;
    }
        {
            pid_t pid = fork();

            if (pid == -1)
            {
                printf("Failed to fork");
                // error, failed to fork()
                }
                else if (pid > 0)
                {
                   int status;
                    waitpid(pid, &status, WUNTRACED);
                    if(WIFSTOPPED(status)!=0)
                    {
                        kill(pid,SIGTSTP);
                    }
                    tcsetpgrp(0,getpgid(0));
                }
                else
                {
                    if(arr2[1]!=NULL)
                    {
                    arr2[1]=deblank(arr2[1]);
                    array[count] = NULL;                //Make the array of count null or else seg fault will occur
                    int fd = open(arr2[1], O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
                    dup2(fd, 1);   // make stdout go to file
                    close(fd);
                    }
                    else {
                        perror(EXEC_NOT_FOUND);
                        exit(EXIT_FAILURE);
                    }
                    execvp(array[0],array);
                    // we are the child
                    printf(EXEC_NOT_FOUND,array[0]);
                    exit(0);                 // exec never returns
                }
         }
         int j = 0;
         while(j<128)
         {
            array[j] = NULL;            //Clear the array to read new line from null or else doesnt overwrite
            j++;
          }
    }
    else if(strstr(input,"<")!=NULL&&strstr(input,">")==NULL&&strstr(input,"|")==NULL) ///////////////      3.2
         {
        int i = 0;
        char *tok = strtok (input, "<");
        char *arr2[128];
        arr2[1] = NULL;
        int counter = 0;
        while (tok != NULL)
    {
        arr2[i++] = tok;
        tok = strtok (NULL, "<");
        counter++;
    }
      int k = 0;
        char *p = strtok (arr2[0], " ");
        char *array[128];
        int count = 0;
        while (p != NULL)
    {
        array[k++] = p;
        p = strtok (NULL, " ");
        count++;
    }
        {
            pid_t pid = fork();

            if (pid == -1)
            {
                printf("Failed to fork");
                // error, failed to fork()
                }
                else if (pid > 0)
                {
                    int status;
                    waitpid(pid, &status, WUNTRACED);
                    if(WIFSTOPPED(status)!=0)
                    {
                        kill(pid,SIGTSTP);
                    }
                    tcsetpgrp(0,getpgid(0));
                }
                else
                {
                    if(arr2[1]!=NULL)
                    {
                    arr2[1]=deblank(arr2[1]);
                    }
                    array[count] = NULL;                //Make the array of count null or else seg fault will occur
                    if(file_exist(arr2[1]))
                    {
                    int fd = open(arr2[1],O_RDONLY);

                    dup2(fd, 0);   // make stdout go to file
                    close(fd);
                    execvp(array[0],array);
                    // we are the child
                    perror(EXEC_NOT_FOUND);
                    exit(0);                 // exec never returns
                }else {perror(EXEC_NOT_FOUND);
                    exit(EXIT_FAILURE);
                };
            }
         }
         int j = 0;
         while(j<128)
         {
            array[j] = NULL;            //Clear the array to read new line from null or else doesnt overwrite
            j++;
          }
    }
    else if(strstr(input,"<")!=NULL&&strstr(input,">")!=NULL&&strstr(input,"|")==NULL)      //////// 3.3
    {
        if(strcmp(input,"<>")!=0&&strcmp(input,"><")!=0)
        {
        int dposfound1 = 0,dposfound2 = 0;
        int dlenstr = strlen(input);
        if (dlenstr > 0)
        {
            char *pfound = strstr(input, "<"); //pointer to the first character found 's' in the string saux
            if (pfound != NULL)
            {
                dposfound1 = (pfound - input); //saux is already pointing to the first string character 't'.
            }
            char *pfound2 = strstr(input, ">"); //pointer to the first character found 's' in the string saux
            if (pfound2 != NULL)
            {
                dposfound2 = (pfound2 - input); //saux is already pointing to the first string character 't'.
            }
        }
        if(dposfound1<dposfound2)             //Here is cat < in.txt > out.txt                   //change func here
        {
        char *farray[128];
        char *strarray[128];
        char *tarray[128];
        int i = 0;
        char *tok = strtok (input, "<");                //tokenize with <
        int counter = 0;
        while (tok != NULL)
    {
        farray[i++] = tok;
        tok = strtok (NULL, "<");
        counter++;
    }
        farray[counter] = NULL;     //made null
        int k = 0;
        char *p = strtok (farray[0], " ");                  //tokenize with spaces
        int count = 0;
        while (p != NULL)
    {
        strarray[k++] = p;
        p = strtok (NULL, " ");
        count++;
    }
        strarray[count] = NULL;                 //made null
        farray[1]=deblank(farray[1]);
        int j = 0;
        char *token = strtok (farray[1], ">");                //tokenize with >
        int counter2 = 0;
        while (token != NULL)
    {
        tarray[j++] = token;
        token = strtok (NULL, ">");
        counter2++;
    }
        tarray[counter2] = NULL;            //made null
         pid_t pid = fork();

            if (pid == -1)
            {
                printf("Failed to fork");
                // error, failed to fork()
                }
                else if (pid > 0)
                {
                    int status;
                    waitpid(pid, &status, WUNTRACED);
                    if(WIFSTOPPED(status)!=0)
                    {
                        kill(pid,SIGTSTP);
                    }
                    tcsetpgrp(0,getpgid(0));
                }
                else
                {
                    if(file_exist(tarray[0]))
                    {
                    int fd = open(tarray[0],O_RDONLY);
                    dup2(fd, 0);   // make stdout go to file
                    close(fd);
                    if(tarray[1]!=NULL)
                    {
                    int fg = open(tarray[1], O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
                    dup2(fg,1);
                    close(fg);
                    }
                    else {
                        perror(SYNTAX_ERROR);
                        exit(EXIT_FAILURE);
                    };
                    execvp(strarray[0],strarray);
                    // we are the child
                    printf(EXEC_NOT_FOUND,strarray[0]);
                    exit(0);                 // exec never returns
                }else
                {printf(SYNTAX_ERROR,tarray[0]);
                    exit(EXIT_FAILURE);
                }
            }
        }                                                //Here is cat > in.txt < out,txt       /////////// quit loop here
        else if(dposfound1>dposfound2)
        {
        char *farray[128];
        char *strarray[128];
        char *tarray[128];
        int i = 0;
        char *tok = strtok (input, ">");                //tokenize with >
        int counter = 0;
        while (tok != NULL)
    {
        farray[i++] = tok;
        tok = strtok (NULL, ">");
        counter++;
    }
        farray[counter] = NULL;     //made null
        int k = 0;
        char *p = strtok (farray[0], " ");                  //tokenize with spaces
        int count = 0;
        while (p != NULL)
    {
        strarray[k++] = p;
        p = strtok (NULL, " ");
        count++;
    }
        strarray[count] = NULL;                 //made null
        farray[1]=deblank(farray[1]);
        int j = 0;
        char *token = strtok (farray[1], "<");                //tokenize with <
        int counter2 = 0;
        while (token != NULL)
    {
        tarray[j++] = token;
        token = strtok (NULL, "<");
        counter2++;
    }
        tarray[counter2] = NULL;            //made null
         pid_t pid = fork();

            if (pid == -1)
            {
                printf("Failed to fork");
                // error, failed to fork()
                }
                else if (pid > 0)
                {
                    int status;
                    waitpid(pid, &status, WUNTRACED);
                    if(WIFSTOPPED(status)!=0)
                    {
                        kill(pid,SIGTSTP);
                    }
                    tcsetpgrp(0,getpgid(0));
                }
                else
                {
                    if(file_exist(tarray[1]))
                    {
                    int fd = open(tarray[1],O_RDONLY);
                    dup2(fd, 0);   // make stdout go to file
                    close(fd);
                    int fg = open(tarray[0], O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
                    dup2(fg,1);
                    close(fg);
                    execvp(strarray[0],strarray);
                    // we are the child
                    printf(EXEC_NOT_FOUND,strarray[0]);
                    exit(0);                 // exec never returns
            }else {printf(SYNTAX_ERROR,tarray[1]);
                exit(0);}
        }
        }
    }        //// quit loop here
    else perror(SYNTAX_ERROR);

}
    else if(strstr(input,"<")==NULL&&strstr(input,">")==NULL&&strstr(input,"|")!=NULL)  /////3.4
    {
        fd = 0;
        len = 0;
        pid2 = 0;
        int status;

        len = strlen(input);
        input[len] = '\0';
        int i, n = 1, numb = 0, first = 1;

        buffer_for_pipe[0] = strtok(input,"|");

        while ((buffer_for_pipe[n] = strtok(NULL,"|"))!=NULL)
        n++;
        buffer_for_pipe[n] = NULL;
        if(buffer_for_pipe[0]!=NULL&&buffer_for_pipe[1]!=NULL)
    {
        for(i=0; i<n-1; i++)
            {
            numb = splitting(buffer_for_pipe[i], numb, first, 0);
            first = 0;
            }
        numb = splitting(buffer_for_pipe[i], numb, first, 1);
        numb = 0;
        waitpid(pid2,&status,0);
    }else perror(SYNTAX_ERROR);
}
    rl_free(input);
}
char* deblank(char* input)
{
    int i,j;
    char *out=input;
    for (i = 0, j = 0; i<strlen(input); i++,j++)
    {
        if (input[i]!=' ')
            out[j]=input[i];
        else
            j--;
    }
    out[j]=0;
    return out;
}
int file_exist (char *filename)
{
  struct stat buffer;
  return (stat (filename, &buffer) == 0);
}
static int splitting(char *buffer_for_pipe, int numb, int first, int last)
{

    char *new_buffer;
    int m = 1;
    new_buffer = strdup(buffer_for_pipe);
      {
        pipe_array[0]=strtok(buffer_for_pipe," ");
        while((pipe_array[m]=strtok(NULL," "))!=NULL)
              m++;
        pipe_array[m]=NULL;
        }
return command_for_pipe(numb, first, last, new_buffer);
}
static int command_for_pipe(int numb, int first, int last, char *buffer_for_pipe)
{

    int pipefd[2], ret;
  ret = pipe(pipefd);
  if(ret == -1)
      {
        perror("Pipe Error");
        return 1;
      }
  pid2 = fork();

  if (pid2 == 0)
  {
    if (first==1 && last==0 && numb==0)
    {
      dup2(pipefd[1], 1 );
    }
    else if (first==0 && last==0 && numb!=0)
    {
      dup2(numb, 0);
      dup2(pipefd[1], 1);
    }
    else
    {
      dup2(numb, 0);
    }
    if(execvp(pipe_array[0], pipe_array)<0)
        { printf(EXEC_NOT_FOUND, pipe_array[0]);
            exit(0);}

              else
         {
            waitpid(pid2, 0, 0);
        }
    }
  if (last == 1)
    close(pipefd[0]);
  if (numb != 0)
    close(numb);
  close(pipefd[1]);
  return pipefd[0];
}
void myintHandler(int signo)
{
    pid_t p_d = getpid();
    if(p_d != shell_pid)
    {
    kill(p_d,2);
    tcsetpgrp(STDIN_FILENO,shell_pid);
    }
    tcsetpgrp(STDIN_FILENO,shell_pid);
}
void myintHandler_z(int signo)
{
    pid_t p_d = getpid();
    if(p_d != shell_pid)
    {
        printf("abcd");
    kill(p_d,SIGTSTP);
    // jobs[count_jobs] = final_id[j_id_int-1];
    // printf("%s",jobs[count_jobs]);
    count_jobs++;
    tcsetpgrp(0,shell_pid);
    }
    tcsetpgrp(0,shell_pid);
}

char *replace_str(char *str, char *orig, char *rep, int start)
{
  static char temp[1024];
  static char buffer[1024];
  char *p;

  strcpy(temp, str + start);

  if(!(p = strstr(temp, orig)))  // Is 'orig' even in 'temp'?
    return temp;

  strncpy(buffer, temp, p-temp); // Copy characters from 'temp' start to 'orig' str
  buffer[p-temp] = '\0';

  sprintf(buffer + (p - temp), "%s%s", rep, p + strlen(orig));
  sprintf(str + start, "%s", buffer);

  return str;
}
char* myitoa(int number,char* buffer,int radix){
    int increment=1,n,num;
    char temp[32];
    radix%=37;
    for(n=0;number>0;n++){
        increment*=radix;
        num=(number%increment)/(increment/radix);
        temp[n]=num+0x30;
        if(num>9){
            temp[n]+=7;
        }
        number-=num*(increment/radix);
    }
    for(int i=0;i<n;i++){
        buffer[n-1-i]=temp[i];
    }
    buffer[n]='\0';
    return buffer;
}
void colorize(char *inp)
{
    if(strcmp(inp,"RED")==0)
    {
        char *c;
        c = "\e[1;31m";
        printf("%s",c);
    }
    else if(strcmp(inp,"GRN")==0)
    {
        char *c;
        c = "\e[1;32m";
        printf("%s",c);
    }
    else if(strcmp(inp,"YEL")==0)
    {
        char *c;
        c = "\e[1;33m";
        printf("%s",c);
    }
    else if(strcmp(inp,"BLU")==0)
    {
        char *c;
        c = "\e[1;34m";
        printf("%s",c);
    }
    else if(strcmp(inp,"MAG")==0)
    {
        char *c;
        c = "\e[1;35m";
        printf("%s",c);
    }
    else if(strcmp(inp,"CYN")==0)
    {
        char *c;
        c = "\e[1;36m";
        printf("%s",c);
    }
    else if(strcmp(inp,"WHT")==0)
    {
        char *c;
        c = "\e[1;37m";
        printf("%s",c);
    }
    else if(strcmp(inp,"BWN")==0)
    {
        char *c;
        c = "\e[0;33m";
        printf("%s",c);
    }
    else printf(SYNTAX_ERROR,inp);
}