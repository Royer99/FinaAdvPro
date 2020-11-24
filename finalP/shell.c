/*
    @author: Royer Donnet Arenas Camacho 
    @id: A01209400
    @date: 22/11/2020
    @description:
        shell header that contains all functions used for the mini-shell project.
*/
#include "shell.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include<sys/stat.h>


int main(int argc, char * argv[]){
    char command[MAX_STRING];
    int file;
    struct stat statbuf;
    //arguments validation 
    if(argc>2){
        printf("usage %s [batchFile]\n",argv[0]);
    }else if(argc==1){
        //interactive mode
        displayPrompt();
        //
        while(fgets(command,MAX_STRING,stdin)!=0){
            //way out
            if(strcmp(command,"quit\n")==0){
                break;
            }else{
                //printf("%s",command);
                //execute the command
                childsManager(command);
            }
            //display prompt again
            displayPrompt();
            
        }
        printf("Bye\n");

    }else
    {
        //bathfile mode
        //verify that the file exist
        if((file = open(argv[1], O_RDONLY)) < 0){ 
            printf("%s: No such file.\n",argv[0]);
            return -2;
        }
        //verify that is a regular file
        stat(argv[1],&statbuf);
        if(S_ISREG(statbuf.st_mode)){
            //runBatchFile(argv[1]);
        }else{
            printf("%s: Not a regular file.\n",argv[0]);
            return -2;
        }
        batchProcessing(file);
    }
    
    
}