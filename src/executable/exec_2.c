/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 17:02:19 by kquerel           #+#    #+#             */
/*   Updated: 2023/12/08 01:05:06 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* Restore I/O's in case of builtins */
int	ft_is_builtin(t_element *cmd, t_env **env, t_pipe *exec, int option)
{
	if (cmd && cmd->builtin == true && cmd->content)
	{
		exec->std_in = dup(STDIN_FILENO);
		exec->std_out = dup(STDOUT_FILENO);
		if (!ft_redirect(cmd))
		{
			close(exec->std_in);
			close(exec->std_out);
			return (0);
		}
		ft_builtins(cmd, env, exec);
		dup2(exec->std_in, STDIN_FILENO);
		dup2(exec->std_out, STDOUT_FILENO);
		close(exec->std_in);
		close(exec->std_out);
		if (option == 1)
			ft_saint_olivier(cmd, env, exec);
		if (option == 0)
			return (0);
		exit (0);
	}
	return (1);
}

/* Free builtins in case of pipe */
void	ft_saint_olivier(t_element *cmd, t_env **env, t_pipe *exec)
{
	free_cmd_list(cmd);
	free_env_list(*env);
	free (exec->line);
	free (exec->prompt);
	free_cmd_arr(exec);
	free(exec);
}

/* Redirects command based on its input
--- if a redirection is detected, ft_redirect is called
--- if a builtin is detected, ft_builtins is called
---	if the cmd is not empty, exec_command is called
*/
void	handle_command(t_element *cmd, t_env **env, t_pipe *exec)
{
	t_env	*exit_status;
	int		exit_nb;

	exit_nb = 0;
	if (!ft_redirect(cmd))
	{
		free_child(cmd, env, exec);
		exit(1);
	}
	exit_status = find_value_with_key_env(*env, "EXIT_STATUS");
	if (cmd->builtin == true)
	{
		ft_builtins(cmd, env, exec);
		close(exec->fd[0]);
		exit(ft_atoi(exit_status->value));
		return ;
	}
	if (exec->cmd_tab[0] != NULL)
		exit_nb = exec_command(cmd, *env, exec);
	free_child(cmd, env, exec);
	exit(exit_nb);
}
