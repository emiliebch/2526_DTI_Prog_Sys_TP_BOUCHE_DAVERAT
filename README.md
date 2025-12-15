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
Vous retrouverez le code complet de cette question dans le fichier *execute_command.c*.


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
Vous retrouverez le code complet de cette question dans le fichier *manage_exit.c*.

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
Vous retrouverez le code complet de cette question dans le fichier *return_code.c*.

Pour éviter les répétition, on écrit la fonction *write_return* qui écrit le code de retour.

```c
void write_return(char *strl, char *command, int code) {
    sprintf(command,"%d",code);
    strcat(strl,command);
    strcat(strl,"]\n");
    write(STDOUT_FILENO,strl,strlen(strl));

}
```

On modifie le processus de gestion du shell :
```c
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
```
On obtient bien le resultat voulu :
<img width="813" height="244" alt="image" src="https://github.com/user-attachments/assets/57f27aef-bd8a-4338-8c41-42d0dcfbee4f" />

Pour vérifier que le programme fonctionne bien lorsqu'une commande échoue, on arrête le processus fils avant la fin de l'execution de la commande. Pour cela on utilise la commadne **kill** dans un autre terminal.

```c
            printf("%d\n", getpid());
            sleep(10);
            execlp(buffer,buffer, (char*)NULL); // executes the entered command
            exit(EXIT_FAILURE);
```

Pour arrêter le processus, il faut récupérer son numéro avec la fonction **getpid()**. On ajoute un temps d'attente pour avoir le temps d'arrêter le processus avant la fin.

Commande pour kill le fils :
<img width="467" height="54" alt="image" src="https://github.com/user-attachments/assets/025e6910-3c5b-49c4-8b25-6092299b1d41" />

Résultat obtenu :
<img width="816" height="225" alt="image" src="https://github.com/user-attachments/assets/8749bd43-1f8d-4391-a8c9-3775c6cf9acd" />

## 5. Mesure du temps d’exécution de la commande en utilisant l’appel clock_gettime :

La différence entre l'instant avant l'éxecution et l'instant à la fin de l'execution d'une commande nous indique la durée d'execution. On utilise start et stop qui sont deux structures timespec.

```c
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
```

Le timer qu'on utilise est séparé en deux valeurs : le temps en ns et le temps en s.

Il faut bien penser à prendre en compte les deux et les convertir en ms :

```c
    // ns and s to ms
            time_exe_ns=stop.tv_nsec-start.tv_nsec;
            time_exe_s=stop.tv_sec-start.tv_sec;
            time_exe_ms=time_exe_s*1000+time_exe_ns*pow(10,-6);
```
Ensuite, on modifie la fonction **write_return** en conséquent :
```c
void write_return(char *strl, char *command, int code, int time_ms) {
    sprintf(command,"%d",code);
    strcat(strl,command);
    strcat(strl,"|");
    sprintf(command,"%d",time_ms);
    strcat(strl,command);
    strcat(strl,"ms]");
    write(STDOUT_FILENO,strl,strlen(strl));
}
```
On obtient donc :
<img width="819" height="242" alt="image" src="https://github.com/user-attachments/assets/54f6742a-52c9-4d07-b16b-c353e6c7d20f" />

On fait un test pour vérifier que le temps est bien pris en compte :

```c
printf("%d\n", getpid());
            sleep(5);
            execlp(buffer,buffer, (char*)NULL); // executes the entered command
            exit(EXIT_FAILURE);
```

On modifie la durée d'execution pour mettre 5s
<img width="819" height="242" alt="image" src="https://github.com/user-attachments/assets/f5f49cd7-c7b3-4371-8805-646bf1400c73" />

On mesure bien 5s. Le code fonctionne.

## 6. Exécution d’une commande complexe (avec arguments) ;
Vous retrouverez le code complet de cette question dans le fichier *complex_command.c*.

```c
char** command_separation (char *command) {
    char **argv;
    argv=malloc(20*sizeof(char*));
    int i=0;
    char *tmp=strtok(command," ");
    while (tmp!=NULL) {
        argv[i]=malloc(20*sizeof(char));
        strcpy(argv[i],tmp);
        tmp=strtok(NULL," ");
        i++;
    }
    argv[i-1][strlen(argv[i-1])-1]='\0'; // delete le "enter" charactere on the last argument
    return argv;
}
```

On crée en premier une fonction qui va servir à séparer chacune des commandes dans le shell. L'objectif est de séparer une commande comme *ls -l* dans un tableau de char donc de récupérer *ls* et *-l* indépendamment en les séparant au niveau de l'espace.
Pour cela, on utilise la fonction *strtok*.
Il ne faut pas oublier d'allouer la mémoire à notre tableau à chaque fois que l'on ajoute une valeur au tableau.
Pour la lisibilité du code, on décide de créer une fonction *execute_command* pour alleger la fonction *main* qui devient :

```c
int main(){
    char welcome[BUFSIZE]="Bienvenue dans le shell ENSEA. \nPour quitter, tapez 'exit'.\nenseash % \n";
    char buffer[BUFSIZE];
    ssize_t command_size;
    char **command_list;

    write (STDOUT_FILENO,welcome,BUFSIZE); // displays the message contained in the buffer welcome

    while (1) {
        write(STDOUT_FILENO,"%",strlen("%"));
        memset(buffer,'\0',BUFSIZE); // clear buffer
        if ((command_size=read(STDIN_FILENO,buffer,BUFSIZE))==-1) {
            perror("read");
            exit(EXIT_FAILURE);
        }
        command_list=command_separation(buffer);
        //read(STDIN_FILENO,buffer,BUFSIZE); // reads the entered command
        execute_command(command_list,command_size);
    }
}
```

On sépra puis on execute les 2 commandes.

La fonction *execute_command* contient toute la partie execution avec le fork, on adapte pour pouvoir prendre en agrument un tableau de chaîne de charactere :
```c
void execute_command (char **buffer, ssize_t command_size) {
```
On veut pouvoir executer plusieurs commande ce qui est impossible avec *execlp*. On va plutôt utiliser *execvp*.

```c
    pid=fork();
    if(pid!=0){ // The father waits the end of the command execution in the son
    ...
    }
    else {
        printf("%d\n", getpid());
        sleep(5);
        execvp(buffer[0],buffer); // executes the entered command
        exit(EXIT_FAILURE);

    }
```
Il nous reste encore une erreur que l'on a pas réussi à résoudre dans le temps imparti.
