/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: casomarr <casomarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 17:39:23 by casomarr          #+#    #+#             */
/*   Updated: 2023/10/24 17:40:27 by casomarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

/* void	signal_handler(int signal, \
siginfo_t *info, void *ucontext) */
void	signal_handler(int signal/*, char *line*/)
{
	if (signal == SIGINT) //ctrl + C
	{
		/*qd je fais ctrl + C alors qu'aucune commande n'est en cours
		(ex de commande en cours : "cat" tout seul) le prompt devient 
		juste un dollar : devrait juste renvoyer le prompt normal.
		Je dois tjrs taper entree apres le dollar pour retomber sur le prompt*/
		ft_putchar_fd('\n', STDERR_FILENO);
		rl_on_new_line();
		rl_replace_line("", 0); /*ecrire le prompt la dedans ne marche pas non plus,
		et en plus on ne peut pas passer de variables dans signal_handler*/
		rl_redisplay();
	}
    else if (signal == SIGQUIT) // ctrl + '\'
    {
		/*ignorer ce signal sauf pendant l'execution d'une commande ex: "cat" va 
		donner ^\Quit (core dumped)*/
		// if (!line)
		// 	return ; // l'ignorer si pas de cmd en execution (ex de cmd en execution: cat tout seul)
		// else 
		//{
			/*cette partie marche bien sauf qd plusieurs ctrl \ a la suite a pres 
			avoir stop une commande en cours.
			Je dois taper entree apres le dollar pour retomber sur le prompt*/
			ft_putstr_fd("Quit (core dumped)\n", STDERR_FILENO);
			rl_on_new_line();
			rl_replace_line("", 0);
			rl_redisplay();
		// }
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
