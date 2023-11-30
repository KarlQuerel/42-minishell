/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 12:57:03 by kquerel           #+#    #+#             */
/*   Updated: 2023/11/30 13:02:51 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* Checks if they are no arg or opt type in the list */
bool	no_args_or_options(t_element *cmd)
{
	while (cmd)
	{
		if (cmd->type == ARGUMENT || cmd->type == OPTION)
			return (false);
		cmd = cmd->next;
	}
	return (true);
}

/* Checks if pipes are present in the list */
bool	no_pipes_before(t_element *cmd)
{
	while (cmd)
	{
		if (cmd->type == PIPE)
			return (false);
		cmd = cmd->prev;
	}
	return (true);
}

/* Checks everything */
void	exit_check_all(t_element *cmd, t_env **env, t_pipe *exec)
{
	if (no_args_or_options(cmd) == true)
	{
		if (no_pipes_before(cmd) == true)
			ft_putendl_fd("exit", STDERR_FILENO);
		exit_free(cmd, env, exec);
		exit(0);
	}
}
