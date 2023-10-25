/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 14:56:39 by kquerel           #+#    #+#             */
/*   Updated: 2023/10/25 18:17:45 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

/* Splits the path
Get the PATH variable from env list and split it by ':'
Returns an array of strings */
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

/* Fills every COMMAND or OPTION type in cmd_tab */
void	fill_cmd_tab(t_element *cmd, t_pipe *exec)
{
	int	i;

	i = 0;
	while (cmd)
	{
		// if (cmd->type == COMMAND/*  || cmd->type == OPTION */) // a changer pour apres
		if (cmd->type != PIPE)
		{
			exec->cmd_tab[i] = ft_calloc(ft_strlen(cmd->content) + 1, sizeof(char));
			cmd = ft_calloc(1, sizeof(t_element))
			exec->cmd_tab[i] = ft_strcpy(exec->cmd_tab[i], cmd->content);
			//printf("cmd_tab[%d] = %s\n", i, exec->cmd_tab[i]);
			i++;
		}
		cmd = cmd->next;
	}
	exec->cmd_tab[i] = NULL;
}

// /* Fills every COMMAND or OPTION type in cmd_tab */
// void	fill_cmd_tab(t_element *cmd, t_pipe *exec)
// {
// 	int	i;

// 	i = 0;
// 	while (cmd)
// 	{
// 		// if (cmd->type == COMMAND/*  || cmd->type == OPTION */) // a changer pour apres
// 		{
// 			exec->cmd_tab[i] = ft_calloc(ft_strlen(cmd->content) + 1, sizeof(char));
// 			ft_strcpy(exec->cmd_tab[i], cmd->content);
// 			//printf("cmd_tab[%d] = %s\n", i, exec->cmd_tab[i]);
// 			i++;
// 		}
// 		cmd = cmd->next;
// 	}
// 	exec->cmd_tab[i] = NULL;
// }

/* Gets all cmd->type number until the next PIPE */
int	get_args_nb(t_element *cmd)
{
	int	cmds_and_options;
	cmds_and_options = 1;
	
	if (!cmd)
		return (0);
	while (cmd)
	{
		if (cmd->type != PIPE)
			cmds_and_options++;
		cmd = cmd->next;
	}
	return (cmds_and_options);
}

/* Gets COMMAND cmd->type number */
int	get_cmds_nb(t_element *cmd, t_pipe *exec)
{
	exec->cmd_nb = 0;
	if (!cmd)
		return (0);
	while (cmd)
	{
		if (cmd->type == COMMAND)
			exec->cmd_nb++;
		cmd = cmd->next;
	}
	return (exec->cmd_nb);
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

/* prints an array */
void	ft_print_array(char **arr)
{
	int	i = 0;
	while (arr[i])
	{
		printf("arr[%d] = %s\n", i, arr[i]);
		i++;
	}
}