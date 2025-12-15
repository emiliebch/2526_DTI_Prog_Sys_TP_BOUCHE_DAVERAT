#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#define BUFSIZE 1096



void write_return(char *strl, char *command, int code, int time_ms) {
    sprintf(command,"%d",code);
    strcat(strl,command);
    strcat(strl,"|");
    sprintf(command,"%d",time_ms);
    strcat(strl,command);
    strcat(strl,"ms]");
    write(STDOUT_FILENO,strl,strlen(strl));
}


int main(){
    char welcome[BUFSIZE]="Bienvenue dans le shell ENSEA. \nPour quitter, tapez 'exit'.\nenseash % \n";
    char buffer[BUFSIZE];
    char out_message[BUFSIZE];
    char end_command[BUFSIZE];
    ssize_t command_size;
    struct timespec start, stop;
    int time_exe_s;
    int time_exe_ns;
    int time_exe_ms;

    write (STDOUT_FILENO,welcome,BUFSIZE); // displays the message contained in the buffer welcome


    while (1) {
        write(STDOUT_FILENO,"%",strlen("%"));
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
            if (clock_gettime(CLOCK_REALTIME, &start)==-1) { // Recovery of time before the execution of a command
                perror("clock_gettime");
                exit(EXIT_FAILURE);
            }
            wait(&status);

            if (clock_gettime(CLOCK_REALTIME, &stop)==1) { // end of execution time
                perror("clock_gettime");
                exit(EXIT_FAILURE);

            }

            // ns and s to ms
            time_exe_ns=stop.tv_nsec-start.tv_nsec;
            time_exe_s=stop.tv_sec-start.tv_sec;
            time_exe_ms=time_exe_s*1000+time_exe_ns*pow(10,-6);

            if (WIFEXITED(status)) { // true if the son finished normally
                strcpy(out_message, "enseash [exit:");
                write_return(out_message, end_command, WEXITSTATUS(status),time_exe_ms); // return the end value of the son

            } else if (WIFSIGNALED(status)) { //true if the son finished due to a signal
                strcpy(out_message, "enseash [sign:");
                write_return(out_message, end_command, WTERMSIG(status),time_exe_ms); // return the number of the signal which cause the son's end

            }


        }
        else {
            printf("%d\n", getpid());
            sleep(5);
            execlp(buffer,buffer, (char*)NULL); // executes the entered command
            exit(EXIT_FAILURE);

        }

    }
}

