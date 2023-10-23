/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 14:46:12 by kquerel           #+#    #+#             */
/*   Updated: 2023/10/23 20:40:42 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

/*
TO DO:

- voir avec Alban, cat Makefile
- UNSET (doubler l'env)
- EXPORT (doubler l'env)
- gerer open et HEREDOC

Structure pour les pipes:
- pipe(pipe_end), returns -1 upon failure
- assigns fork return value to pid_t pid_child1;
	-> fork returns -1 upon failure
- in first child
	-> open assigned to fd_infile (av[1], )
	-> dup2 (fd_infile)
	-> dup2 (pipe_end[1])
- waitpid(pid_child1, NULL, 0)
- waitpid(pid_child2, NULL, 0)
*/

/* Executes the command */
void	execute_command(t_element *cmd, t_env *env, t_pipe *exec, int i)
{
	//printf("--> i est egal a = %d\n", i);
	if (cmd->builtin == true)
	{
		commands(cmd, env);
		return ;
	}
	printf("--> exec->cmd_tab[%d] = %s\n",i , exec->cmd_tab[i]);
	int	pid;
	if (cmd->content[i] == '\0')
		return ;
	pid = fork();
	if (pid < 0)
	{
		perror("fork:");
		//free
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
	{
		exec->cmd_path = split_path(env);
		if (!exec->cmd_path)
		{
			printf("Split_path failed\n");
			// free des trucs
		}
		cmd->content = ft_get_command(exec->cmd_path, exec->cmd_tab[i]);
		if (!cmd->content)
		{
			if (!exec->cmd_tab[i])
				ft_putstr_fd("\n", 2);
			else
			{
				ft_putstr_fd(exec->cmd_tab[i], 2);
				ft_putstr_fd(": command not found\n", 2);
			}
		}
		//printf("--> cmd->content = %s\n", cmd->content);

		if (execve(cmd->content, exec->cmd_tab, env->env) == -1)
			ft_putstr_fd("execve failed\n", STDOUT_FILENO);
	}
	waitpid(pid, NULL, 0);
}

/* Extracts command from char *argument and verify if they are valid
using access*/
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

/* Handles execution */
void	ft_execute(t_element *cmd, t_env *env, t_pipe *exec)
{
	int	i;
	
	i = 0;
	exec->av_nb = get_args_nb(cmd);
	exec->cmd_tab = malloc(sizeof(char *) * (exec->av_nb + 1)); //peut etre calloc
	if (!exec->cmd_tab)
		return ;
	fill_cmd_tab(cmd, exec);
	get_cmds_nb(cmd, exec);
	if (exec->cmd_nb == 1) // dans le cas d'une single command
		execute_command(cmd, env, exec, 0);
	else // plusieurs commandes
	{
		if (!ft_give_me_my_pipes(exec))
				return ;
		exec->pid = ft_calloc(exec->cmd_nb - 1, sizeof(pid_t)); // malloc pid et malloc pid[i]
		if (!exec->pid)
		{
			ft_putendl_fd("Pid has failed to malloc", STDERR_FILENO);
			// free
			return ;
		}
		while (i < exec->cmd_nb)
		{
			ft_redir(cmd, env, exec, i);
			i++;
		}
		ft_waitpid(exec->pid, i);
	}
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
bool	ft_give_me_my_pipes(t_pipe *exec)
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
		exec->my_pipes[i][0] = 0; // fd[0]
		exec->my_pipes[i][1] = 0; // fd[1]
		i++;
	}
	return (true);
}


/* Malloc all the pipes */
// bool	ft_give_me_my_pipes(t_pipe *exec)
// {
// 	int	i;

// 	i = 0;
// 	if (exec->cmd_nb == 1)
// 	{
// 		exec->my_pipes = NULL;
// 		return (true);
// 	}
// 	printf("exec->cmd_nb = %d\n", exec->cmd_nb);
// 	exec->my_pipes = malloc(sizeof(int) * 2 * (exec->cmd_nb - 1));
// 	if (!exec->my_pipes)
// 		return (false);
// 	exec->my_pipes[i] = malloc (sizeof(int) * (exec->cmd_nb - 1));
// 	if (!exec->my_pipes)
// 		return (false);
// 	while (i < exec->cmd_nb - 1)
// 	{
// 		exec->my_pipes[i] = 0;
// 		// exec->my_pipes[i][0] = 0;
// 		// exec->my_pipes[i][1] = 0;
// 		i++;
// 	}
// 	return (true);
// }



//->PABLO
// ici on fair les redirection avec panache
// cas 1 : j'ai pas de pipe avant :
// je ne fais rien
// cas 2 : j'ai une pipe avant :
// je dois dup 2 mon fd 0 vers le fd[0] de la pipe

// cas 1 bis : j'ai pas de pipe apres :
// je ne fais rien
// cas 2 bis : j'ai une pipe apres :
// je dois dup 2 mon fd 1 vers le fd 1 de la pipe

bool	ft_redir(t_element *cmd, t_env *env, t_pipe *exec, int i)
{

	// A GERER APRES
	// if ()//j'ai une / des redirection d'entree
	// {
	// }
	if (pipe(exec->my_pipes[i]) < 0)
		perror("pipe");
	exec->pid[i] = fork();
	if (exec->pid[i] < 0)
	{
		perror ("fork");
		return (false);
	}
	if (exec->pid[i] == 0) //child
	{
		if (i == 0) // first command
		{
			//dans le cas d'infile dup2(fd_infile, 0);
			if (dup2(exec->my_pipes[i][1], 1) < 0)
				printf("dup2 1 failed\n");
			// ft_close(exec->my_pipes[0]);
			close(exec->my_pipes[0][0]);
			close(exec->my_pipes[0][1]);
		}
		else if (i == exec->cmd_nb - 1) // last command
		{
			//dans le cas outfile dup(fd, 1);
			if (dup2(exec->my_pipes[(i + 1) % 2][0], 0) < 0)
				printf("dup2 2 failed\n");
			// ft_close(exec->my_pipes[1]);
			close(exec->my_pipes[(i + 1) % 2][0]);
			close(exec->my_pipes[(i + 1) % 2][1]);
		}
		else // les middles commands
		{
			if (dup2(exec->my_pipes[(i + 1) % 2][0], 0) < 0) // on alterne entre 0 et 1 (pour alterner les fd)
				printf("dup2 3 failed\n");
			if (dup2(exec->my_pipes[i % 2][1], 1) < 0)
				printf("dup2 4 failed\n");
		}
		// ft_close infile et outfile
		execute_command(cmd, env, exec, i);
		//ft_execve();
	}
	// if (i)
	// 	ft_close_pipe(exec->my_pipes[(i + 1) % 2]);
	// ft_close_all_pipes(exec);
	return (true);
}

void	ft_close_fd(t_pipe *exec)
{
	close(exec->my_pipes[0][0]);
	close(exec->my_pipes[0][1]);
	close(exec->my_pipes[1][0]);
	close(exec->my_pipes[1][1]);
}

void	execution(t_element *cmd, t_pipe *exec)
{
	int	i;

	if (!ft_give_me_my_pipes(exec))
		;// le menage
	i = 0;
	if (exec->cmd_nb == 1 && cmd->builtin == true)
	{
		;//je lance le builtin sans lancer de child process
		return ;
	}
	while (i < exec->cmd_nb)
	{
		//ft_child(cmd, exec, i);
		i++;
	}
	
// fin de ft_redir
// if (!cmd)
// 		return (true);
// 	if (ft_is_a_pipe_before(cmd))
// 	{
// 		printf("i = %d\n", i);
// 		printf("content BEFORE = %s\n", cmd->content);
// 		dup2(exec->my_pipes[i - 1][0], 0);
// 	}

// 	// A GERER APRES
// 	// if ()// j'ai une / des redirections de sortie
// 	// {

// 	// }
	
// 	else if (ft_is_a_pipe_after(cmd))
// 	{
// 		printf("i = %d\n", i);
// 		printf("content AFTER = %s\n", cmd->content);
// 		dup2(exec->my_pipes[i][1], 1);
// 	}
// 	// ft_close_pipe(exec->fd_file); // apres
// 	ft_close_all_pipes(exec);
// 	//ft_execve()
// 	return (true);
// }


// void	ft_child(t_element *cmd, t_pipe *exec, int i)
// {
// 	// etape 1 on redirige les trucs
// 	//if (!ft_redir(cmd, exec, i))
// 		;
// 	// etape 2 on cherche un chemin
// 	// etape 3 on execute l'enfant
// }
}