# 2526_DTI_Prog_Sys_TP_BOUCHE_DAVERAT


## 1. Affichage d'un message d'accueil
```c
#include <unistd.h>
#define BUFSIZE 1096

int main(){
    char welcome[BUFSIZE]="Bienvenue dans le shell ENSEA. \n pour quitter, tapez 'exit'.\n enseash % \n";
    write (STDOUT_FILENO,welcome,BUFSIZE); // displays the message contained in the buffer welcome
    
}
```

<img width="811" height="113" alt="image" src="https://github.com/user-attachments/assets/58b78c82-9695-45d0-9c30-5a4410f6a007" />
