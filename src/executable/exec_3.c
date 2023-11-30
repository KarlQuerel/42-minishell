/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: casomarr <casomarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 15:26:49 by kquerel           #+#    #+#             */
/*   Updated: 2023/11/30 19:11:12 by casomarr         ###   ########.fr       */
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
	// t_env *test;
	// test = find_value_with_key_env(env, "EXIT_STATUS");
	// printf("exit status AVANT TRANSFORM ENV = %d\n", ft_atoi(test->value));

	exec->env_execve = ft_transform_env(env);

	// test = find_value_with_key_env(env, "EXIT_STATUS");
	// printf("exit status APRÈS TRANSFORM ENV = %d\n", ft_atoi(test->value));


	if (ft_exec_slash(cmd, exec, env))
		return (127);
	exec->cmd_path = split_path(env);
	
	// test = find_value_with_key_env(env, "EXIT_STATUS");
	// printf("exit status APRÈS SPLIT PATH = %d\n", ft_atoi(test->value));
	
	cmd->content = ft_get_command(exec->cmd_path, exec->cmd_tab[0]);
	if (!cmd->content)
	{
		
		// test = find_value_with_key_env(env, "EXIT_STATUS");
		// printf("exit status AVANT CMD_NOT_FOUND = %d\n", ft_atoi(test->value));
		
		// free_cmd_list(cmd);
		if (!exec->cmd_tab[0])
			ft_putstr_fd("\n", STDERR_FILENO);
		else
			return (msg_error(3, exec->cmd_tab[0])/* , free(exec->cmd_tab[0]) */, 127);
			//command_not_found(cmd, env, exec);
	}
	else
		execve(cmd->content, exec->cmd_tab, exec->env_execve);
	return (127);
}

/* Execve if a "/" is found in the cmd */
int	ft_exec_slash(t_element *cmd, t_pipe *exec, t_env *env)
{
	(void)env;
	
	if (ft_strchr(exec->cmd_tab[0], '/'))
	{
		execve(cmd->content, exec->cmd_tab, exec->env_execve);
		// printf("KARL\n");
		msg_error(2, exec->cmd_tab[0]);
		perror(" ");
		//free_child(cmd, &env, exec);
		// add_exit_status_in_env(&env, 127);
		return (127);
	}
	// add_exit_status_in_env(&env, 0);
	return (0);
}

/* exec continued */
// int	command_not_found(t_pipe *exec)
// int	command_not_found(t_element *cmd, t_env *env, t_pipe *exec)
// {
// 	// t_env	*exit_status;
// 	(void)cmd;
// 	(void)env;

// 	// !!!!!!!! leaks a fix

// 	// t_env *test;
// 	// test = find_value_with_key_env(env, "EXIT_STATUS");
// 	// printf("exit status DANS CMD_NOT_FOUND = %d\n", ft_atoi(test->value));
// 	// printf("JE SUIS LA\n");
// 	printf(" AVANT exec->cmd_tab[0] = %s\n", exec->cmd_tab[0]);
	
// 	msg_error(3, exec->cmd_tab[0]);
// 	printf(" APRES exec->cmd_tab[0] = %s\n", exec->cmd_tab[0]);
// 	return (127);
// }

void	free_child(t_element *cmd, t_env **env, t_pipe *exec)
{
	// (void)cmd;
	(void)env;
	// (void)exec;
	// if (free_cmd_list(cmd) == 1)
	// 	cmd = NULL;
	free_cmd_list(cmd);
/* 	if (free_env_list(*env) == 3)
	{
		*env = NULL;
		return (3);
	} */
	//history(FREE_HISTORY, 0);
	// 80 errors mais bcp de still reachable
	// t_env *exit_status;
	// exit_status = find_value_with_key_env(*env, "EXIT_STATUS");
	// free(exit_status->value);
	close(exec->std_in);
	close(exec->std_out);
	free (*exec->line);
	free (*exec->prompt);
	free_cmd_arr(exec);
	//un peu mois derreurs mais bcp de def lost
	free(exec);
	free_env_list(*env); //execve ne free pas env lui
}
