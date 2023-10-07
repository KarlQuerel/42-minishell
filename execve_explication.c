#include <unistd.h>
#include <stdio.h>


/* 
Explications Caro:

Execve execute le programme situe dans le premier argument

Execve a besoin de 3 arguments
1er -> char * -> (le path de la commande, on peut checker avec
whereis + nom de la commande)
2eme -> char ** -> tableau de commandes
3eme -> l'environnement

*/

int   main(int ac, char **av, char **env)
{
    char *args[] = {"/usr/bin/ls", "-a", NULL};
    
    (void)av;
    if (ac == 2)
        execve(args[0], args, env);
    else
        printf("Only one argument is accepted\n");
}