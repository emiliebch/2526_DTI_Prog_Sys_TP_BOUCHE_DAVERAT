#include <unistd.h>
#define BUFSIZE 1096

int main(){
    char welcome[BUFSIZE]="Bienvenue dans le shell ENSEA. \nPour quitter, tapez 'exit'.\nenseash % \n";
    write (STDOUT_FILENO,welcome,BUFSIZE); // displays the message contained in the buffer welcome

}