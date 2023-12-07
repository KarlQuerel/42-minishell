/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/02 23:29:17 by kquerel           #+#    #+#             */
/*   Updated: 2023/12/07 14:21:40 by kquerel          ###   ########.fr       */
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
		(*current_cmd)->next = lstnew(line, (*i), CMD);
		(*current_cmd)->next->prev = (*current_cmd);
		(*current_cmd) = (*current_cmd)->next;
	}
}

int	parsing_fix_dollar(t_element **cmd_list, t_element *current, \
t_env *env_list)
{
	int	ret;

	if (is_in_line(current->content, "$") == true && \
	current->change == true)
	{
		current->content = dollar(current->content, env_list);
		if (current->content == NULL)
		{
			ret = ft_delete_node_cmd(cmd_list, current);
			if (ret == 1)
			{
				*cmd_list = NULL;
				return (1);
			}
			current = NULL;
			if (ret == 3)
				return (2);
		}
	}
	return (0);
}

void	type_arg_after_cmd(t_element **current)
{
	t_element	*temp;

	if ((*current)->type == COMMAND && (*current)->next)
	{
		temp = (*current)->next;
		while (temp->type != PIPE && temp != NULL)
		{
			if (temp->type != OPTION && temp->type < 3)
				temp->type = ARGUMENT;
			if (temp->next != NULL)
				temp = temp->next;
			else
				break ;
		}
	}
}

bool	no_cmd_before(t_element *current)
{
	while (current->prev && current->prev->type != PIPE)
	{
		if (current->prev->type == COMMAND)
			return (false);
		current = current->prev;
	}
	return (true);
}

/* To fix the type of the arguments that are not in between quotes
and are therefore considered as a COMMAND instead of an ARGUMENT
in the parsing function. This functions sets all arguments that are
not of type OPTION or redirecter after a cmd to ARGUMENT until a 
type PIPE is found.*/
int	parsing_fix(t_element **cmd_list, t_env *env_list)
{
	t_element	*current;
	int			ret;

	current = (*cmd_list);
	while (current != NULL)
	{
		if ((current->prev != NULL && current->prev->type >= 3 && \
		current->type < 3 && no_cmd_before(current) == true) || \
		(current->prev == NULL && current->type < 3))
			current->type = COMMAND;
		if (current->type == COMMAND && current->next)
			type_arg_after_cmd(&current);
		ret = parsing_fix_dollar(cmd_list, current, env_list);
		if (ret == 1)
			return (1);
		else if (ret == 0)
			current->change = false;
		else if (ret == 2)
			current = NULL;
		if (current)
			current = current->next;
	}
	return (0);
}
