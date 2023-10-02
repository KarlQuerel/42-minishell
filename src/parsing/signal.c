/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: casomarr <casomarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 17:39:23 by casomarr          #+#    #+#             */
/*   Updated: 2023/09/20 19:38:14 by casomarr         ###   ########.fr       */
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
/*
if (feof(stdin)) {
	exit(0);
	return 0;
}
*/

