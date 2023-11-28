/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: casomarr <casomarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 15:26:49 by kquerel           #+#    #+#             */
/*   Updated: 2023/11/28 18:36:58 by casomarr         ###   ########.fr       */
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
	// t_env	*exit_status;
	
	exec->env_execve = ft_transform_env(env);
	if (ft_exec_slash(cmd, exec, env))
		return (127);
	exec->cmd_path = split_path(env);
	cmd->content = ft_get_command(exec->cmd_path, exec->cmd_tab[0]);
	if (!cmd->content)
	{
		// free_cmd_list(cmd);
		if (!exec->cmd_tab[0])
			ft_putstr_fd("\n", STDERR_FILENO);
		else
			command_not_found(cmd, env, exec);
	}
	else
	{
		execve(cmd->content, exec->cmd_tab, exec->env_execve);
		perror("bash");
		// free_cmd_list(cmd);
	}
	add_exit_status_in_env(&env, 127);
	return (127);
}

/* Execve if a "/" is found in the cmd */
int	ft_exec_slash(t_element *cmd, t_pipe *exec, t_env *env)
{
	add_exit_status_in_env(&env, 127);
	if (ft_strchr(exec->cmd_tab[0], '/'))
	{
		execve(cmd->content, exec->cmd_tab, exec->env_execve);
		perror("bash: ");
		ft_putstr_fd(exec->cmd_tab[0], STDERR_FILENO);
		ft_putendl_fd(": No such file or directory", STDERR_FILENO);
		//free_cmd_list(cmd);
		free_cmd_arr(exec);
		return(127);
	}
	return (0);
}

/* exec continued */
// int	command_not_found(t_pipe *exec)
int	command_not_found(t_element *cmd, t_env *env, t_pipe *exec)
{
	//t_env	*exit_status;
	
	// !!!!!!!! leaks a fix
	//exit_status = find_value_with_key_env(env, "EXIT_STATUS");
	//free(exit_status->value);
	ft_putstr_fd("bash: ", STDERR_FILENO);
	ft_putstr_fd(exec->cmd_tab[0], STDERR_FILENO);
	ft_putendl_fd(": command not found", STDERR_FILENO);
		
	
	free_cmd_arr(exec);
	free_cmd_list(cmd); // moins de leaks mais plus d'erreurs

	//pourquoi pas free_cmd_list, on l'appelle dans le main
	// free(exec->cmd_tab[0]);
	add_exit_status_in_env(&env, 127);
	return (127);
}
