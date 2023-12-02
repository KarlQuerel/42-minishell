/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 12:39:52 by casomarr          #+#    #+#             */
/*   Updated: 2023/12/02 13:22:55 by kquerel          ###   ########.fr       */
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
			msg_error_bash(6, "syntax error near unexpected token `>' or `<'");
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
		msg_error_bash(7, "");
	else
		msg_error_bash(8, "");
}
