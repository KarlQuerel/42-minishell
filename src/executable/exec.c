/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 14:46:12 by kquerel           #+#    #+#             */
/*   Updated: 2023/10/05 15:33:33 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

void	ft_redirect(t_element *s)
{
	while (s)
	{
		printf("%s\n", s->content);
		if (s->type == 0)
			printf(" -> pipe\n");
		else if (s->type == 1)
			printf(" -> pipe - option\n");
		else if (s->type == 2)
			printf(" -> arg\n");
		else if (s->type == 3)
			printf(" -> fd_infile\n");
		else if (s->type == 4)
			printf(" -> fd_outfile\n");
		else if (s->type == 5)
			printf(" -> fd_infile HEREDOC\n");
		else if (s->type == 6)
			printf(" -> fd_outfile HEREDOC\n");
		else if (s->type == 7)
			printf(" -> pipec_bonus\n");
		s = s->next;
	}
}

/*
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

/* Splits the path*/
char	**split_path(t_env *env_list)
{
	char **res_split;
	
	env_list = find_value_with_key_env(env_list, "PATH");
	res_split = ft_split(env_list->value, ':');
	return (res_split);
}

/* Gets commands number (type 0) */
int	get_args_nb(t_element *cmd)
{
	int	nb_args;

	nb_args = 0; // peutre commencer a 1 ?
	cmd->cmd_tab = malloc(9999); // a fix
	while (cmd) //while qui seg fault
	{
		if (cmd->type == COMMAND)
		{
			cmd->cmd_tab[nb_args] = cmd->content; //ca marche sur papier mais pas sur
			nb_args++;
			printf("-----> args = %d\n", nb_args);
		}
		cmd = cmd->next;
	}
	return (nb_args);
}

/* Creates child process
--> will fork as long as i is < to av_nb
--> several cases occur, first child, middle child and last child
*/
void	ft_babyboom(t_element *cmd, t_env *env, t_pipe *exec, int i)
{
	exec->pid = fork(); // system call to create new process (child)
	if (exec->pid == -1) // if it fails
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	else if (exec->pid == 0) // meaning we are in the child process
	{
		if (i == 0) // its the first child in the pipeline
		{
			dup2(exec->fd_infile, 0); //fd_infile = result of the open function, fd 0 now reads from infile
			dup2(exec->pipe_end[1], 1);
		}
		else if (i == exec->av_nb) // last child in the pipeline
		{
			dup2(exec->pipe_end[(i * 2) - 2], 0); // duplicates the reading pipe (fd[0]) of the last child to the writing of the previous child
			dup2(exec->fd_outfile, 1); //fd_outfile = result of the open function
		}
		else // all the middle childs
		{
			dup2(exec->pipe_end[(i * 2) - 2], 0);
			dup2(exec->pipe_end[(i * 2) + 1], 1); // we redirect the writing pipe (fd[1])
		}
		ft_close_pipe(exec);
		cmd->cmd_tab = ft_split(cmd->cmd_tab[i], ' ');
		cmd->cmd = ft_get_command(exec->cmd_path, cmd->cmd_tab[0]);
		if (!cmd->cmd)
		{
			printf("Error\n");
			// free
			exit (EXIT_FAILURE);
		}
		execve(cmd->content, cmd->cmd_tab, env->env);
	}
}


void	ft_execute(t_element *cmd, t_env *env)
{
	// find nb of commands if type == 0, count_cmd ++;
	// nombre de pipe = (argv - 1) * 2;
	//faire le malloc des pipe
	//create pipes
	// A FAIRE
	// gerer le open et le here_doc !
	t_pipe *exec;

	exec = malloc(sizeof(t_pipe));
	if (!exec)
	{
		perror("exec");
		exit(EXIT_FAILURE);
	}
	exec->av_nb = get_args_nb(cmd); //seg fault ici
	exec->pipe_nb = (exec->av_nb) * 2;
	//peut etre malloc le nombre de pipe, pas sur, voir avec caro
	exec->cmd_path = split_path(env);
	if (!exec->cmd_path)
	{
		printf("Split_path failed\n");
		//free les pipes dans le cas ou on les malloc
		free (exec);
		// free en plus dans le code de caro ?
	}
	ft_create_pipe(exec); // cree le bon nombre de pipes
	int i = 0;
	while (i < exec->av_nb)
	{
		ft_babyboom(cmd, env, exec, i);
		i++;
	}
	ft_close_pipe(exec);
	waitpid(-1, NULL, 0);
}



/* Extracts command from argv and verify if they are valid */
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
			if (access(to_return, 0) == 0)
				return (to_return);
			free(to_return);
			i++;
		}
	}
	return (NULL);
}

/* Initiate pipe and create all pipe ends according to commands number */
void	ft_create_pipe(t_pipe *exec)
{
	int	i;

	i = 0;
	while (i < exec->av_nb)
	{
		if (pipe(exec->pipe_end + 2 * i) == -1)
		{
			//gerer les close and free
			perror("pipe_end");
			exit(EXIT_FAILURE);
		}
		i++;
	}
}
/* Close all pipe ends */
void	ft_close_pipe(t_pipe *exec)
{
	int	i;

	i = 0;
	while (i < exec->pipe_nb)
	{
		close(exec->pipe_end[i]);
		i++;
	}
}
