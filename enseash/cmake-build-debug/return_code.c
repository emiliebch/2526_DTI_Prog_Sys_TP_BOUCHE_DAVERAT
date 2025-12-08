#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <stdio.h>

#define BUFSIZE 1096

void write_return(char *strl, char *command, int code) {
    sprintf(command,"%d",code);
    strcat(strl,command);
    strcat(strl,"]\n");
    write(STDOUT_FILENO,strl,strlen(strl));

}

int main(){
    char welcome[BUFSIZE]="Bienvenue dans le shell ENSEA. \nPour quitter, tapez 'exit'.\nenseash % \n";
    char buffer[BUFSIZE];
    char out_message[BUFSIZE];
    char end_command[BUFSIZE];
    ssize_t command_size;

    write (STDOUT_FILENO,welcome,BUFSIZE); // displays the message contained in the buffer welcome

    //reste à definir out_message et end_command

    while (1) {
        write(1,"%",strlen("%"));
        command_size=read(STDIN_FILENO,buffer,BUFSIZE);
        //read(STDIN_FILENO,buffer,BUFSIZE); // reads the entered command

        int pid,status;
        buffer[command_size-1]='\0';

        if (strcmp("exit",buffer)==0||command_size==0) { // manages the exit display
            write(STDOUT_FILENO,"Bye bye...\n$\n",strlen("Bye bye...\n$\n"));
            exit(EXIT_FAILURE);
        }

        pid=fork();
        if(pid!=0){ // The father waits the end of the command execution in the son
            wait(&status);
            if (WIFEXITED(status)) { // true if the son finished normally
                strcpy(out_message, "enseash [exit:");
                write_return(out_message, end_command, WEXITSTATUS(status)); // return the end value of the son

            } else if (WIFSIGNALED(status)) { //true if the son finished due to a signal
                strcpy(out_message, "enseash [sign:");
                write_return(out_message, end_command, WTERMSIG(status)); // return the number of the signal which cause the son's end

            }

        }
        else {
            execlp(buffer,buffer, (char*)NULL); // executes the entered command
            exit(EXIT_SUCCESS);
        }



    }
}

