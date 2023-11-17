/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: casomarr <casomarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 14:56:39 by kquerel           #+#    #+#             */
/*   Updated: 2023/11/17 17:33:11 by casomarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

/* Splits the path
Get the PATH variable from env list and split it by ':'
Returns an array of strings */
char	**split_path(t_env *env)
{
	char	**res_split;

	res_split = NULL;
	if (is_key_in_env(env, "PATH") == true)
	{
		env = find_value_with_key_env(env, "PATH");
		res_split = ft_split(env->value, ':');
	}
	return (res_split);
}

/* Fills cmd_tab with the current cmd 
Stops if it encounters a node with cmd->type PIPE or the end of the list*/
void	fill_array(t_element *cmd, t_pipe *exec)
{
	int	i;

	// if (cmd->prev && cmd->prev->type < 3)
	// 	free_cmd_arr(exec);
	i = 0;
	while (cmd && cmd->type < 3)
	{
		exec->cmd_tab[i] = ft_calloc(ft_strlen(cmd->content) + 1, sizeof(char));
		exec->cmd_tab[i] = ft_strcpy(exec->cmd_tab[i], cmd->content);
		cmd = cmd->next;
		i++;
	}
	exec->cmd_tab[i] = NULL;
}

/* Gets the size of the whole command for memory allocation */
int	get_size_cmd(t_element *cmd)
{
	int	i;

	i = 0;
	while (cmd)
	{
		if (cmd->type != PIPE) // peut etre a changer pour les redir
			i++;
		cmd = cmd->next;
	}
	return (i);
}

/* Counts the number of pipes */
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

/* Strcpy */
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
