/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 14:46:12 by kquerel           #+#    #+#             */
/*   Updated: 2023/10/10 17:41:43 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

/*
TO DO:

- traiter les pipes - FINIR ft_babyboom
- les redirections
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


/* A FAIRE
-rediriger dans le cas ou la commande est un built in a coder nous meme
	- echo -n
	- cd
	- pwd
	- export
	- unset
	- env
	- exit
- peut etre creer un 8eme type dans le .h avec BUILTIN ? voir avec caro
*/
void	ft_redirect(t_element *s)
{
	/* par exemple
	while (s)
	{
		if (s->type == 8)
			on skip ft_execute pour eviter le doublon des commandes
		s = s->next;
	}
	
	*/
	while (s)
	{
		// printf("%s\n", s->content);
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
		// else if (s->type == 8)
		s = s->next;
	}
}

/* Splits the path*/
char	**split_path(t_env *env_list)
{
	char **res_split;
	
	env_list = find_value_with_key_env(env_list, "PATH");
	res_split = ft_split(env_list->value, ':');
	return (res_split);
}

/* Gets COMMANDS and OPTION number (type 0 and type 1) */
int	get_args_nb(t_element *cmd, t_pipe *exec)
{
	int	nb_args;

	nb_args = 0;
	while (cmd)
	{
		if (cmd->type == COMMAND || cmd->type == OPTION)
		{
			exec->cmd_tab[nb_args] = malloc(sizeof(char *) * ft_strlen(cmd->content) + 1);
			ft_strcpy(exec->cmd_tab[nb_args], cmd->content);
			// printf("cmd_tab[%d] = %s\n", nb_args, exec->cmd_tab[nb_args]);
			nb_args++;
		}
		cmd = cmd->next;
	}
	exec->cmd_tab[nb_args] = NULL;
	return (nb_args);
}


/* Gets the number of pipes depending on input */
int	get_pipe_nb(t_element *cmd, t_pipe *exec)
{
	exec->pipe_nb = 0;
	while (cmd)
	{
		if (cmd->type == PIPE)
			exec->pipe_nb++;
		cmd = cmd->next;
	}
	return (exec->pipe_nb);
}


/* Handles a single command */
void	single_command(t_element *cmd, t_env *env, t_pipe *exec)
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
			// free
		}
		cmd->cmd = ft_get_command(exec->cmd_path, exec->cmd_tab[0]);
		if (!cmd->cmd)
		{
			if (!exec->cmd_tab[0])
				ft_putstr_fd("\n", 2);
			else
			{
				ft_putstr_fd(exec->cmd_tab[0], 2);
				ft_putstr_fd(": command not found\n", 2);
			}
		}
		execve(cmd->cmd, exec->cmd_tab, env->env);
	}
	waitpid(pid, NULL, 0);
}

/* Creates child process
--> will fork as long as i is < to av_nb
--> several cases occur, first child, middle child and last child
*/
void	mult_commands(t_element *cmd, t_env *env, t_pipe *exec, int i)
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
			//dup2(0, 0); // inutile
			dup2(exec->pipe_end[1], 1);
		}
		else if (i == exec->av_nb) // last child in the pipeline
		{
			dup2(exec->pipe_end[(i * 2) - 2], 0); // duplicates the reading pipe (fd[0]) of the last child to the writing of the previous child
			dup2(1, 1); //fd_outfile = result of the open function
		}
		else // all the middle childs
		{
			dup2(exec->pipe_end[(i * 2) - 2], 0);
			dup2(exec->pipe_end[(i * 2) + 1], 1); // we redirect the writing pipe (fd[1])
		}
		ft_close_pipe(exec);
		exec->cmd_tab = ft_split(exec->cmd_tab[i], ' ');


		
		// int	j = 0;
		// while (exec->cmd_tab[j])
		// {
		// 	printf("cmd_tab[%d] = %s\n", j, exec->cmd_tab[j]);
		// 	j++;
		// }


		
		cmd->cmd = ft_get_command(exec->cmd_path, exec->cmd_tab[i]);

		printf("---%s----\n", cmd->cmd);
		
		if (!cmd->cmd)
		{
			if (!exec->cmd_tab[i])
				ft_putstr_fd("\n", 2);
			else
			{
				ft_putstr_fd(exec->cmd_tab[i], 2);
				ft_putstr_fd(": command not found\n", 2);
			}
		}
		execve(cmd->cmd, exec->cmd_tab, env->env);
	}
}

/* Handles execution */
void	ft_execute(t_element *cmd, t_env *env, t_pipe *exec)
{
	//faire le malloc des pipe
	//create pipes
	// A FAIRE
	// gerer le open et le here_doc !
	
	int	i;

	
	exec->cmd_tab = malloc(sizeof(char **) + 1); // +1 car le tableau de strings doit finir par NULL
	if (!exec->cmd_tab)
		return ;
	exec->av_nb = get_args_nb(cmd, exec);
	get_pipe_nb(cmd, exec);
	// printf("pipe_nb = %d\n", exec->pipe_nb);

	if (exec->pipe_nb == 0) // pas de pipe donc single command
		single_command(cmd, env, exec);
	else
	{
		i = 0;
		ft_create_pipe(exec);
		while (i < exec->av_nb)
		{
			mult_commands(cmd, env, exec, i);
			i++;
		}
	}
}



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
	// cmd->cmd = ft_get_command(exec->cmd_path, exec->cmd_tab[0]);
	// if (!cmd->cmd)
	// {
	// 	if (!exec->cmd_tab[0])
	// 		ft_putstr_fd("\n", 2);
	// 	else
	// 	{
	// 		ft_putstr_fd(exec->cmd_tab[0], 2);
	// 		ft_putstr_fd(": command not found\n", 2);
	// 	}
	// }

	
	// printf("1st argument AKA cmd->cmd = %s\n", cmd->cmd);
	// printf("2nd argument AKA cmd->cmd_tab[0] = %s\n", exec->cmd_tab[0]);
	// printf("2nd argument AKA cmd->cmd_tab[1] = %s\n", exec->cmd_tab[1]);
	
	
	// TEST TEST TEST
	// execve(cmd->cmd, exec->cmd_tab, env->env);
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
	while (i < exec->pipe_nb)
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


/* strcpy */
char	*ft_strcpy(char *dst, char *src)
{
	int	i;

	i = 0;
	while (src[i])
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (dst);
}