/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: casomarr <casomarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 17:39:23 by casomarr          #+#    #+#             */
/*   Updated: 2023/11/21 18:10:23 by casomarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

int	set_signals(void)
{
	struct sigaction	signal;

	memset(&signal, 0, sizeof(signal));
	signal.sa_flags = SA_SIGINFO | SA_RESTART;
	signal.sa_handler = &signal_handler;
	if (sigaction(SIGINT, &signal, NULL) == -1)
		return (EXIT_FAILURE);
	if (g_signals.location == IN_PROMPT)
	{
		signal.sa_handler = SIG_IGN;
		if (sigaction(SIGQUIT, &signal, NULL) == -1)
			return (EXIT_FAILURE);
	}
	else
	{
		if (sigaction(SIGQUIT, &signal, NULL) == -1)
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

/* void	signal_handler(int signal, \
siginfo_t *info, void *ucontext) */
void	signal_handler(int signal)
{
	if (signal == SIGINT)
	{
		if (g_signals.location == IN_PROMPT)
		{
			ft_putchar_fd('\n', STDERR_FILENO);
			rl_on_new_line();
			rl_redisplay();
		}
		else
		{
			ft_putchar_fd('\n', STDERR_FILENO);
			rl_reset_line_state();
		}
	}
	else if (signal == SIGQUIT)
	{
		if (g_signals.location == IN_COMMAND)
			ft_putendl_fd("Quit (core dumped)", STDERR_FILENO);
		rl_redisplay();
	}
}
