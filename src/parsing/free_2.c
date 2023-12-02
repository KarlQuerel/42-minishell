/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: casomarr <casomarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/02 12:41:23 by kquerel           #+#    #+#             */
/*   Updated: 2023/12/02 18:33:14 by casomarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_delete_node_cmd(t_element **head, t_element *to_delete)
{
	// printf("to_delete->content : %s\n", to_delete->content);
	// printf("to_delete->change : %d\n", to_delete->change);
	if (!to_delete)
		return (2);
	if(!to_delete->prev && !to_delete->next)
	{
		if (to_delete->content != NULL/*  && to_delete->change == false */)
		{
			free(to_delete->content);
			to_delete->content = NULL;
		}
		free(to_delete); // gdb nous montre que le free marche pas
		to_delete = NULL;
		return (1);
	}
	if (to_delete->prev)
		to_delete->prev->next = to_delete->next;
	else
		*head = to_delete->next;
	if (to_delete->next)
		to_delete->next->prev = to_delete->prev;
	if (to_delete->content != NULL/*  && to_delete->change == false */)
	{
		free(to_delete->content);
		to_delete->content = NULL;
	}
	free(to_delete);
	to_delete = NULL;
	return (0);
}

/* Fonction free parent test. seule diff : condition to_delete->change*/
int	ft_delete_node_cmd_parent(t_element **head, t_element *to_delete)
{
	if (!to_delete)
		return (2);
	if(!to_delete->prev && !to_delete->next)
	{
		if (to_delete->content != NULL)
		{
			free(to_delete->content);
			to_delete->content = NULL;
		}
		free(to_delete);
		to_delete = NULL;
		return (1);
	}
	if (to_delete->prev)
		to_delete->prev->next = to_delete->next;
	else
		*head = to_delete->next;
	if (to_delete->next)
		to_delete->next->prev = to_delete->prev;
	if (to_delete->content != NULL)
	{
		free(to_delete->content);
		to_delete->content = NULL;
	}
	free(to_delete);
	to_delete = NULL;
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
			//printf("cmd_list->builtin = %d\n", cmd_list->builtin);
			//free(cmd_list);
			cmd_list = NULL;
			return 1;
		}
	}
	free(cmd_list);
	cmd_list = NULL;
	return 0;
}