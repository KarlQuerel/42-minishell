/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 14:46:12 by kquerel           #+#    #+#             */
/*   Updated: 2023/11/27 18:53:10 by kquerel          ###   ########.fr       */
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
		single_command(cmd, env, exec);
	else
		multiple_commands(cmd, env, exec);
}

/* Restore I/O's in case of builtins */
int	ft_is_builtin(t_element *cmd, t_env **env, t_pipe *exec)
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
		return (0);
	}
	return (1);
}

/* Checks if only redirections are present in the linked list */
bool	ft_all_redir(t_element *cmd)
{
	while (cmd)
	{
		if (cmd->type < 3 && cmd->type != PIPE)
			return (false);
		cmd = cmd->next;
	}
	return (true);
}

/* Only creates files in the case of all_redir */
bool	ft_only_create(t_element *cmd)
{
	while (cmd)
	{
		if (cmd->type == OUTFILE)
		{
			if (open(cmd->content, O_CREAT | O_RDWR | O_TRUNC, 0644) < 0)
				return (perror("bash"), true);
		}
		else if (cmd->type == OUTFILE_APPEND)
		{
			if (open(cmd->content, O_CREAT | O_RDWR | O_APPEND, 0644) < 0)
				return (perror("bash"), true);
		}
		else if (cmd->type == INFILE)
		{
			if (open (cmd->content, O_RDONLY, 0644) < 0)
			{
				ft_putstr_fd("bash: ", STDERR_FILENO);
				ft_putstr_fd(cmd->content, STDERR_FILENO);
				perror(" ");
				return (true);
			}
		}
		cmd = cmd->next;
	}
	return (true);
}

/* If no pipes are present
--- If a builtin is detected, ft_builtins is called, preventing
	from reaching handle_command
---	if child pid is detected, handle_command is called
*/
void	single_command(t_element *cmd, t_env **env, t_pipe *exec)
{
	int	pid;
	int	status;
	t_env	*exit_status;
	
	if (ft_all_redir(cmd) == true)
	{
		ft_only_create(cmd);
		//free
		//free(exec->cmd_tab);
		return ;
	}
	while(cmd)
	{
		if (cmd->type == COMMAND)
			break;
		cmd = cmd->next;
	}
	if (!ft_is_builtin(cmd, env, exec))
		return ;
	fill_array(cmd, exec);
	pid = fork();
	g_location = IN_COMMAND; // set_signal_state(IN_COMMAND); si in command SIGIGNORE, sinon ce que jfais deja dans signal
	set_signals();
	if (pid < 0)
	{
		perror("fork");
		exit(127);
	}
	if (pid == 0)
		handle_command(cmd, env, exec);
/* 	if (g_location == QUIT_HEREDOC)
	{
		ft_putendl_fd("HELLO", STDERR_FILENO);
		unlink(exec->hd_filename);
	} */
	if (waitpid(pid, &status, 0) == -1)
	{
		perror("waitpid");
		return ;
	}
	exit_status = find_value_with_key_env(*env, "EXIT_STATUS");
	//free(exit_status->value);
	if (WIFEXITED(status))
		exit_status->value = ft_itoa(WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		exit_status->value = ft_itoa(128 + WTERMSIG(status));
	else
		exit_status->value = ft_itoa(status);
}

/* Separates the pipes according to their number
--- If we are on the (1 to n -1) range, we call midde_pipes
---	If we are on the last pipe we call last_pipe
*/
void	multiple_commands(t_element *cmd, t_env **env, t_pipe *exec)
{
	int	i = 0;
	int	status;
	t_env	*exit_status;
	pid_t	wpid;

	status = 0;
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
	exit_status = find_value_with_key_env(*env, "EXIT_STATUS");
	while (true)
	{
		wpid = wait(&status);
		if (wpid < 0)
			break ;
		if (wpid == exec->last_pid)
		{
			//free(exit_status->value);
			if (WIFEXITED(status))
				exit_status->value = ft_itoa(WEXITSTATUS(status));
			else
				exit_status->value = ft_itoa(WTERMSIG(status) + 128);
		}
	}
	return ;
}

/* Handles all middle pipes behaviour
--- Calls middle_dup function if child process is created 
---	If fd_temp exists, we dup the reading fd and assigns it to fd_temp*/
void	middle_pipes(t_element *cmd, t_env **env, t_pipe *exec)
{
	int	pid;

	if (pipe(exec->fd) < 0)
		perror("Pipe");
	pid = fork();
	g_location = IN_COMMAND; // set_signal_state(IN_COMMAND); si in command SIGIGNORE, sinon ce que jfais deja dans signal
	set_signals();
	if (pid < 0)
		perror("Fork");
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
	g_location = IN_COMMAND; // set_signal_state(IN_COMMAND); si in command SIGIGNORE, sinon ce que jfais deja dans signal
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
