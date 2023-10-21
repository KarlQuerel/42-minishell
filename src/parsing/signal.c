/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: octonaute <octonaute@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 17:39:23 by casomarr          #+#    #+#             */
/*   Updated: 2023/10/21 12:32:45 by octonaute        ###   ########.fr       */
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
		/*qd je fais ctrl + C le prompt devient juste un dollar, je pense que 
		c'est ici qu'il faut lui envoye rle path à imprimer (voir printf dans le main)*/
		ft_putchar_fd('\n', STDOUT_FILENO);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
    else if (signal == SIGQUIT) // ctrl + '\'
    {
		/*
		if (!line)
			return ; // l'ignorer si pas de prompt en execution
		else
		{
			ft_putchar_fd('\n', STDERR_FILENO);
			ft_putstr_fd(printf("Quit (core dumped)\n");, STDERR_FILENO);
		}
		// l'ignorer sauf pendant l'execution ex: "cat" va donner ^\Quit (core dumped)
		//ft_putnbr_fd(signal, STDERR_FILENO);
		// ft_putchar_fd('\n', STDERR_FILENO);
		*/
		ft_putchar_fd('\n', STDERR_FILENO);
		ft_putstr_fd("Quit (core dumped)\n", STDERR_FILENO);
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
	

