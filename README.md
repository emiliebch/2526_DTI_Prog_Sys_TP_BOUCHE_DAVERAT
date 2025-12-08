# Compte rendu 


## 1. Affichage d'un message d'accueil
Vous retrouverez le code de cette question dans le fichier *affiche_message.c*.
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
Vous retrouverez le code de cette question dans le fichier *execute_command.c*.


### a) Lecture de la commande saisie
On rajoute dans le mainla boucle while(1) dans la fonction main:
```c
    while (1) {
        read (0,buffer,BUFSIZE); // read the entered command

    }
```
On peut maintenant taper une commande dans le terminal:
<img width="836" height="156" alt="image" src="https://github.com/user-attachments/assets/06daf63e-4021-4d78-8577-13a3f6edca6a" />

### b) Exécution d’une commande simple (sans argument)
On veut executer les commandes de bases d'une console. D'après le cours, on doit utiliser ***execlp***. Le caractère "enter" est pris en compte avec le *read* donc il faut le retirer pour ne pas fausser *execlp*. Quand on ne le retire pas, la commande saisie ne s'execute pas.

La boucle while(1) devient donc :
```c
    ssize_t command_size;
    while (1) {
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
```

<img width="836" height="156" alt="image" src="https://github.com/user-attachments/assets/32d062a1-c047-4f21-907b-bd9f84081d2b" />

### c) Retour au prompt enseash % et attente de la commande suivante
On ajoute dau début de la boucle infinie la ligne suivante qui permet de retourner à la ligne et d'attendre la prochaine commande ( et on modifie le *write* de l'affichage pour que '*enseash %*' ne s'affiche qu'une seule fois) :
```c
write(1,"enseash %",strlen("enseash %"));
```

<img width="966" height="156" alt="image" src="https://github.com/user-attachments/assets/b29ee5a4-2afe-45d9-98fa-9527071dd9f9" />

## 3. Gestion de la sortie du shell avec la commande “exit” ou un <ctrl>+d
Vous retrouverez le code de cette question dans le fichier *manage_exit.c*.

On veut comparer le texte saisie à la chaîne de caractère "*exit*", on utilise donc la fonction ***strcmp*** qui renvoie 0 si les deux chaînes de caractère qu'elle prend en argument sont identiques.

Pour la sortie avec *ctrl+d*, on sait que la commande *ctrl+d* envoie le message d'End Of File. Ce qui signifie qu'il n'y a plus rien à lire et la variable *command_size* est nul. Donc il suffit de rajouter la condition "*||command_size==0*" dans le *if* pour prendre en compte le *ctrl+d*.

On ajoute le boucle de comparaison suivante juste avant la ligne "*pid=fork();*" pour éviter que d'autres processus ne se créent et s'exécutent avant la comparaison avec *exit*:
```c
        if (strcmp("exit",buffer)==0||command_size==0) {
            write(STDOUT_FILENO,"Bye bye...\n$\n",strlen("Bye bye...\n$\n"));
            exit(EXIT_FAILURE);
        }
```

<img width="966" height="179" alt="image" src="https://github.com/user-attachments/assets/e01ab8fc-94ec-4117-8ebe-b6aba691db54" />

## 4. Affichage du code de retour (ou du signal) de la commande précédente dans le prompt 



