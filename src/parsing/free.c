/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/30 13:41:24 by casomarr          #+#    #+#             */
/*   Updated: 2023/11/17 12:14:21 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

/* When exit is called */
void	exit_free(t_element *cmd_list, t_env **env_list, t_pipe *exec)
{
	free_cmd_list(cmd_list);
	history(FREE_HISTORY, 0); // 0 : on s'en fout, on utilise pas len qd on free
	free_env_list(*env_list);
	free (exec->line[0]);
	free (exec->prompt[0]); //a voir
	free_cmd_arr(exec);
	free(exec);
}

void	free_cmd_arr(t_pipe *exec)
{
	int	i;

	i = 0;
	if (exec->cmd_tab)
	{	
		while (exec->cmd_tab[i])
			free(exec->cmd_tab[i++]);
		free(exec->cmd_tab);
	}
	if (exec->cmd_path)
	{
		i = 0;
		while (exec->cmd_path[i])
			free(exec->cmd_path[i++]);
		free(exec->cmd_path);
	}
}

void	free_cmd_list(t_element *cmd_list)
{
	while (cmd_list != NULL)
	{
		free(cmd_list->content);
		cmd_list = cmd_list->next;
	}
	free(cmd_list);

/* 	while (cmd_list != NULL)
	{
		free(cmd_list->content);
		cmd_list = cmd_list->prev;
	}
	free(cmd_list); */
}

void	free_env_list(t_env *env_list)
{
	while (env_list != NULL)
	{
		free(env_list->value);
		free(env_list->key);
		env_list = env_list->next;
	}
	free(env_list);
}
