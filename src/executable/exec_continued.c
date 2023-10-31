/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_continued.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karl <karl@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 17:02:19 by kquerel           #+#    #+#             */
/*   Updated: 2023/10/31 18:36:04 by karl             ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

/* Being on the middle pipes, both fd are being redirected */
void	middle_dup(t_element *cmd, t_env *env, t_pipe *exec, t_history *entries)
{
	if (dup2(*(exec->fd_temp), STDIN_FILENO) < 0)
	{
		perror("dup2");
		exit(0);
	}
	close(exec->fd[0]);
	if (dup2(exec->fd[1], STDOUT_FILENO) < 0)
	{
		perror("dup2");
		exit(0);
	}
	//close(*(exec->fd_temp)); a garder pour le heredoc apparemment
	close(exec->fd[1]);
	handle_command(cmd, env, exec, entries);
}

/* Being on the last pipe, only entry fd is being cloned and redirected */
void last_dup(t_element *cmd, t_env *env, t_pipe *exec, t_history *entries)
{
	if (dup2(*(exec->fd_temp), STDIN_FILENO) < 0)
		perror("dup");
	(void)exec->fd;
	close(*(exec->fd_temp));
	handle_command(cmd, env, exec, entries);
}

/* Redirects command based on its input
--- if a builtin is detected, ft_builtins is sent
---	if the cmd is not empty, exec_command is called
*/
void	handle_command(t_element *cmd, t_env *env, t_pipe *exec, t_history *entries)
{
	if (cmd->builtin == true)
	{
		ft_builtins(cmd, env, entries/* , exec */); // on doit envoyer fd ici pour les pipes, exec->fd
		return ;
	}
	// on doit peut etre free des trucs dans le child
	if (exec->cmd_tab[0][0] != '\0')
		g_signals.exit_status = exec_command(cmd, env, exec);
	exit(g_signals.exit_status);
}

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
	
	
	// gerer /usr/bin/ls
	//printf("-----cmd->content = %s\n", cmd->content);
	if (ft_strchr(cmd->content, '/'))
		execve(cmd->content, exec->cmd_tab, env->env);
	exec->cmd_path = split_path(env);
	if (!exec->cmd_path)
	{
		printf("Split_path failed\n");
		// free des trucs
		return (127);
	}
	cmd->content = ft_get_command(exec->cmd_path, exec->cmd_tab[0]);
	if (!cmd->content)
	{
		if (!exec->cmd_tab[0])
			ft_putstr_fd("\n", STDERR_FILENO);
		else
		{
			ft_putstr_fd("bash: ", STDERR_FILENO);
			ft_putstr_fd(exec->cmd_tab[0], STDERR_FILENO);
			ft_putstr_fd(": command not found\n", STDERR_FILENO);
		}
		return (127);
	}
	//ft_print_array(exec->cmd_tab); // to test what is sent to execve
	if (execve(cmd->content, exec->cmd_tab, env->env) == -1)
	{
		ft_putstr_fd("execve failed\n", STDERR_FILENO);
		//perror("bash");
		//strerror(errno); --> a utiliser, par exemple chmod 000 ./exec
	}
	return (127); //return a exit code, faire une fonction cmd not found
}

/* Joins every splitted string from the PATH= env variable with the command
	then tests its validity with access */
char	*ft_get_command(char **path, char *argument)
{
	char	*to_free;
	char	*to_return;
	int		i;

	i = 0;
	if (argument && path)
	{
		while (path[i])
		{
			to_free = ft_strjoin(path[i], "/");
			to_return = ft_strjoin(to_free, argument);
			free(to_free);
			if (access(to_return, F_OK) == 0) // rajouter des flags
				return (to_return);
			free(to_return);
			i++;
		}
	}
	return (NULL);
}
