/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: octonaute <octonaute@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 17:39:23 by casomarr          #+#    #+#             */
/*   Updated: 2023/11/09 15:50:05 by octonaute        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"


/*
---------------------------------- DIEU ALBAN -----------------------------
si dans le prompt (vide et qu on fait ctrl C) : $? doit donner un exit 130
si dans le heredoc autre exit code

on appelle une premiere fois une fonction qui permet de recuperer l'adresse 
de ma structure avant de rentrer dans la while de mon main.
variable static dans le signal_handler pour stocker l'adresse 
de ma structure en appelant une fonction qui rend cette adresse.

Define une variable globale avec 5 autres define (int) :
0 IN_PROMPT
1 IN_COMMAND
2 IN_HEREDOC
// les 2 autres define ne sont peut etre pas obligatoires en fonction de
notre structure, a voir plus tard.

C'est ces defines auxquels ont doit pouvoir acceder a travers la structure
dans le signal_handler. Donc les mettre dans une structure deja existante
qui passe partout dans le code.
Je suppose qu'il faudra le mettre dans t_element du coup il ne faut pas 
la free en entier a chaque boucle while, il faudra free son content mais
pas toucher a la variable globale qui stock cette info.

---> voir photo aroise dans les favoris sur mon tel pour $? --> la variable
globale est de type int et recoit le code erreur a afficher (ex: la variable
globale aura la valeur 130 car a recu un sigint en dernier donc on affichera
130 si on tape $?

--> le sujet a change : on ne peut utiliser la variable globale que pour
peu de choses, c'est pour ca qu'on ne l'utilise que pour garder ce num en 
memoire, et non pour en plus gerer les signaux avec les 3 define plus haut
----------------------------------------------------------------------------
*/

//Karl
// --> gerer l'exit code dans une variable globale pour les signaux (exit)
	// man waitpid, regarder tous les WIF et wstatus

/* void	signal_handler(int signal, \
siginfo_t *info, void *ucontext) */
void	signal_handler(int signal/*, char *line*/)
{

	struct sigaction	act;
	
	if (signal == SIGINT) //ctrl + C
	{
		if (g_signals.location == IN_PROMPT)
		{
			ft_putchar_fd('\n', STDERR_FILENO);
			rl_on_new_line();
			rl_replace_line("", 0); //a réglé le pb de prompt ne s'affiche pas sur lignw suivante
			rl_redisplay();
		}
		else
		{
			ft_putchar_fd('\n', STDERR_FILENO);
			rl_reset_line_state();
		}
	}
	else if (signal == SIGQUIT) // ctrl + '\'
	{
		if (g_signals.location == IN_PROMPT) //marche parfaitement au bout de la troisième fois WTF
		{
			ft_memset(&act, 0, sizeof(act));
			act.sa_handler = SIG_IGN;
			sigaction(SIGQUIT, &act, NULL);
			rl_on_new_line();
			rl_replace_line("", 0);
		}
		if (g_signals.location == IN_COMMAND) //marche seulement la première fois
		{
			ft_putstr_fd("Quit (core dumped)\n", STDERR_FILENO);
			rl_reset_line_state();
		}
	}
}

//pour ctrl + D https://github.com/Swoorup/mysh

		/* if (feof(stdin)) // pour ctrl + D?? // ne le comprend pas
		{
			printf("CTRL + D detected\n");
			final_free(line, env_list);
			return (EXIT_SUCCESS);
		} */

void	ctrlD(char *line) //lui envoyer line ou new_line?
{
	if (fgets(line, sizeof(line), stdin) == NULL)
	{
		printf("exit\n");
		//free all
		exit(0);
	}
}






/*

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

void set_terminal_mode() {
    struct termios t;
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag &= ~ICANON;  // Disable canonical mode
    t.c_lflag &= ~ECHO;    // Disable echoing
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

void reset_terminal_mode() {
    struct termios t;
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag |= ICANON;  // Enable canonical mode
    t.c_lflag |= ECHO;    // Enable echoing
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

char get_key() {
    char key;
    read(STDIN_FILENO, &key, 1);
    return key;
}

int main() {
    set_terminal_mode();

    char key;
    while (1) {
        key = get_key();

        if (key == '\x1b') {  // Check if the key is the escape character
            char sequence[3];
            if (read(STDIN_FILENO, sequence, 3) == 3) {
                if (sequence[0] == '[' && sequence[2] == 'A') {
                    // Up arrow key is pressed
                    printf("Up arrow key pressed!\n");
                    // Your variable-changing logic goes here
                }
            }
        }

        // Add other key handling logic as needed

        if (key == 'q') {
            break;  // Exit the loop if 'q' is pressed
        }
    }

    reset_terminal_mode();
    return 0;
}
*/