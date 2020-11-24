/*
    @author: Royer Donnet Arenas Camacho 
    @id: A01209400
    @date: 22/11/2020
    @description:
        shell header that contains all functions used for the mini-shell project.
*/

#include<stdio.h>
#include<stdlib.h>
#include <ctype.h>
#include<string.h>
#include<unistd.h>
#include <dirent.h>
#include<sys/wait.h>
#include<sys/fcntl.h>
#include <sys/types.h>
#include<sys/stat.h>
#include <pwd.h>
#include <time.h>
#include <grp.h>
typedef unsigned char uchar;
typedef unsigned long ulong;
#define MAX_STRING 512

/*
    cleanSpaces
    @param: command is a raw string that most likely includes exta spaces
    @Description: void function remove extra spaces form raw strings
*/
void cleanSpaces(char * command){
    int i, x;
    for(i=x=0; command[i]; ++i){
        if(!isspace(command[i]) || (i > 0 && !isspace(command[i-1]))){
            command[x++] = command[i];
        }
    }
    command[x] = '\0';
}
/*
    displayPrompt
    @Description:void function that displays the shell prompt using the following format
    "mini-shell>"
*/
void displayPrompt(){
    printf("mini-shell> ");
}
/*
    runCommand
    @param: command is a raw string to be proccess and execute.
    @Description:this call cleanSpaces to get rid of all extra spaces, then tokenize the string to get the command and corresponding flags.
                 finally calls the execvp.
*/
int runCommand(char * command){
    char *args[100], *tokenizeCommand;
    int contador = 0;
    int i;
    //clean extra spaces
    //cleanSpaces(command);
    //printf("%s",command);
    tokenizeCommand = strtok(command," ");
    while(tokenizeCommand != NULL){
        args[contador] = tokenizeCommand;
        contador++;
        tokenizeCommand = strtok(NULL, " ");
    }
    printf("contador %i\n",contador);
    //remove the last "\n" to avoid problems at the execvp
    args[contador-1]=strtok(args[contador-1],"\n");
    /*
    for(i=0;i<contador;i++){
        printf("%s",args[i]);
    }
    printf("este es el comando a ejecutar:%s",args[contador]);
    */
    //Ejecucion de comandos 
    printf("%s\n\n",args[0]);
    printf("%s\n\n",args[1]);
    if (execvp(args[0], args) == -1) {
        printf("id: %i, error\n",getpid());
        printf("Not a unix command, please type a valid command\n");
        return -1;
    }
    
}

void childsManager(char *command){
    char *tokenizeCommand;
    int pid=0;
    int counter=0;
    cleanSpaces(command);
    //printf("despues de la limpia %s\n",command);
    tokenizeCommand = strtok(command,";");
    while(tokenizeCommand!= NULL){
        if((pid = fork()) == 0){
            printf("hijo id: %i, el commando es %s\n",getpid(),tokenizeCommand);
            runCommand(tokenizeCommand);
            /*
            printf("id: %i, el commando termino con exito %s\n",getpid(),tokenizeCommand);
            
            */
            exit(0);
        }else{
            printf("padre id: %i\n",getpid());
            counter++;
            tokenizeCommand = strtok(NULL, ";");
        }
    }
     while(counter-- > 0){
        wait(NULL);
    }
}

void batchProcessing(int file){
    char * buffer,*args[100];
    char *commands;
    ulong file_size;
    //retrive file size
    file_size = lseek(file, 0, SEEK_END);
    //rewind to the begining of the file
    lseek(file, 0, SEEK_SET);
    //memory allocation
    buffer=(char*)malloc(sizeof(char)*file_size);
    read(file,buffer,sizeof(char)*file_size);
    if(strlen(buffer)>MAX_STRING){
        printf("Can't process the batch file, because it's too long\n");
        exit(0);
    }
    //printf("%s\n",buffer);
    close(file);
    //tokenize with '\n' deliminter
    commands=strtok(buffer,"\n");
    int contador = 0;
    //printf("antes del tokenizado\n");
    while(commands != NULL){
        args[contador] = commands;
        //args[contador]=strtok(args[contador],"\n");
        //printf("%s,",args[contador]);
        //
        //childsManager(args[contador]);
        contador++;
        commands = strtok(NULL, "\n");
    }
    childsManager(args[2]);
    free(buffer);
}