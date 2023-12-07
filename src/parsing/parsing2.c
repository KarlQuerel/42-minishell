/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 19:55:56 by octonaute         #+#    #+#             */
/*   Updated: 2023/12/07 14:23:17 by kquerel          ###   ########.fr       */
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
	typestr = CMD;
	return (lstnew(line, (*start), typestr));
}

/*Advances the i and start variables until the beginning of the next word*/
void	parsing_advance_to_next_word(char *line, int *start, int *i)
{
	while (line[(*i)] && (line[(*i)] == ' ' || line[(*i)] == '<' || \
	line[(*i)] == '>'))
		(*i)++;
	if (line[(*i)] == ' ' || line[(*i)] == '<' || line[(*i)] == '>')
		(*start) = (*i) + 1;
	else
		(*start) = (*i);
}

int	skip_first_quote(char *line, int *i)
{
	int	start;
	int	temp;

	start = (*i);
	temp = (*i);
	if (line[(*i)] == '\'' || line[(*i)] == '\"')
	{
		(*i)++;
		temp++;
		while (line[temp] && line[temp] != line[start])
			temp++;
		return (temp);
	}
	return (-1);
}

void	fill_content_loop(t_element **cur, char *line, int *i)
{
	int		j;
	int		closing_quote;

	j = 0;
	closing_quote = -1;
	while (line[(*i)] && line[(*i)] != ' ' && line[(*i)] != '|' && \
	line[(*i)] != '<' && line[(*i)] != '>')
	{
		closing_quote = skip_first_quote(line, i);
		if (closing_quote != -1)
		{
			if (line[(*i) - 1] == '\'')
				(*cur)->change = false;
			while (line[(*i)] && (*i) < closing_quote)
				(*cur)->content[j++] = line[(*i)++];
			(*i)++;
		}
		else
			(*cur)->content[j++] = line[(*i)++];
	}
	(*cur)->content[j] = '\0';
}
