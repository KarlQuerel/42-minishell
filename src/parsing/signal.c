/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: casomarr <casomarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 17:39:23 by casomarr          #+#    #+#             */
/*   Updated: 2023/11/27 16:10:08 by casomarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	set_signals(void)
{
	struct sigaction	signal;

	memset(&signal, 0, sizeof(signal));
	signal.sa_flags = SA_SIGINFO | SA_RESTART;
	signal.sa_handler = &signal_handler;
	if (sigaction(SIGINT, &signal, NULL) == -1)
		return (EXIT_FAILURE);
	if (g_location == IN_PROMPT || \
	g_location == IN_HEREDOC) //marche pas pour heredoc
		signal.sa_handler = SIG_IGN;
	if (sigaction(SIGQUIT, &signal, NULL) == -1)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

/* void	signal_handler(int signal, \
siginfo_t *info, void *ucontext) */
void	signal_handler(int signal)
{
	if (signal == SIGINT)
	{
		if (g_location == IN_PROMPT)
		{
			ft_putchar_fd('\n', STDERR_FILENO);
			rl_replace_line("", 0);
			rl_on_new_line();
			rl_redisplay();
		}
		else if (g_location == IN_COMMAND)
		{
			ft_putchar_fd('\n', STDERR_FILENO);
			rl_replace_line("", 0);
			rl_on_new_line();
		}
		else
		{
			//IN HEREDOC
			//g_signals.exit_status = 130;
			g_location = QUIT_HEREDOC;
			exit(130);
		}
		//g_signals.exit_status = 130;
	}
	else if (signal == SIGQUIT)
	{
		//g_signals.exit_status = 131;
		if (g_location == IN_COMMAND)
			ft_putendl_fd("Quit (core dumped)", STDERR_FILENO);
		rl_redisplay();
	}
}
