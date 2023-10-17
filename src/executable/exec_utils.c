/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karl <karl@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 14:56:39 by kquerel           #+#    #+#             */
/*   Updated: 2023/10/16 19:50:04 by karl             ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

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


/* Forks every child processes */
int	ft_fork(t_element *cmd, t_pipe *exec, int pipe_e[2], int fd)
{
	static int	i;

	i = 0;
	
	exec->pid[i] = fork();
	if (exec->pid[i] < 0)
	{
		perror("pipe");
		return (EXIT_FAILURE);
	}
	// printf("%sJE SUIS LA%s\n", BGRE, WHT);
	if (exec->pid[i] == 0)
		ft_dup(cmd, exec, pipe_e, fd);
	i++;
	return (EXIT_SUCCESS);
}


void	ft_dup(t_element *cmd, t_pipe *exec, int pipe_e[2], int fd)
{
	exec->here_doc = 0; // juste pour utiliser exec
	if (cmd->prev && dup2(fd, STDIN_FILENO) < 0)
	{
		perror("dup");
		ft_putstr_fd("pipe creation failed\n", STDERR_FILENO);
	}
	close(pipe_e[0]);
	if (cmd->next && dup2(pipe_e[1], STDOUT_FILENO) < 0)
	{
		perror("dup");
		ft_putstr_fd("pipe creation failed\n", STDERR_FILENO);
	}
	close(pipe_e[1]);
	if (cmd->prev)
		close(fd);
	execute_command(cmd, exec->env, exec);
	// printf("%sJE SUIS LA%s\n", BGRE, WHT);
	
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