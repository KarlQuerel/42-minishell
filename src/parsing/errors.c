/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: casomarr <casomarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 12:39:52 by casomarr          #+#    #+#             */
/*   Updated: 2023/11/28 20:28:11 by casomarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*The first two "if" check if there are errors in the grammar of the command
line and print the associated error. The last function replaces the $ by its
associated value so that the executable receives directly the line completed.*/
bool	line_errors_and_fix(char *line)
{
	if (!line)
		return (NULL);
	if (redirecters_error(line) == false)
		return (false);
	if (first_character_error(line) == false)
		return (false);
	if (pipe_double_or_eof(line) == false)
		return (false);
	return (true);
}

bool	first_character_error(char *line)
{
	bool	status;

	status = false;
	if (line[0] == '|')
		pipe_error(line);
	else if (line[0] == '&')
		and_error(line);
	else
		status = true;
	return (status);
}

bool	redirecters_error(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if ((line[i] == '<' || line[i] == '>') && \
		(line[i + 1] == '<' || line[i + 1] == '>') && \
		(line[i + 2] == '<' || line[i + 2] == '>'))
		{
			printf("KARL MESSAGE ERREUR\n");
			return (false);
		}
		i++;
	}
	return (true);
}

void	pipe_error(char *line)
{
	int	i;

	i = 0;
	while (line[i] != ' ' && line[i])
		i++;
	if (i == 1)
		printf("bash: syntax error near unexpected token `|'\n");
	else
		printf("bash: syntax error near unexpected token `||'\n");
}
