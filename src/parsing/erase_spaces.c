/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   erase_spaces.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: octonaute <octonaute@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 19:51:55 by octonaute         #+#    #+#             */
/*   Updated: 2023/12/06 20:00:06 by octonaute        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*The first condition checks if there are quotes and quotes
can close. It is meant to skip the spaces in between quotes,
so that they are kept in new_line. It keeps the separators.*/
/* void	erase_spaces_loop(char *line, char **new_line, int *i, int *j)
{
	char	*separator;

	if (parsing_str_type(line, (*i)) == STR)
	{
		separator = type_of_separator(line, (*i), parsing_str_type(line, (*i)));
		(*new_line)[(*j)++] = line[(*i)++];
		while (line[(*i)] != separator[0] && line[(*i)])
			(*new_line)[(*j)++] = line[(*i)++];
		(*new_line)[(*j)++] = line[(*i)++];
		free(separator);
	}
	else if ((line[(*i)] == ' ' && line[(*i) + 1] == ' ') || \
	(line[(*i)] == ' ' && line[(*i) + 1] == '\0'))
		(*i) += 1;
	else
		(*new_line)[(*j)++] = line[(*i)++];
} */

void	erase_spaces_loop(char *line, char **new_line, int *i, int *j)
{
	char	*separator;

	if (line[(*i)] == '\'')
	{
		(*new_line)[(*j)++] = line[(*i)++];
		while (line[(*i)] && line[(*i)] != '\'')
			(*new_line)[(*j)++] = line[(*i)++];
		(*new_line)[(*j)++] = line[(*i)++];
	}
	else if (line[(*i)] == '\"')
	{
		(*new_line)[(*j)++] = line[(*i)++];
		while (line[(*i)] && line[(*i)] != '\"')
			(*new_line)[(*j)++] = line[(*i)++];
		(*new_line)[(*j)++] = line[(*i)++];
	}
	else if ((line[(*i)] == ' ' && line[(*i) + 1] == ' ') || \
	(line[(*i)] == ' ' && line[(*i) + 1] == '\0') || \
	(line[(*i)] >= 9 && line[(*i)] <= 13))
		(*i) += 1;
	else
		(*new_line)[(*j)++] = line[(*i)++];
}

/*Returns a new_line that is the same as the original command line
except all the superfulous spaces have been erased.*/
char	*erase_spaces(char *line)
{
	char	*new_line;
	int		i;
	int		j;

	i = 0;
	j = 0;
	new_line = erase_spaces_malloc(line);
	if (!new_line)
		return (NULL);
	while (line[i] == ' ')
		i++;
	while (line[i])
		erase_spaces_loop(line, &new_line, &i, &j);
	if (j == 0)
	{
		free(line);
		free(new_line);
		return (NULL);
	}
	new_line[j] = '\0';
	free(line);
	new_line = erase_empty_strings(new_line);
	return (new_line);
}

char	*erase_empty_strings(char *line)
{
	char	*new_line;
	int		i;
	int		j;

	i = 0;
	j = 0;
	new_line = empty_strings_malloc(line);
	if (!new_line)
		return (NULL);
	while (line[i]) //test "''"'""
	{
		if (line[i] != '\'' && line[i] != '\"')
			break;
		i++;
	}
	if (line[i] == '\0')
	{
		free(line);
		new_line[j] = '\0';
		return (new_line);
	}
	i = 0;
	while (line[i])
	{
		if (line[i] == '\'' && line[i + 1] == '\'')
			i += 2;
		else if (line[i] == '\"' && line[i + 1] == '\"')
			i += 2;
		else
			new_line[j++] = line[i++];
	}
	new_line[j] = '\0';
	free (line);
	return (new_line);
}
