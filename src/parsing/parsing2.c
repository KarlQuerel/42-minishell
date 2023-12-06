/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karl <karl@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 19:55:56 by octonaute         #+#    #+#             */
/*   Updated: 2023/12/06 19:20:59 by karl             ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

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
	//typestr = parsing_str_type(line, (*i));
	typestr = CMD; //tjrs, alors que avant STR aussi
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

void	fill_content_loop(t_element **cur, char *line, int *i/*,  \
char *sep */) //je peux rajouter un parametre pour norme!!
{
	int		x;
	int		j;
	int		closing_quote;

	j = 0;
	closing_quote = -1;
	while (line[(*i)] && line[(*i)] != ' ' && line[(*i)] != '|' && \
	line[(*i)] != '<' && line[(*i)] != '>')
	{
		closing_quote = skip_first_quote(line, i); //checks if we are on a quote
		if (closing_quote != -1) //if yes
		{
			if (line[(*i) - 1] == '\'')
				(*cur)->change = false;
			while (line[(*i)] && (*i) < closing_quote)
				(*cur)->content[j++] = line[(*i)++];
			(*i)++; //skip last quote
		}
		else
			(*cur)->content[j++] = line[(*i)++];
	}
	(*cur)->content[j] = '\0';
}
