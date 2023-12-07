/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   erase_spaces.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 19:51:55 by octonaute         #+#    #+#             */
/*   Updated: 2023/12/07 14:33:44 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	erase_spaces_loop(char *line, char **new_line, int *i, int *j)
{
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
	char	*new_line_a;
	char	*new_line;
	int		i;
	int		j;

	i = 0;
	j = 0;
	new_line_a = erase_empty_strings(line);
	new_line = erase_spaces_malloc(new_line_a);
	if (!new_line)
		return (NULL);
	while (new_line_a[i] == ' ')
		i++;
	while (new_line_a[i])
		erase_spaces_loop(new_line_a, &new_line, &i, &j);
	if (j == 0)
	{
		free(new_line_a);
		free(new_line);
		return (NULL);
	}
	new_line[j] = '\0';
	free(new_line_a);
	return (new_line);
}

char	*erase_empty_strings(char *line)
{
	char	*new_line;
	int		i;
	int		j;

	i = 0;
	j = 0;
	new_line = ft_calloc(ft_strlen(line) + 1, sizeof(char));
	if (!new_line)
		return (NULL);
	while (line[i])
		erase_empty_strings_loop(line, &new_line, &i, &j);
	new_line[j] = '\0';
	return (new_line);
}

void	erase_empty_strings_loop(char *line, char **new_line, int *i, int *j)
{
	if (line[(*i)] == '\'')
	{
		if (if_consec_quotes(line, i) == true)
			return ;
		if (line[(*i)])
			(*new_line)[(*j)++] = line[(*i)++];
		while (line[(*i)] && line[(*i)] != '\'')
			(*new_line)[(*j)++] = line[(*i)++];
		(*new_line)[(*j)++] = line[(*i)++];
	}
	else if (line[(*i)] == '\"')
	{
		if (if_consec_quotes(line, i) == true)
			return ;
		if (line[(*i)])
			(*new_line)[(*j)++] = line[(*i)++];
		while (line[(*i)] && line[(*i)] != '\"')
			(*new_line)[(*j)++] = line[(*i)++];
		(*new_line)[(*j)++] = line[(*i)++];
	}
	else
		(*new_line)[(*j)++] = line[(*i)++];
}

bool	if_consec_quotes(char *line, int *i)
{
	if (line[(*i)] == '\'' && line[(*i) + 1] == '\'')
	{
		(*i) += 2;
		return (true);
	}
	if (line[(*i)] == '\"' && line[(*i) + 1] == '\"')
	{
		(*i) += 2;
		return (true);
	}
	return (false);
}
