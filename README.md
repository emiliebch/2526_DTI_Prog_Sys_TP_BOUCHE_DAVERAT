# 2526_DTI_Prog_Sys_TP_BOUCHE_DAVERAT


## 1. Affichage d'un message d'accueil
```c
#include <unistd.h>
#define BUFSIZE 1096

int main(){
    char buf[BUFSIZE]="Bienvenue dans le shell ENSEA. \n pour quitter, tapez 'exit'.\n enseash % \n";
    write (STDOUT_FILENO,buf,BUFSIZE);

}
```

<img width="806" height="122" alt="image" src="https://github.com/user-attachments/assets/2c881c21-6696-4c64-90a2-6ca32494bd42" />
