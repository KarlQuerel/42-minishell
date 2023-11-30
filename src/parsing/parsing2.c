/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: casomarr <casomarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 19:55:56 by octonaute         #+#    #+#             */
/*   Updated: 2023/11/30 20:41:57 by casomarr         ###   ########.fr       */
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

void	parsing_fill_content(t_element **current_cmd, char *line, int *i, \
char *separator)
{
	int	j;
	int	x;

	j = 0;
	if (separator[0] == '\'' || separator[0] == '\"')
		(*i)++;
	while (line[(*i)])
	{
		x = 0;
		while (separator[x])
		{
			if (line[(*i)] == separator[x])
			{
				if (separator[x] == '|' && j == 0)
					(*current_cmd)->content[j++] = line[(*i)++];
				(*current_cmd)->content[j] = '\0';
				if (separator[x] == '\'' || separator[x] == '\"')
					(*i)++;
				return ;
			}
			x++;
		}
		if (line[(*i)] == '\\') /*EST-CE QUE CETTE CONDITION NE DEVRAIT PAS
		JUSTE ETRE DANS ECHO?? IL Y A PEUT ETRE DES COMMANDES DANS LESQUELS ON VEUT
		GARDER LE SLASH? Cette condition est à la base faite pour le test :
		echo hola\ncaro -> doit donner holancaro*/
			(*i)++;
		(*current_cmd)->content[j++] = line[(*i)++];
	}
	(*current_cmd)->content[j] = '\0';
	if (separator[0] == '\'' || separator[0] == '\"')
		(*i)++;
}

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
