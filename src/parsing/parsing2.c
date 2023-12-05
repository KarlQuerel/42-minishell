/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: casomarr <casomarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 19:55:56 by octonaute         #+#    #+#             */
/*   Updated: 2023/12/05 16:39:43 by casomarr         ###   ########.fr       */
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

int	skip_first_quote(char *line, int *i)
{
	int start;
	int	temp;

	start = (*i);
	temp = (*i);
	if (line[(*i)] == '\'' || line[(*i)] == '\"')
	{
		(*i)++;
		while (line[temp] != line[start])
			temp++;
		return (temp);
	}
	return (-1);
}

void	fill_content_loop(t_element **cur, char *line, int *i, \
char *sep)
{
	int	x;
	int	j;
	int	closing_quote;

	j = 0;
	closing_quote = -1;
	while (line[(*i)])
	{
		x = 0;
		while (sep[x])
		{
			closing_quote = skip_first_quote(line, i);
			if (closing_quote != -1)
			{
				while((*i) < closing_quote && line[(*i)])
					(*cur)->content[j++] = line[(*i)++];
			}
			if (line[(*i)] && line[(*i)] == sep[x])
			{
				if (sep[x] == '|' && j == 0)
					(*cur)->content[j++] = line[(*i)++];
				(*cur)->content[j] = '\0';
				return ;
			}
				x++;
		}
		if (line[(*i)])
			(*cur)->content[j++] = line[(*i)++];
	}
}
