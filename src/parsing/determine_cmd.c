/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   determine_cmd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 20:35:37 by casomarr          #+#    #+#             */
/*   Updated: 2023/12/02 23:13:51 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	option(char *line, size_t end, size_t start)
{
	if ((line[start] == '-' && ft_isalpha(line[start + 1]) == 1) || \
	(line[end] >= 4 && line[start] == '-' && (line[start + 1] == '\'' || \
	line[start + 1] == '\"') && ft_isalpha(line[start + 2]) == 1 && \
	(line[end - 1] == '\'' || line[end - 1] == '\"')))
		return (true);
	return (false);
}

bool	heredoc(char *line, size_t start)
{
	if ((start >= 3 && (line[start - 1] == ' ' && line[start - 2] == '<' && \
	line[start - 3] == '<')) || (start >= 2 && (line[start - 1] == '<' && \
	line[start - 2] == '<')) || (start >= 3 && ((line[start - 1] == '\"' || \
	line[start - 1] == '\'') && line[start - 2] == '<' && line[start - 3] \
	== '<')) || (start >= 4 && ((line[start - 1] == '\"' || line[start - 1] \
	== '\'') && line[start - 2] == ' ' && line[start - 3] == '<' && \
	line[start - 4] == '<')))
		return (true);
	return (false);
}

bool	infile(char *line, size_t start)
{
	if ((start >= 2 && (line[start - 1] == ' ' && line[start - 2] == '<')) || \
	(start >= 1 && line[start - 1] == '<') || (start >= 2 && ((line[start - 1] \
	== '\"' || line[start - 1] == '\'') && line[start - 2] == '<')) || \
	(start >= 3 && ((line[start - 1] == '\"' || line[start - 1] == '\'') && \
	line[start - 2] == ' ' && line[start - 3] == '<')))
		return (true);
	return (false);
}

bool	outfile_append(char *line, size_t start)
{
	if ((start >= 3 && (line[start - 1] == ' ' && line[start - 2] == '>' && \
	line[start - 3] == '>')) || (start >= 2 && (line[start - 1] == '>' && \
	line[start - 2] == '>')) || (start >= 3 && ((line[start - 1] == '\"' || \
	line[start - 1] == '\'') && line[start - 2] == '>' && line[start - 3] \
	== '>')) || (start >= 4 && ((line[start - 1] == '\"' || line[start - 1] \
	== '\'') && line[start - 2] == ' ' && line[start - 3] == '>' && \
	line[start - 4] == '>')))
		return (true);
	return (false);
}

bool	outfile(char *line, size_t start)
{
	if ((start >= 2 && (line[start - 1] == ' ' && line[start - 2] == '>')) || \
	(start >= 1 && line[start - 1] == '>') || (start >= 2 && ((line[start - 1] \
	== '\"' || line[start - 1] == '\'') && line[start - 2] == '>')) || \
	(start >= 3 && ((line[start - 1] == '\"' || line[start - 1] == '\'') && \
	line[start - 2] == ' ' && line[start - 3] == '>')))
		return (true);
	return (false);
}
