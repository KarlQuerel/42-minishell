/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   erase_spaces.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: casomarr <casomarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 19:51:55 by octonaute         #+#    #+#             */
/*   Updated: 2023/11/17 11:14:11 by casomarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

/*The first condition is meant to skip the spaces in between quotes,
so that they are kept in new_line.*/
void	erase_spaces_loop(char *line, char **new_line, int *i, int *j)
{
	char	*separator;

	if (parsing_str_type(line, (*i)) == STR) //if quotes && quotes can close
	{
		separator = type_of_separator(line, (*i), parsing_str_type(line, (*i)));
		(*new_line)[(*j)++] = line[(*i)++]; //pour copier/coller le separateur
		while (line[(*i)] != separator[0] && line[(*i)])
			(*new_line)[(*j)++] = line[(*i)++];
		(*new_line)[(*j)++] = line[(*i)++]; //pour copier/coller le separateur
	}
	else if ((line[(*i)] == ' ' && line[(*i) + 1] == ' ') || \
	(line[(*i)] == ' ' && line[(*i) + 1] == '\0'))
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
	while (line[i])
		erase_spaces_loop(line, &new_line, &i, &j);
	if (j == 0)
		return (NULL);
	new_line[j] = '\0';
	free(line);
	return (new_line);
}
