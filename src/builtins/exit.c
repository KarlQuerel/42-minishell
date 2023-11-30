/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 19:36:13 by kquerel           #+#    #+#             */
/*   Updated: 2023/11/30 14:37:07 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* Reproduces the exit function
If the first argument is not valid (not numeric or > INT_MAX) the programs ends
else if the second argument is not valid, the program keeps running */
void	ft_exit(t_element *cmd, t_env **env, t_pipe *exec)
{
	t_element	*head;
	int			arg_count;

	exit_check_all(cmd, env, exec);
	arg_count = 0;
	head = cmd;
	cmd = cmd->next;
	while (cmd && cmd->type != PIPE)
	{
		if (cmd->type == ARGUMENT)
		{
			arg_count++;
			if (arg_count > 1)
				return (ft_too_many_arg(cmd, env));
			if (!ft_is_num(cmd->content) || \
		!ft_atoi_check(cmd->content))
				ft_num_arg(cmd, env, exec, head);
		}
		cmd = cmd->next;
	}
	if (arg_count > 1)
		return (ft_too_many_arg(cmd, env));
	cmd = head;
	exit_cont(cmd, env, exec, head);
}

/* Numeric argument required */
void	ft_num_arg(t_element *cmd, t_env **env, t_pipe *exec, t_element *head)
{
	if (no_pipes_before(cmd) == true)
		ft_putendl_fd("exit", STDERR_FILENO);
	ft_putstr_fd("bash: ", STDERR_FILENO);
	ft_putstr_fd(cmd->content, STDERR_FILENO);
	ft_putendl_fd(": numeric argument required", STDERR_FILENO);
	exit_free(head, env, exec);
	exit(2);
}

/* Too many arguments */
void	ft_too_many_arg(t_element *cmd, t_env **env)
{
	if (no_pipes_before(cmd) == true)
		ft_putendl_fd("exit", STDERR_FILENO);
	ft_putendl_fd("bash: exit: too many arguments", STDERR_FILENO);
	add_exit_status_in_env(env, 127);
}

/* exit_continued */
void	exit_cont(t_element *cmd, t_env **env, t_pipe *exec, t_element *head)
{
	int	exit_code;

	exit_code = 0;
	while (cmd && cmd->type != PIPE)
	{
		if (cmd->type == ARGUMENT)
		{
			exit_code = ft_atoi(cmd->content);
			if (no_pipes_before(cmd) == true)
				ft_putendl_fd("exit", STDERR_FILENO);
			exit_free(head, env, exec);
			exit(exit_code % 256);
		}
		cmd = cmd->next;
	}
}

/* Adds the EXIT_STATUS key in the env */
int	add_exit_status_in_env(t_env **env, int n)
{
	static t_env	*node;

	if (is_key_in_env(*env, "EXIT_STATUS") == false)
	{
		node = *env;
		while (node->next)
			node = node->next;
		node->next = ft_calloc(1, sizeof(t_env));
		node->next->key = "EXIT_STATUS";
		node->next->value = ft_itoa(n);
		node->next->prev = node;
		node->next->next = NULL;
	}
	else
	{
		node = find_value_with_key_env(*env, "EXIT_STATUS");
		free(node->value);
		node->value = ft_itoa(n);
	}
	return (ft_atoi(node->value));
}
