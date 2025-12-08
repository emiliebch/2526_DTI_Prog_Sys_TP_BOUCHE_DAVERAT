#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define BUFSIZE 1096

int main(){
    char welcome[BUFSIZE]="Bienvenue dans le shell ENSEA. \nPour quitter, tapez 'exit'.\n";
    char buffer[BUFSIZE];
    ssize_t command_size;

    write (STDOUT_FILENO,welcome,BUFSIZE); // displays the message contained in the buffer welcome


    while (1) {
        write(1,"enseash %",strlen("enseash %"));
        command_size=read(STDIN_FILENO,buffer,BUFSIZE);
        //read(STDIN_FILENO,buffer,BUFSIZE); // read the entered command

        int pid,status;
        buffer[command_size-1]='\0';
        pid=fork();
        if(pid!=0){ // The father waits the end of the command execution in the son
            wait(&status);
        }
        else {
            execlp(buffer,buffer, (char*)NULL);
            exit(EXIT_SUCCESS);
        }
    }
}