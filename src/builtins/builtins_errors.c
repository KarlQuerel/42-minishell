/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_errors.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 15:39:41 by kquerel           #+#    #+#             */
/*   Updated: 2023/12/02 18:17:02 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	no_option(t_element *cmd, t_element *head)
{
	while (cmd && cmd->type != PIPE)
	{
		if (cmd->type == OPTION)
		{
			ft_putstr_fd(head->content, STDERR_FILENO);
			ft_putendl_fd(" cannot take options", STDERR_FILENO);
			return (false);
		}
		cmd = cmd->next;
	}
	return (true);
}

bool	cd_option(t_element *cmd, t_element *head)
{
	int	arg_count;

	arg_count = 0;
	while (cmd && cmd->type != PIPE)
	{
		if (cmd->type == OPTION)
		{
			ft_putstr_fd(head->content, STDERR_FILENO);
			ft_putendl_fd(": cannot take options", STDERR_FILENO);
			return (false);
		}
		if (cmd->type == ARGUMENT)
			arg_count++;
		cmd = cmd->next;
	}
	if (arg_count > 1)
	{
		ft_putstr_fd(head->content, STDERR_FILENO);
		ft_putendl_fd(": too many arguments", STDERR_FILENO);
		return (false);
	}
	return (true);
}

bool	env_option(t_element *cmd)
{
	if (cmd->next && cmd->next->type < 3)
	{
		ft_putendl_fd("env cannot take arguments nor options", \
		STDERR_FILENO);
		return (false);
	}
	return (true);
}

bool	echo_option(t_element *cmd)
{
	int	args_count;

	args_count = 0;
	while (cmd && cmd->type != PIPE)
	{
		if (cmd->type == ARGUMENT)
			args_count++;
		if (cmd->type == OPTION && compare(cmd->content, "-n") == false)
		{	
			ft_putendl_fd("echo only accepts option -n", STDERR_FILENO);
			return (false);
		}
		if (cmd->type == OPTION && (args_count == 0 && \
		no_further_args(cmd) == true))
		{
			ft_putstr_fd("", STDOUT_FILENO);
			return (false);
		}
		cmd = cmd->next;
	}
	return (true);
}

/* Checks if builtins take an option or an argument (for env)
The variable "cmd" sent to this function is the node following
the builtin command.
--- if option 0, checks if env has a next node
--- if option 1, checks if the built has a next node AND its an option
*/
bool	check_next(t_element *cmd, int option)
{
	t_element	*head;

	head = cmd;
	if (option == NONE)
		return (no_option(cmd, head));
	else if (option == CD)
		return (cd_option(cmd, head));
	else if (option == ENV)
		return (env_option(cmd));
	else if (option == HISTORY)
		return (history_option(cmd));
	else
		return (echo_option(cmd));
}
