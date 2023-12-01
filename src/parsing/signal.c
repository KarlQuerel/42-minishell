/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 17:39:23 by casomarr          #+#    #+#             */
/*   Updated: 2023/12/01 20:15:17 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	set_signals(void)
{
	struct sigaction	signal;

	memset(&signal, 0, sizeof(signal));
	signal.sa_flags = SA_SIGINFO | SA_RESTART;
	signal.sa_handler = &sigint_handler;
	if (sigaction(SIGINT, &signal, NULL) == -1)
		return (EXIT_FAILURE);
	if (g_location == IN_PROMPT || g_location == IN_HEREDOC)
		signal.sa_handler = SIG_IGN;
	else
		signal.sa_handler = &sigquit_handler;
	if (sigaction(SIGQUIT, &signal, NULL) == -1)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

/* void	signal_handler(int signal, \
siginfo_t *info, void *ucontext) */
void	sigint_handler(int signal)//, int stdin_la_revanche)
{
	(void)signal;
	if (g_location == IN_PROMPT || g_location == QUIT)
	{
		ft_putchar_fd('\n', STDERR_FILENO);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		if (g_location == IN_PROMPT)
			g_location = QUIT;
	}
	else if (g_location == IN_COMMAND)
	{
		ft_putchar_fd('\n', STDERR_FILENO);
		rl_replace_line("", 0);
		rl_on_new_line();
	}
	else if (g_location == IN_HEREDOC)
	{
		g_location = QUIT_HEREDOC;
		ft_putchar_fd('\n', STDERR_FILENO);
		close(STDIN_FILENO);
	}
}

void	sigquit_handler(int signal)
{
	(void)signal;
	if (g_location == IN_COMMAND)
		ft_putendl_fd("Quit (core dumped)", STDERR_FILENO);
	rl_redisplay();
}
