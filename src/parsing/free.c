/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: casomarr <casomarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/30 13:41:24 by casomarr          #+#    #+#             */
/*   Updated: 2023/11/17 18:23:59 by casomarr         ###   ########.fr       */
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
/* 	free (exec->line[0]);
	free (exec->prompt[0]); //a voir */
	free (*exec->line);
	free (*exec->prompt); //a voir
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
	if (exec->fd_temp)
		free(exec->fd_temp);
}

/*Goes back to the head of cmd_list if needed and
frees everything.*/
void	free_cmd_list(t_element *cmd_list)
{
	t_element	**head;

	head = &cmd_list;
	while (cmd_list->prev != NULL)
		cmd_list = cmd_list->prev;
	while (cmd_list != NULL)
		ft_delete_node_cmd(head, cmd_list); //ca fait cmd = cmd->next
	free(cmd_list);
}

void	free_env_list(t_env *env_list)
{
	t_env	**head;

	head = &env_list;
	while (env_list != NULL)
	{
		// free(env_list->value);
		// free(env_list->key);
		// env_list = env_list->next;
		ft_delete_node_env(head, env_list);
	}
	free(env_list);
}
