/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_and_dup.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: casomarr <casomarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 17:30:50 by kquerel           #+#    #+#             */
/*   Updated: 2023/12/01 15:48:06 by casomarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* Handles all middle pipes behaviour
--- Calls middle_dup function if child process is created 
---	If fd_temp exists, we dup the reading fd and assigns it to fd_temp*/
void	middle_pipes(t_element *cmd, t_env **env, t_pipe *exec)
{
	int	pid;

	if (pipe(exec->fd) < 0)
		perror("pipe");
	pid = fork();
	g_location = IN_COMMAND;
	set_signals();
	if (pid < 0)
		perror("fork");
	if (pid == 0)
		middle_dup(cmd, env, exec);
	else
	{
		close(exec->fd[1]);
		close(exec->fd_temp);
		exec->fd_temp = exec->fd[0];
	}
}

/* Handles last pipe behaviour */
void	last_pipe(t_element *cmd, t_env **env, t_pipe *exec)
{
	int	pid;

	pid = fork();
	g_location = IN_COMMAND;
	set_signals();
	if (pid < 0)
		perror("fork");
	if (pid == 0)
		last_dup(cmd, env, exec);
	else
	{
		exec->last_pid = pid;
		close(exec->fd_temp);
	}
}

/* Being on the middle pipe(s), both fds's are sent to dup2 */
void	middle_dup(t_element *cmd, t_env **env, t_pipe *exec)
{
	if (dup2(exec->fd_temp, STDIN_FILENO) < 0)
	{
		perror("dup2");
		exit(0);
	}
	close(exec->fd_temp);
	if (dup2(exec->fd[1], STDOUT_FILENO) < 0)
	{
		perror("dup2");
		exit(0);
	}
	close(exec->fd[1]);
	if (!ft_is_builtin(cmd, env, exec, 1))
		return ;
	handle_command(cmd, env, exec, 1);
}

/* Being on the middle pipe(s), only stdin fd is sent to dup2 */
void last_dup(t_element *cmd, t_env **env, t_pipe *exec)
{
	if (dup2(exec->fd_temp, STDIN_FILENO) < 0)
		perror("dup last");
	close(exec->fd_temp);
	if (!ft_is_builtin(cmd, env, exec, 1))
		return ;
	handle_command(cmd, env, exec, 1);
}
