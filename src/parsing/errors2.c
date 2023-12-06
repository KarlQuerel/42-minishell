/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: octonaute <octonaute@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 15:40:58 by octonaute         #+#    #+#             */
/*   Updated: 2023/12/06 19:13:45 by octonaute        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	and_error(char *line)
{
	int	i;

	i = 0;
	while (line[i] != ' ' && line[i])
		i++;
	if (i == 1)
		msg_error_bash(6, "syntax error near unexpected token `&'");
	else
		msg_error_bash(6, "syntax error near unexpected token `&&'");
}

bool	pipe_double_or_eof(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] == '|' && line[i + 1] == '|')
		{
			msg_error_bash(8, "");
			return (false);
		}
		i++;
	}
	if (line[ft_strlen(line) - 1] == '|')
	{
		msg_error_bash(7, "");
		return (false);
	}
	if (line[ft_strlen(line) - 1] == '<' || line[ft_strlen(line) - 1] == '>')
	{
		msg_error_bash(6, "bash: syntax error near unexpected token `newline'");
		return (false);
	}
	return (true);
}
