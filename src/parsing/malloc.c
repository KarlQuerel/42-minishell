/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 17:08:27 by casomarr          #+#    #+#             */
/*   Updated: 2023/12/07 14:13:20 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*The first two conditions are here to avoid counting the spaces between
quotes, since those won't be deleted.*/
int	count_spaces(char *line)
{
	int	i;
	int	spaces;

	i = 0;
	spaces = 0;
	while (line[i])
	{
		if (line[i] == '\'' && quotes_can_close(line, i) == true)
		{
			i++;
			while (line[i] != '\'')
				i++;
		}
		if (line[i] == '\"' && quotes_can_close(line, i) == true)
		{
			i++;
			while (line[i] != '\"')
				i++;
		}
		if (line[i] == ' ' && (line[i + 1] == ' ' || line[i + 1] == '\0'))
			spaces++;
		i++;
	}
	return (spaces);
}

/*Malloc for the new command line that replaces the original
command line after all the extra spaces are deleted.*/
char	*erase_spaces_malloc(char *line)
{
	char	*new_line;
	int		spaces;

	spaces = count_spaces(line);
	new_line = ft_calloc(ft_strlen(line) - spaces + 2, sizeof(char));
	if (!new_line)
	{
		msg_error(1, "");
		return (NULL);
	}
	return (new_line);
}

/*Malloc for the joinstr found in the ft_joinstr_minishell function.*/
char	*joinstr_minishell_malloc(char *line, int len, char *str, char type)
{
	char	*new_str;

	if (type == '\'' || type == '\"')
	{
		if (str == NULL)
			new_str = ft_calloc(size_of_command(line, len, STR) \
			+ 1, sizeof(char));
		else
			new_str = ft_calloc((ft_strlen(str) + \
			size_of_command(line, len, STR) + 1), sizeof(char));
	}
	else
	{
		if (str == NULL)
			new_str = ft_calloc(size_of_command(line, len, CMD) \
			+ 1, sizeof(char));
		else
			new_str = ft_calloc((ft_strlen(str) \
			+ size_of_command(line, len, CMD) + 1), sizeof(char));
	}
	if (!new_str)
		return (NULL);
	return (new_str);
}
