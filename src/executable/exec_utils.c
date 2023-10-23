/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 14:56:39 by kquerel           #+#    #+#             */
/*   Updated: 2023/10/23 20:31:23 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

/*
Splits the path
Get the PATH variable from env list and split it by ':'
Returns an array of strings
*/
char	**split_path(t_env *env)
{
	char **res_split;
	
	res_split = NULL;
	if (is_key_in_env(env, "PATH") == true)
	{
		env = find_value_with_key_env(env, "PATH");
		res_split = ft_split(env->value, ':');
	}
	return (res_split);
}

/*
Fills every command or option type in cmd_tab
*/
void	fill_cmd_tab(t_element *cmd, t_pipe *exec)
{
	int	i;

	i = 0;
	while (cmd)
	{
		if (cmd->type == COMMAND || cmd->type == OPTION) // a changer pour apres
		{
			exec->cmd_tab[i] = malloc(sizeof(char ) * ft_strlen(cmd->content) + 1);
			ft_strcpy(exec->cmd_tab[i], cmd->content);
			//printf("cmd_tab[%d] = %s\n", i, exec->cmd_tab[i]);
			i++;
		}
		cmd = cmd->next;
	}
	exec->cmd_tab[i] = NULL;
}
/* Gets COMMAND and OPTION cmd type number */
int	get_args_nb(t_element *cmd)
{
	int	cmds_and_options;

	cmds_and_options = 0;
	while (cmd)
	{
		if (cmd->type == COMMAND || cmd->type == OPTION)
			cmds_and_options++;
		cmd = cmd->next;
	}
	return (cmds_and_options);
}


/* Gets COMMAND cmd type number */
int	get_cmds_nb(t_element *cmd, t_pipe *exec)
{
	exec->cmd_nb = 0;
	while (cmd)
	{
		if (cmd->type == COMMAND)
			exec->cmd_nb++;
		cmd = cmd->next;
	}
	return (exec->cmd_nb);
}


// /* Forks every child processes */
// int	ft_fork(t_element *cmd, t_pipe *exec, int pipe_e[2], int fd)
// {
// 	int	i;

// 	i = 0;
	
// 	exec->pid[i] = fork();
// 	if (exec->pid[i] < 0)
// 	{
// 		perror("pipe");
// 		return (EXIT_FAILURE);
// 	}
// 	// printf("%sJE SUIS LA%s\n", BGRE, WHT);
// 	if (exec->pid[i] == 0)
// 		ft_dup(cmd, exec, pipe_e, fd);
// 	i++;
// 	return (EXIT_SUCCESS);
// }


// void	ft_dup(t_element *cmd, t_pipe *exec, int pipe_e[2], int fd)
// {
// 	exec->here_doc = 0; // juste pour utiliser exec
// 	if (cmd->prev && dup2(fd, STDIN_FILENO) < 0)
// 	{
// 		perror("dup");
// 		ft_putstr_fd("pipe creation failed\n", STDERR_FILENO);
// 	}
// 	close(pipe_e[0]);
// 	if (cmd->next && dup2(pipe_e[1], STDOUT_FILENO) < 0)
// 	{
// 		perror("dup");
// 		ft_putstr_fd("pipe creation failed\n", STDERR_FILENO);
// 	}
// 	close(pipe_e[1]);
// 	if (cmd->prev)
// 		close(fd);
// 	execute_command(cmd, exec->env_s, exec, i);
// 	// printf("%sJE SUIS LA%s\n", BGRE, WHT);
	
// }

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