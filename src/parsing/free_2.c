/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: casomarr <casomarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/02 12:41:23 by kquerel           #+#    #+#             */
/*   Updated: 2023/12/05 15:27:36 by casomarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_delete_node_cmd(t_element **head, t_element *to_delete)
{
	int	no_next;
	
	no_next = 0;
	if (!to_delete)
		return (2);
	if (!to_delete->prev && !to_delete->next)
	{
		ft_free_null_cmd(to_delete);
		return (1);
	}
	if (to_delete->prev)
		to_delete->prev->next = to_delete->next;
	else
		*head = to_delete->next;
	if (to_delete->next)
		to_delete->next->prev = to_delete->prev;
	else
		no_next = 3;
	ft_free_null_cmd(to_delete);
	return (no_next);
}

/* Fonction free parent test. seule diff : condition to_delete->change*/
int	ft_delete_node_cmd_parent(t_element **head, t_element *to_delete)
{
	if (!to_delete)
		return (2);
	if (!to_delete->prev && !to_delete->next)
	{
		ft_free_null_cmd(to_delete);
		return (1);
	}
	if (to_delete->prev)
		to_delete->prev->next = to_delete->next;
	else
		*head = to_delete->next;
	if (to_delete->next)
		to_delete->next->prev = to_delete->prev;
	ft_free_null_cmd(to_delete);
	return (0);
}

/*Fonction free parent test*/
int	free_cmd_list_parent(t_element *cmd_list)
{
	t_element	**head;

	head = &cmd_list;
	while (cmd_list->prev != NULL)
		cmd_list = cmd_list->prev;
	while (cmd_list != NULL)
	{
		if (ft_delete_node_cmd_parent(head, cmd_list) == 1)
		{
			cmd_list = NULL;
			return (1);
		}
	}
	free(cmd_list);
	cmd_list = NULL;
	return (0);
}

/* Frees and sets to NULL */
void	ft_free_null_cmd(t_element *to_delete)
{
	if (to_delete->content != NULL)
	{
		free(to_delete->content);
		to_delete->content = NULL;
	}
	free(to_delete);
	to_delete = NULL;
}

void	free_and_update(char *line, t_element *cmd_list, t_pipe *exec, \
t_env **env_list)
{
	if (compare(line, "$") == true)
	{
		free(cmd_list);
		cmd_list = NULL;
	}
	free(line);
	line = NULL;
	free(exec->prompt);
	pwd_update_in_env(env_list);
}
