/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/02 23:29:17 by kquerel           #+#    #+#             */
/*   Updated: 2023/12/03 00:51:40 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*Sets next to NULL if line is over, otherwise initialises the next command.*/
void	parsing_initialize_next(t_element **current_cmd, char *line, \
int *i)
{
	if (line[(*i)] == '\0')
		(*current_cmd)->next = NULL;
	else
	{
		(*current_cmd)->next = lstnew(line, (*i), parsing_str_type(line, (*i)));
		(*current_cmd)->next->prev = (*current_cmd);
		(*current_cmd) = (*current_cmd)->next;
	}
}

int	parsing_fix_dollar(t_element **cmd_list, t_element *current, \
t_env *env_list)
{
	if (is_in_line(current->content, "$") == true && \
	current->change == true && current->type != ARGUMENT) // FAIRE LE TEST echo "hola $USER"
	{
		current->content = dollar(current->content, env_list);
		if (current->content == NULL)
		{
			if (ft_delete_node_cmd(cmd_list, current) == 1)
			{
				*cmd_list = NULL;
				return (1);
			}
			current = NULL;
		}
	}
	return (0);
}
