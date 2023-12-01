/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 14:46:12 by kquerel           #+#    #+#             */
/*   Updated: 2023/12/01 17:46:30 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* Handles the execution part
--- Gets size_cmd to alloc memory accordingly
--- Gets the amount of pipe to detect if we need to create childs.
--- ft_calloc takes a +1 for the NULL string at the end of cmd_tab
--- If no pipes are detected, fills cmd_tab and calls single_command function
--- Else we calls multipe_commands function
*/
void	ft_execute(t_element *cmd, t_env **env, t_pipe *exec)
{
	int	i;
	int	size_cmd;

	if (!cmd)
		return ;
	i = 0;
	size_cmd = get_size_cmd(cmd);
	exec->pipe_nb = ft_count_pipes(cmd);
	exec->cmd_tab = ft_calloc(size_cmd + 1, sizeof(char *));
	if (!exec->cmd_tab)
		return ;
	if (exec->pipe_nb == 0)
	{
		fill_array(cmd, exec);
		single_command(cmd, env, exec);
	}
	else
		multiple_commands(cmd, env, exec);
}

/* If no pipes are present
--- If a builtin is detected, ft_builtins is called, preventing
	from reaching handle_command
---	if child pid is detected, handle_command is called
*/
void	single_command(t_element *cmd, t_env **env, t_pipe *exec)
{
	int	pid;

	if (ft_all_redir(cmd) == true)
		return ;
	while (cmd)
	{
		if (cmd->type == COMMAND)
			break ;
		cmd = cmd->next;
	}
	if (!ft_is_builtin(cmd, env, exec, 0))
		return ;
	pid = fork();
	g_location = IN_COMMAND;
	set_signals();
	if (pid < 0)
		(perror("fork"), exit(127));
	if (pid == 0)
		handle_command(cmd, env, exec, 0);
	if (!ft_exit_status_single(env, pid))
		return ;
}

/* Wait for child PID and handle $? exit status */
int	ft_exit_status_single(t_env **env, int pid)
{
	int	status;

	if (waitpid(pid, &status, 0) == -1)
		return (perror("waitpid"), 0);
	if (WIFEXITED(status))
		add_exit_status_in_env(env, WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		add_exit_status_in_env(env, 128 + WTERMSIG(status));
	else
		add_exit_status_in_env(env, status);
	return (1);
}

/* Separates the pipes according to their number
--- If we are on the (1 to n -1) range, we call midde_pipes
---	If we are on the last pipe we call last_pipe
*/
void	multiple_commands(t_element *cmd, t_env **env, t_pipe *exec)
{
	int	i;

	i = 0;
	exec->fd_temp = dup(STDIN_FILENO);
	while (i <= exec->pipe_nb)
	{
		fill_array(cmd, exec);
		if (i < exec->pipe_nb)
		{
			middle_pipes(cmd, env, exec);
			while (cmd->next && cmd->type != PIPE)
				cmd = cmd->next;
			cmd = cmd->next;
		}
		else
			last_pipe(cmd, env, exec);
		i++;
	}
	if (!ft_exit_status_multiple(env, exec))
		return ;
}

/* Wait for child PID and handle $? exit status  for mult commands */
int	ft_exit_status_multiple(t_env **env, t_pipe *exec)
{
	int		status;
	pid_t	wait_pid;

	while (1)
	{
		wait_pid = wait(&status);
		if (wait_pid < 0)
			return (0);
		if (wait_pid == exec->last_pid)
		{
			if (WIFEXITED(status))
				add_exit_status_in_env(env, WEXITSTATUS(status));
			else
				add_exit_status_in_env(env, 116 + WTERMSIG(status));
		}
	}
	return (1);
}
