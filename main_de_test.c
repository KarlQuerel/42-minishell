#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <stdbool.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
# include <signal.h>
# include <sys/wait.h>
#include <sys/uio.h>
# include <fcntl.h>

int main (int argc, char**argv)
{
    printf("%d\n", chdir(argv[1]));
    /*ca rend -1 qd j'envoie pas d'argv[1]
    sinon ca rend bien 0 (qs success)*/
    return 0;
}