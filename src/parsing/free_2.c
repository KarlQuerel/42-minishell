/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/02 12:41:23 by kquerel           #+#    #+#             */
/*   Updated: 2023/12/02 12:41:52 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_delete_node_cmd(t_element **head, t_element *to_delete)
{

/////////////// FAIRE TEST QD HEREDOC
        /* if (to_delete->content)
        {
            free(to_delete->hd_filename);
            to_delete->hd_filename = NULL;
        } */
///////////////

	if (!to_delete)
		return (2);
	if(!to_delete->prev && !to_delete->next)
	{
		if (to_delete->content != NULL)
		{
			//printf("to_delete->content : %s\n", to_delete->content);
			//printf("cmd\n");
			free(to_delete->content);
			to_delete->content = NULL;
		}
		// if (to_delete->content)
		// {
		// 	free(to_delete->hd_filename);
		// 	to_delete->hd_filename = NULL;
		// }
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
	// if (to_delete->content)
	// {
	// 	free(to_delete->hd_filename);
	// 	to_delete->hd_filename = NULL;
	// }
	free(to_delete);
	to_delete = NULL;
	return (0);
}
