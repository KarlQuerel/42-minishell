#include <unistd.h>
#include <stdio.h>


/* 
Explications Caro:

Il remplace le process existant par le programme se situant
dans le premier argument (path)

Execve a besoin de 3 arguments
1er -> char * -> (le path de la commande, on peut checker avec
whereis + nom de la commande)
2eme -> char ** -> tableau de strings + le tableau se finit par NULL pour permettre
a execve de savoir quand s'arreter
3eme -> l'environnement

*/

int   main(int ac, char **av, char **env)
{
    char *cmd = "/usr/bin/ls";
    
    char *args[] = {av[1], av[2], av[3], av[4], NULL};

    printf("Start of execve call %s:\n", cmd);
    printf("==================================================\n");

    if (execve(cmd, args, env) == -1) //ne marche que si av[1] est ls (correspond a *cmd)
        perror("Could not execute execve");

    printf("THIS SHOULD NOT BE PRINTED\n");
}