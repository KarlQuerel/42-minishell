/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: octonaute <octonaute@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 19:36:13 by kquerel           #+#    #+#             */
/*   Updated: 2023/11/29 14:33:07 by octonaute        ###   ########.fr       */
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

/* Reproduces the exit function
If the first argument is not valid (not numeric or > INT_MAX) the programs ends
else if the second argument is not valid, the program keeps running */
void	ft_exit(t_element *cmd, t_env **env, t_pipe *exec)
{
	t_element	*head;
	int			arg_count;

	if (no_args_or_options(cmd) == true)
	{
		if (no_pipes_before(cmd) == true)
			ft_putendl_fd("exit", STDERR_FILENO);
		exit_free(cmd, env, exec);
		exit(0);
	}
	arg_count = 0;
	head = cmd;
	cmd = cmd->next;
	while (cmd && cmd->type != PIPE)
	{
		if (cmd->type == ARGUMENT)
		{
			arg_count++;
			if (arg_count > 1)
				return (ft_exit_continued(cmd, env, exec, head, 1));
			if (!ft_is_num(cmd->content) || \
		!ft_atoi_check(cmd->content))
				ft_exit_continued(cmd, env, exec, head, 0);
		}
		cmd = cmd->next;
	}
	if (arg_count > 1)
		return (ft_exit_continued(cmd, env, exec, head, 1));
	cmd = head;
	ft_exit_continued_2(cmd, env, exec, head);
}

/* exit_continued_2 */
void	ft_exit_continued_2(t_element *cmd, t_env **env, \
t_pipe *exec, t_element *head)
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

/* exit_continued */
void	ft_exit_continued(t_element *cmd, t_env **env, t_pipe *exec, \
t_element *head, int option)
{
	if (option == 0)
	{
		if (no_pipes_before(cmd) == true)
			ft_putendl_fd("exit", STDERR_FILENO);
		ft_putstr_fd("bash: ", STDERR_FILENO);
		ft_putstr_fd(cmd->content, STDERR_FILENO);
		ft_putendl_fd(": numeric argument required", STDERR_FILENO);
		exit_free(head, env, exec);
		exit(2);
	}
	else
	{
		if (no_pipes_before(cmd) == true)
				ft_putendl_fd("exit", STDERR_FILENO);
		ft_putendl_fd("bash: exit: too many arguments", STDERR_FILENO);
		add_exit_status_in_env(env, 127);
	}
}

//FAIS DES LEAKS DANS L'EXECUTABLE
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
