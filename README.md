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

## 2.Exécution de la commande saisie et retour au prompt (REPL : read–eval–print loop) :
### a) Lecture de la commande saisie,
On rajoute dans le mainla boucle while(1) :
```c
    while (1) {
        read (0,buffer,BUFSIZE); // read the entered command

    }
```
On peut maintenant taper une commande dans le terminal:
<img width="836" height="156" alt="image" src="https://github.com/user-attachments/assets/06daf63e-4021-4d78-8577-13a3f6edca6a" />

### b) Exécution d’une commande simple (sans argument),
### c) Retour au prompt enseash % et attente de la commande suivante.
