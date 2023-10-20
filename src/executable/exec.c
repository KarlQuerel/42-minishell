/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 14:46:12 by kquerel           #+#    #+#             */
/*   Updated: 2023/10/20 13:44:12 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

/*
TO DO:

- UNSET
- EXPORT



- cas pablo (demander a caro la photo)
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
void	execute_command(t_element *cmd, t_env *env, t_pipe *exec, char *line, char *home_path)
{
	int	pid;
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
		if (cmd->builtin == true)
		{
			commands(line, env, home_path);
			return ;
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
		if (execve(cmd->content, exec->cmd_tab, env->env) == -1)
			ft_putstr_fd("execve failed\n", STDOUT_FILENO);
		/* wlaw
			access(cmd_tab[0], F OK, regarder sur internet) */
		
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
			if (access(to_return, F_OK) == 0)
				return (to_return);
			free(to_return);
			i++;
		}
	}
	return (NULL);
}

/* Handles execution */
void	ft_execute(t_element *cmd, t_env *env, t_pipe *exec, char *line, char *home_path)
{
	exec->av_nb = get_args_nb(cmd);
	exec->cmd_tab = malloc(sizeof(char *) * (exec->av_nb + 1));
	if (!exec->cmd_tab)
		return ;
	fill_cmd_tab(cmd, exec);
	get_cmds_nb(cmd, exec); // utiliser le nombre de commands while (i < nb_commands)
	if (exec->cmd_nb == 1) // dans le cas d'une single command
		execute_command(cmd, env, exec, line, home_path);
	else // plusieurs commandes
	{
		redir(cmd, exec, env, line, home_path);
	}


	// {
		
	// 	int	i = 0;
	// 	while (i < exec->cmd_nb)
	// 	{
	// 		ft_children(cmd, exec, i, line, home_path);
	// 		i++;
	// 	}
	// }
	
	// {
	// 	exec->pid = ft_calloc(sizeof(int), exec->cmd_nb + 2);
	// 	if (!exec->pid)
	// 		return (msg_error(0));
	// 	childrens(cmd, exec, line, home_path);
	// }
}

// void ft_children(t_element *cmd, t_pipe *exec, int i, char *line, char *home_path)
// {
	
// }

void	redir(t_element *cmd, t_pipe *exec, t_env *env, char *line, char *home_path)
{
	pid_t	pid;
	int	pipefd[2];

	if (pipe(pipefd) < 0)
	{
		perror("pipe");
		return ;
	}
	pid = fork();
	if (pid) // parent
	{
		close(pipefd[1]);
		dup2(pipefd[0], STDIN_FILENO);
		waitpid(pid, NULL, 0);
	}
	else // child pid == 0
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		execute_command(cmd, env, exec, line, home_path);
	}
}

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

bool	ft_give_me_my_pipes(t_pipe *exec)
{
	if (exec->cmd_nb == 1)
	{
		exec->my_pipes = NULL;
		return (true);
	}
	exec->my_pipes = malloc(sizeof(int) * 2 * (exec->cmd_nb - 1));
	if (!exec->my_pipes)
		return (false);
	return (true);
}


void	execution(t_element *cmd, t_pipe *exec)
{
	int	i;

	if (!ft_give_me_my_pipes(exec))
		;// le menage
	i = 0;
	while (i < exec->cmd_nb)
	{
		if ()
		ft_children(cmd, exec, i);
		i++;
	}
}
// 	if (ft_is_a_pipe_before(cmd))
// 	{
// 		dup2(, 0);
// 		close();
// 	}

// cas 1 : j'ai pas de pipe avant :
// je ne fais rien
// cas 2 : j'ai une pipe avant :
// je dois dup 2 mon fd 0 vers le fd[0] de la pipe

// cas 1 bis : j'ai pas de pipe apres :
// je ne fais rien
// cas 2 bis : j'ai une pipe apres :
// je dois dup 2 mon fd 1 vers le fd 1 de la pipe





/* fonction test */
int	childrens(t_element *cmd, t_pipe *exec, char *line, char *home_path)
{
	int		pipe_end[2];
	int		fd;

	fd = STDIN_FILENO;
	// printf("JE SUIS LA\n");
	while (cmd && cmd->next)
	{
		if (cmd->type != COMMAND && cmd->type != OPTION) // pour sauter les pipes
			cmd = cmd->next;
		printf("cmd = %s\n", cmd->content);
		if (cmd->next)
		{
			if (pipe(pipe_end) < 0)
			{
				printf("hello\n");
				exit(EXIT_FAILURE);
			}
			ft_create_pipe(exec, pipe_end);
		}
		ft_fork(cmd, exec, pipe_end, fd, line, home_path);
		close(pipe_end[1]);
		if (cmd->prev)
			close(fd);
		if (cmd->next)
			cmd = cmd->next;
		else
			break ;

	}
	ft_waitpid(exec->pid, exec->cmd_nb);
	return (0);
}

// int	execute_cmd(t_element *cmd, t_pipe *exec)
// {
// }

		
	// else // on split les process car pipe presents
	// {
	// 	i = 0;
	// 	// ft_create_pipe(exec); // ne marche ->bad adress
	// 	while (i < exec->av_nb)
	// 	{
	// 		mult_commands(cmd, env, exec, i);
	// 		i++;
	// 	}
	// }


	//--------------------- pipe

	// printf("pipe_end[%d] = %d\n", 0, exec->pipe_end[0]);
	// printf("pipe_end[%d] = %d\n", 1, exec->pipe_end[1]);
	// printf("pipe_end[%d] = %d\n", 2, exec->pipe_end[2]);

	// if (pipe(exec->pipe_end) < 0)
	// {
	// 	perror("pipe:");
	// 	//free
	// 	exit(EXIT_FAILURE);
	// }


	//------------------------- fork

	// exec->pid = fork();
	// if(exec->pid == -1)
	// {
	// 	perror("fork:");
	// 	//free
	// 	exit(EXIT_FAILURE);
	// }
	// else if (exec->pid == 0)
	// {
	// 	if (dup2(exec->pipe_end[1], 1) < 0)
	// 	{
	// 		perror("dup2:");
	// 		//free
	// 		exit(EXIT_FAILURE);
	// 	}
	// 	close(exec->pipe_end[0]);
	// 	close(exec->pipe_end[1]);
	// }

	
	
	// if (exec->av_nb == 0)
	// {
	// 	printf("Je suis la\n"); // c'est pour ca que ce connard de minishell exit
	// 	exit(127);
	// }

	// TEST TEST TEST
	// exec->cmd_path = split_path(env);
	// if (!exec->cmd_path)
	// {
	// 	printf("Split_path failed\n");
	// 	//free les pipes dans le cas ou on les malloc
	// 	// free (exec);
	// 	// free en plus dans le code de caro ?
	// }
	
	
	/* POUR PRINT LE PATH
	-> la ou toutes les commandes se trouvent (check avec whereis)
	*/
	// int	j = 0;
	// while (exec->cmd_path[j])
	// {
	// 	printf("cmd_path[%d] = %s\n", j, exec->cmd_path[j]);
	// 	j++;
	// }
	// exec->cmd_tab = ft_split(exec->cmd_tab[0], ' ')

	// TEST TEST TEST
	// cmd->content = ft_get_command(exec->cmd_path, exec->cmd_tab[0]);
	// if (!cmd->content)
	// {
	// 	if (!exec->cmd_tab[0])
	// 		ft_putstr_fd("\n", 2);
	// 	else
	// 	{
	// 		ft_putstr_fd(exec->cmd_tab[0], 2);
	// 		ft_putstr_fd(": command not found\n", 2);
	// 	}
	// }
	// printf("1st argument AKA cmd->content = %s\n", cmd->content);
	// printf("2nd argument AKA cmd->content_tab[0] = %s\n", exec->cmd_tab[0]);
	// printf("2nd argument AKA cmd->content_tab[1] = %s\n", exec->cmd_tab[1]);
	
	
	// TEST TEST TEST
	// execve(cmd->content, exec->cmd_tab, env->env);
	// printf("--> S'affiche uniquement si execve fail <--\n");

	
	// ft_create_pipe(exec); // cree le bon nombre de pipes
	// int i = 0;
	// while (i < exec->av_nb)
	// {
	// 	ft_babyboom(cmd, env, exec, i);
	// 	i++;
	// }
	// ft_close_pipe(exec);
	// waitpid(exec->pid, NULL, 0);


