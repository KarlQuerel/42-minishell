/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 15:26:49 by kquerel           #+#    #+#             */
/*   Updated: 2023/12/02 17:59:00 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* Executes the command
---	split_path returns the "PATH=" variable from env, split the string by :
	and assigns it to cmd_path.
---	ft_get_command tests all paths variable from cmd_path, joins it with
	the command cmd_tab[i] and tests its validity with access.
---	Execve replaces the existing process by the program within his first argument
	1st arg: cmd->content, pointer to a string that specifies the path
		to the exec file, can be binary or script file.
	2nd arg: array of strings (char **) representing the command-line of arguments
		of the new program.
	3st arg: array of strings (char **) representing the environment variables
		for the new program.
--- Waitpid waits for the process to end before continuing.
 */
int	exec_command(t_element *cmd, t_env *env, t_pipe *exec)
{
	char *path;

	exec->env_execve = ft_transform_env(env);
	if (ft_exec_slash(cmd, exec, env))
		return (127);
	exec->cmd_path = split_path(env);
	path = ft_get_command(exec->cmd_path, exec->cmd_tab[0]);
	if (!path)
	{
		if (!exec->cmd_tab[0])
			ft_putstr_fd("\n", STDERR_FILENO);
		else
			return (msg_error_bash(1, exec->cmd_tab[0]), 127);
	}
	else
	{
		t_env	*exit;
		exit = NULL;
		if (is_key_in_env(env, "EXIT_STATUS") == true)
			exit = find_value_with_key_env(env, "EXIT_STATUS");
		free(exit->value);
		execve(path, exec->cmd_tab, exec->env_execve);
	}
	return (127);
}

/* Execve if a "/" is found in the cmd */
int	ft_exec_slash(t_element *cmd, t_pipe *exec, t_env *env)
{
	(void)env;
	char *path;

	path = cmd->content;
	if (ft_strchr(exec->cmd_tab[0], '/'))
	{
		t_env	*exit;
		exit = NULL;
		if (is_key_in_env(env, "EXIT_STATUS") == true)
			exit = find_value_with_key_env(env, "EXIT_STATUS");
		free(exit->value);
		execve(path, exec->cmd_tab, exec->env_execve);
		msg_error_bash(0, exec->cmd_tab[0]);
		perror(" ");
		//free_child(cmd, &env, exec);
		// add_exit_status_in_env(&env, 127);
		return (127);
	}
	// add_exit_status_in_env(&env, 0);
	return (0);
}

void	free_child(t_element *cmd, t_env **env, t_pipe *exec)
{
	//(void)cmd;
	//(void)env;
	// (void)exec;
	//if (is_in_line(*exec->line, "$?") == false)
	if (free_cmd_list(cmd) == 1)
		cmd = NULL;
	if (exec->std_in > 2)
		close(exec->std_in);
	if (exec->std_out > 2)
		close(exec->std_out);
	free (*exec->line);
	free (*exec->prompt);
	free_cmd_arr(exec);
	free(exec);
	if (free_env_list(*env) == 1)
		env = NULL;
}
