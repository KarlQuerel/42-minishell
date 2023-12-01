/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 17:02:19 by kquerel           #+#    #+#             */
/*   Updated: 2023/12/01 13:11:02 by kquerel          ###   ########.fr       */
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
		if (!ft_redirect(cmd, exec))
		{
			//free ?
			return (0);
		}
		ft_builtins(cmd, env, exec);
		
		dup2(exec->std_in, STDIN_FILENO);
		dup2(exec->std_out, STDOUT_FILENO);
		close(exec->std_in);
		close(exec->std_out);
		
		if (option == 1)
		{
			free_cmd_list(cmd);
			free_env_list(*env);
			free (*exec->line);
			free (*exec->prompt);
/* 			int i = 0;
			while (exec->cmd_tab[i]) //que si strdup
				free(exec->cmd_tab[i]); */
			free_cmd_arr(exec);
			free(exec);
		}
		if (option == 0)
			return (0);
		exit (0);
	}
	return (1);
}

/* Redirects command based on its input
--- if a redirection is detected, ft_redirect is called
--- if a builtin is detected, ft_builtins is called
---	if the cmd is not empty, exec_command is called
*/
// void	handle_command(t_element *cmd, t_env **env, t_pipe *exec)
void	handle_command(t_element *cmd, t_env **env, t_pipe *exec, int option)
{
	t_env	*exit_status;
	int		exit_nb;
	(void)option;

	exit_nb = 0;
	if (!ft_redirect(cmd, exec))
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
	{
		exit_nb = exec_command(cmd, *env, exec);
		add_exit_status_in_env(env, exit_nb);
	}
	// if (option == 0 && exec->cmd_tab[0] && (ft_strncmp(*exec->line, "$?", 2) != 0 || \
	// (ft_strncmp(*exec->line, "$?", 2) == 0 && ft_strlen(*exec->line) != 2)) != 0)
	if (is_exit_status_in_line(*exec->line, "$?") == false)
	{
		//printf("blabla\n");
		free(exec->cmd_tab[0]);
		exec->cmd_tab[0] = NULL;
	}
	free_child(cmd, env, exec);
	exit(exit_nb);
}
