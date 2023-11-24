/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 14:46:12 by kquerel           #+#    #+#             */
/*   Updated: 2023/11/24 13:07:57 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

//heredoc avant les pipes
// extern t_global g_signals;
/*
TO DO:
- HEREDOC
- ls > caro | cat caro ----> doit s'executer en mm tmps
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

	if (!cmd)
		return ;
	i = 0;
	size_cmd = get_size_cmd(cmd);
	exec->pipe_nb = ft_count_pipes(cmd);
	exec->cmd_tab = ft_calloc(size_cmd + 1, sizeof(char *));
	if (!exec->cmd_tab)
		return ;

	// etape 1
	/*
		si type HEREDOC
		open heredoc
		>
		close
		stock le nom 
		check si heredoc 1 existe 
		
	
	*/



	//create_heredoc(heredoc, exec, fd); // quand tu trouves un type HEREDOC je create un HEREDOC et ils retournent un fd

	// ft_parse(t_element *cmd);

	if (exec->pipe_nb == 0)
	{
		// fill_array(cmd, exec);
		single_command(cmd, env, exec);
	}
	else
		multiple_commands(cmd, env, exec);
}

int	ft_is_builtin(t_element *cmd, t_env **env, t_pipe *exec)
{
	
	if (cmd && cmd->builtin == true && cmd->content)
	{
		exec->std_in = dup(STDIN_FILENO);
		exec->std_out = dup(STDOUT_FILENO);
		if (!ft_redirect(cmd, exec))
		{
			// free et on return
			ft_putstr_fd("Redirect failed\n", STDERR_FILENO); // A EFFACER
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


/* If no pipes are present
--- If a builtin is detected, ft_builtins is called, preventing
	from reaching handle_command
---	if child pid is detected, handle_command is called
*/
void	single_command(t_element *cmd, t_env **env, t_pipe *exec)
{
	int	pid;
	int	status;

	if (!ft_is_builtin(cmd, env, exec))
		return ;
	fill_array(cmd, exec);
	pid = fork();
	g_signals.location = IN_COMMAND; // set_signal_state(IN_COMMAND); si in command SIGIGNORE, sinon ce que jfais deja dans signal
	set_signals();
	if (pid < 0)
	{
		perror("fork");
		exit(127);
	}
	if (pid == 0)
		handle_command(cmd, env, exec);
/* 	if (g_signals.location == QUIT_HEREDOC)
	{
		ft_putendl_fd("HELLO", STDERR_FILENO);
		unlink(exec->hd_filename);
	} */
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
---	If we are on the last pipe we call last_pipe
*/
void	multiple_commands(t_element *cmd, t_env **env, t_pipe *exec)
{
	int	i = 0;
	int	status;

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
	// wait(NULL);
	// //wait(NULL); // ou waitpid
	// // if (waitpid(exec->last_pid, &status, 0) == -1)
	// if (waitpid(-1, &status, 0) == -1)
	// {
	// 	perror("waitpid MULT_COMMANDS.C");
	// 	return ;
	// }
	pid_t	wpid;
	wpid = 0;
	while (wpid != -1)
	{
		wpid = waitpid(-1, &status, 0);
		//si le wpid matche le pid de la derniere commande, on assigne le exit status de cette commande dans notre g_var
		if (wpid == exec->last_pid)
			g_signals.exit_status = status;
	}
	if (WIFSIGNALED(status))
		g_signals.exit_status = WTERMSIG(status) + 128;
	else if (WIFEXITED(status))
		g_signals.exit_status = WEXITSTATUS(status);
	// else
	// 	g_signals.exit_status = status;
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
	g_signals.location = IN_COMMAND; // set_signal_state(IN_COMMAND); si in command SIGIGNORE, sinon ce que jfais deja dans signal
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
	g_signals.location = IN_COMMAND; // set_signal_state(IN_COMMAND); si in command SIGIGNORE, sinon ce que jfais deja dans signal
	set_signals();
	if (pid < 0)
		perror("fork");
	if (pid == 0)
		last_dup(cmd, env, exec);
	else
	{
		//dans le cas ou ls -la | hello ---> on doit avoir exit_status = 127;
		// printf("EXIT STATUS PARENT : %d\n", g_signals.exit_status);	
		exec->last_pid = pid;
		close(exec->fd_temp);
	}
}
