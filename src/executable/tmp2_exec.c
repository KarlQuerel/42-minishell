/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 14:46:12 by kquerel           #+#    #+#             */
/*   Updated: 2023/10/25 15:34:43 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"


void	ft_execute(t_element *cmd, t_env *env, t_pipe *exec, t_history *entries)
{
	int	i;
	i = 0;

	// if (!cmd)
	// 	return ;
	exec->pipe_nb = ft_count_pipes(cmd);
	printf("%spipe_nb = %d\n%s", YELLOW, exec->pipe_nb, RESET);
    exec->cmd_tab = ft_calloc(exec->pipe_nb, sizeof(char *));
	if (!exec->cmd_tab)
		return ;
	fill_cmd_tab(cmd, exec);
	if (exec->pipe_nb == 0)
		single_command(cmd, env, exec, entries);
	else
	{
		exec->pid = ft_calloc(exec->pipe_nb + 2, sizeof(pid_t));
		if (!exec->pid)
		{
			ft_putendl_fd("Pid has failed to malloc", STDERR_FILENO);
			exit (127); // a enlever apres
			// free
			return ;
		}
		executor(cmd, env, exec, entries);
	}
}

int	executor(t_element *cmd, t_env *env, t_pipe *exec, t_history *entries)
{
	int	end[2];
	int	fd_in;
	t_element	*head;

	head = cmd;
	fd_in = STDIN_FILENO;
	while(cmd)
	{
		//redirectons
		if (cmd->next) // rajouter un type // si on est pas a la fin
			pipe(end);
		ft_fork(cmd, env, exec, entries, end, fd_in);
		close(end[1]);
		if (cmd->prev)
			close(fd_in);
		//fd_in = check_heredoc;
		if (cmd->next)
			cmd = cmd->next;
		else
			break;
	}
	pipe_wait(exec->pid, exec->pipe_nb);
	if (cmd)
		cmd = head;
	return (0);
}

int	pipe_wait(int *pid, int amount)
{
	int	i;
	int	status;

	i = 0;
	while (i < amount)
	{
		waitpid(pid[i], &status, 0);
		i++;
	}
	waitpid(pid[i], &status, 0);
	// 	if (WIFEXITED(status))
	// 	g_global.error_num = WEXITSTATUS(status);
	return (0);
}
void	ft_fork(t_element *cmd, t_env *env, t_pipe *exec, t_history *entries, int end[2], int fd_in)
{
	static int	i = 0;
	
	exec->pid[i] = fork();
	if(exec->pid[i] < 0)
	{
		printf("%serror FORK\n%s", GREEN, RESET);
		exit(1);
	}
	if (exec->pid[i] == 0) // child -> dup2
		ft_dup(cmd, env, exec, entries, end, fd_in);
	i++;
}

void	ft_dup(t_element *cmd, t_env *env, t_pipe *exec, t_history *entries, int end[2], int fd_in)
{
	if (ft_is_a_pipe_before(cmd) && dup2(fd_in, STDIN_FILENO) < 0)
	{
		printf("dup2 prev failed\n");
		exit(1);
	}
	// if (cmd->prev && cmd->prev->type == COMMAND && dup2(fd_in, STDIN_FILENO) < 0) // a regarder si cmd->prev->type == COMMAND
	// {
	// 	printf("dup2 prev failed\n");
	// 	exit(1);
	// }
	close(end[0]);
	printf("cmd->next->content = %s\n", cmd->next->content);
	if (ft_is_a_pipe_after(cmd) && dup2(end[1], STDOUT_FILENO) < 0)
	{
		printf("dup2 next failed\n");
		exit(0) ;
	}
	// if (cmd->next && cmd->next->type == COMMAND && dup2(end[1], STDOUT_FILENO) < 0)
	// {
	// 	printf("dup2 next failed\n");
	// 	exit(0) ;
	// }
	close(end[1]);
	// if (ft_is_a_pipe_before(cmd))
	// 	close(fd_in); // pourquoi a voir
	//handle_command(cmd, env, exec, entries);
}


void	single_command(t_element *cmd, t_env *env, t_pipe *exec, t_history *entries)
{
	int	pid;
	int status; // pourquoi status undefined

	if (cmd && cmd->builtin == true)
	{
		commands(cmd, env, entries);
		return ;
	}
	pid = fork();
	if (pid < 0)
	{
		printf("error\n");
		exit(127);
	}
	if (pid == 0)
		handle_command(cmd, env, exec, entries);
	waitpid(pid, &status, 0);
	// if (WIFEXITED(status))
	// 	g_global.error_num = WEXITSTATUS(status);
}

void	handle_command(t_element *cmd, t_env *env, t_pipe *exec, t_history *entries)
{
	int	exit_code;
	
	// A VIRER (ne plus envoyer entries)
	if (cmd->builtin == true)
	{
		commands(cmd, env, entries);
		return ;
	}
	exit_code = 0;
	if (exec->cmd_tab[0][0] != '\0')
		exit_code = exec_command(cmd, env, exec);
	exit(exit_code);
}

int	exec_command(t_element *cmd, t_env *env, t_pipe *exec)
{
	exec->cmd_path = split_path(env);
	if (!exec->cmd_path)
	{
		printf("Split_path failed\n");
		// free des trucs
		return (0);
	}
	cmd->content = ft_get_command(exec->cmd_path, exec->cmd_tab[0]);
	if (!cmd->content)
	{
		if (!exec->cmd_tab[0])
			ft_putstr_fd("\n", 2);
		else
		{
			ft_putstr_fd(exec->cmd_tab[0], 2);
			ft_putstr_fd(": command not found\n", 2);
		}
	}
	ft_print_array(exec->cmd_tab);
	if (execve(cmd->content, exec->cmd_tab, env->env) == -1)
		ft_putstr_fd("execve failed\n", STDOUT_FILENO);
	return (1); //return a exit code, faire une fonction cmd not found
}

int	ft_count_pipes(t_element *cmd)
{
	int	pipe_nb;
	pipe_nb = 0;
	
	if (!cmd)
		return (0);
	while (cmd)
	{
		if (cmd->type == PIPE)
			pipe_nb++;
		cmd = cmd->next;
	}
	return (pipe_nb);
}

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
			// printf("path[%d] = %s\n", i, to_return);
			if (access(to_return, F_OK) == 0)
				return (to_return);
			free(to_return);
			i++;
		}
	}
	return (NULL);
}

/* Checks if there is a pipe before */
bool	ft_is_a_pipe_before(t_element *cmd)
{
	while (cmd)
	{
		if (cmd->type == PIPE)
			return (true);
		cmd = cmd->prev;
	}
	return (false);
}

/* Checks if there is a pipe after */
bool	ft_is_a_pipe_after(t_element *cmd)
{
	while (cmd)
	{
		if (cmd->type == PIPE)
			return (true);
		cmd = cmd->next;
	}
	return (false);
}

/* Malloc all the pipes */
bool	init_pipes(t_pipe *exec)
{
	int	i;

	i = 0;
	if (exec->cmd_nb == 1)
	{
		exec->my_pipes = NULL;
		return (true);
	}
	exec->my_pipes = ft_calloc(2, sizeof(int *));
	if (!exec->my_pipes)
	{	
		ft_putendl_fd("Malloc failed\n", STDERR_FILENO);
		// free
		return (false);
	}
	exec->my_pipes[0] = ft_calloc (2, sizeof(int)); // on malloc 2 fd pour chaque pipe
	if (!exec->my_pipes[0])
	{
		ft_putendl_fd("Malloc failed\n", STDERR_FILENO);
		// free
		return (false);
	}
	exec->my_pipes[1] = ft_calloc (2, sizeof(int)); // on malloc 2 fd pour chaque pipe
	if (!exec->my_pipes[1])
	{
		ft_putendl_fd("Malloc failed\n", STDERR_FILENO);
		// free
		return (false);
	}
	while (i < exec->cmd_nb)
	{
		//exec->my_pipes[0];
		exec->my_pipes[i][0] = 0; // fd[0]
		exec->my_pipes[i][1] = 0; // fd[1]
		i++;
	}
	return (true);
}
