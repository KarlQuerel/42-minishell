/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karl <karl@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 14:56:39 by kquerel           #+#    #+#             */
/*   Updated: 2023/10/23 23:02:28 by karl             ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

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

/* Gets COMMAND and OPTION cmd->type number */
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

/* Gets COMMAND cmd->type number */
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