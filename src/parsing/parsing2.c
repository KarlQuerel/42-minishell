/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 19:55:56 by octonaute         #+#    #+#             */
/*   Updated: 2023/12/03 00:51:58 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*Initializes the element_list by returning the first node.*/
t_element	*parsing_initialisation(char *line, int *i, int *start)
{
	int	typestr;

	(*i) = 0;
	while ((line[(*i)] == '<' || line[(*i)] == '>') && line[(*i)])
		(*i)++;
	if ((*i) != 0 && line[(*i)] == ' ')
		(*i)++;
	(*start) = (*i);
	typestr = parsing_str_type(line, (*i));
	return (lstnew(line, (*start), typestr));
}

/*Advances the i and start variables until the beginning of the next word*/
void	parsing_advance_to_next_word(char *line, int *start, int *i)
{
	while ((line[(*i)] == ' ' || line[(*i)] == '<' || \
	line[(*i)] == '>') && line[(*i)])
		(*i)++;
	if (line[(*i)] == ' ' || line[(*i)] == '<' || line[(*i)] == '>')
		(*start) = (*i) + 1;
	else
		(*start) = (*i);
}

void	parsing_fill_content(t_element **cur, char *line, int *i, \
char *sep)
{
	int	j;
	int	x;

	j = 0;
	if (sep[0] == '\'' || sep[0] == '\"')
		(*i)++;
	while (line[(*i)])
	{
		x = 0;
		while (sep[x])
		{
			if (line[(*i)] == sep[x])
			{
				if (sep[x] == '|' && j == 0)
					(*cur)->content[j++] = line[(*i)++];
				(*cur)->content[j] = '\0';
				if (sep[x] == '\'' || sep[x] == '\"')
					(*i)++;
				return ;
			}
			x++;
		}
		if (line[(*i)] == '\\')
			(*i)++;
		if (line[(*i)])
			(*cur)->content[j++] = line[(*i)++];
	}
	(*cur)->content[j] = '\0';
	if (line[(*i)] && (sep[0] == '\'' || sep[0] == '\"'))
		(*i)++;
}
