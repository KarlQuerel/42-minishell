/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: casomarr <casomarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 14:46:12 by kquerel           #+#    #+#             */
/*   Updated: 2023/11/17 20:08:13 by casomarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

// extern t_global g_signals;
/*
TO DO:
- gerer open et HEREDOC, en dernier
- redirections
*/
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

	g_signals.location = IN_COMMAND;
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
	int	status;
	int	stdin_tmp;
	int	stdout_tmp;

	stdin_tmp = dup(STDIN_FILENO);
	stdout_tmp = dup(STDOUT_FILENO);
	if (!ft_redirect(cmd/*, exec*//* , NO_PRINT */))
	{
		// free et on return
		printf("ft_redirect n'a pas marche\n");
		return ;
	}
	if (cmd && cmd->builtin == true && cmd->content)
	{
		ft_builtins(cmd, env, exec, PRINT);
		dup2(stdin_tmp, STDIN_FILENO);
		dup2(stdout_tmp, STDOUT_FILENO);
		close(stdin_tmp);
		close(stdout_tmp);
		return ;
	}
	close(stdin_tmp);
	close(stdout_tmp);
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		exit(127);
	}
	if (pid == 0)
		handle_command(cmd, env, exec);
	if (waitpid(pid, &status, 0) == -1)
	{
		perror("waitpid");
		return ;
	}
	if (WIFEXITED(status))
		g_signals.exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		g_signals.exit_status = 128 + WTERMSIG(status);
	else
		g_signals.exit_status = status;
}

/* Separates the pipes according to their number
--- If we are on the (1 to n -1) range, we call midde_pipes
	--> when we are on a pipe and the pipe is
---	If we are on the last pipe we call last_pipe
(le traduire en anglais pour les commentaires)
le status s'initialise dans waitpid pour etre reutilise dans les W flags de waitpid
*/
void	multiple_commands(t_element *cmd, t_env **env, t_pipe *exec)
{
	int	i = 0;
	int	status;
	//int	fd_temp; // pour le cas du CTRL+D et heredoc, on gerera apres
	//int	fd[2]; // exec->fd[2];
	
	exec->fd_temp = ft_calloc(1, sizeof(int)); //doit free
	*(exec->fd_temp) = 0;
	while (i <= exec->pipe_nb)
	{
		fill_array(cmd, exec);
		if ( i < exec->pipe_nb)
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
	//wait(NULL); // ou waitpid
	// if (waitpid(exec->last_pid, &status, 0) == -1)
	// if (waitpid(-1, &status, 0) == -1)
	// {
	// 	perror("waitpid MULT_COMMANDS.C");
	// 	return ;
	// }
	pid_t	wpid;
	wpid = 0;
	//DEMANDER A ALBAN POURQUOI LE LAST PID NE MARCHE PAS
	while (wpid != -1)
	{
		wpid = waitpid(-1, &status, 0);
		//si le wpid matche le pid de la derniere commande, on assigne le exit status de cette commande dans notre g_var
		if (wpid == exec->last_pid)
			g_signals.exit_status = status;
		continue ;
	}
	if (WIFSIGNALED(status))
		g_signals.exit_status = WTERMSIG(status) + 128;
	else if (WIFEXITED(status))
		g_signals.exit_status = WEXITSTATUS(status);
	// si un signal interrompt execve
	else
		g_signals.exit_status = status;
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
	if (pid < 0)
		perror("Fork");
	if (pid == 0)
		middle_dup(cmd, env, exec);
	else
	{
		if (*(exec->fd_temp))
			close(*(exec->fd_temp));
		*(exec->fd_temp) = dup(exec->fd[0]);
		close(exec->fd[0]);
		close(exec->fd[1]);
		//waitpid(pid, NULL, 0); // ca change rien avec ou sans
	}
}

/* Handles last pipe behaviour */
void	last_pipe(t_element *cmd, t_env **env, t_pipe *exec)
{
	int	pid;

	pid = fork();
	if (pid < 0)
		perror("fork");
	if (pid == 0)
	{
		//exec->last_pid = getpid(); //fonction interdite a enlever
		//printf("exec->last_pid = %d\n", exec->last_pid);
		last_dup(cmd, env, exec);
	}
	else
	{
		if (*(exec->fd_temp))
			close(*(exec->fd_temp));
		close(exec->fd[0]);
		waitpid(pid, NULL, 0);
	}
	// exec->last_pid = pid;
	// printf("pid dans last pipe = %d\n", pid);
	//return (0);
	//return (pid);
}
