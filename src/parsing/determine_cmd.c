/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   determine_cmd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: casomarr <casomarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 20:35:37 by casomarr          #+#    #+#             */
/*   Updated: 2023/11/28 20:55:58 by casomarr         ###   ########.fr       */
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

/*4 options : 
<< HD
<<HD
<<"HD" or <<'HD'
<< "HD" or << 'HD'*/
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

/*Determines the type of a given cmd for the parsing function.*/
int	determine_command_type(char *line, size_t end, size_t start)
{
	if (line[start] == '\'' || line[start] == '\"')
		start = start + 1;
	if (option(line, end, start) == true)
		return (OPTION);
	if (heredoc(line, start) == true)
		return (HEREDOC);
	if (infile(line, start) == true)
		return (INFILE);
	if (outfile_append(line, start) == true)
		return (OUTFILE_APPEND);
	if(outfile(line, start) == true)
		return (OUTFILE);
	if (ft_strncmp(&line[start], "|", 1) == 0)
		return (PIPE);
	while(start < end)
	{
		if (line[start] == ' ')
			return (ARGUMENT);
		start++;
	}
	return (COMMAND);
}

/*Determines the type of a given cmd for the parsing function.*/
// int	determine_command_type(char *line, size_t end, size_t start)
// {
// 	if (line[start] == '\'' || line[start] == '\"')
// 		start = start + 1;
// 	if ((line[start] == '-' && ft_isalpha(line[start + 1]) == 1) || \
// 	(line[end] >= 4 && line[start] == '-' && (line[start + 1] == '\'' || \
// 	line[start + 1] == '\"') && ft_isalpha(line[start + 2]) == 1 && \
// 	(line[end - 1] == '\'' || line[end - 1] == '\"')))
// 		return (OPTION);
// 	if ((start >= 3 && (line[start - 1] == ' ' && line[start - 2] == '<' && line[start - 3] == '<'/*  && line[start - 4] == ' ' */)/*  && nothing_before(start, 3, line) == true */) || \
// 	(start >= 2 && (line[start - 1] == '<' && line[start - 2] == '<'/*  && line[start - 3] == ' ' */)/*  && nothing_before(start, 4, line) == true */) || \
// 	(start >= 3 && ((line[start - 1] == '\"' || line[start - 1] == '\'') && line[start - 2] == '<' && line[start - 3] == '<'/*  && line[start - 4] == ' ' */)/*  && nothing_before(start, 3, line) == true */) || \
// 	(start >= 4 && ((line[start - 1] == '\"' || line[start - 1] == '\'') && line[start - 2] == ' ' && line[start - 3] == '<' && line[start - 4] == '<'/*  && line[start - 5] == ' ' */)/*  && nothing_before(start, 4, line) == true */))
// 		return (HEREDOC);
// 	if ((start >= 2 && (line[start - 1] == ' ' && line[start - 2] == '<'/*  && line[start - 3] == ' ' */)/*  && nothing_before(start, 2, line) == true */) || \
// 	(start >= 1 && line[start - 1] == '<'/*  && line[start - 2] == ' ' *//*  && nothing_before(start, 1, line) == true */) || \
// 	(start >= 2 && ((line[start - 1] == '\"' || line[start - 1] == '\'') && line[start - 2] == '<'/*  && line[start - 3] == ' ' */)/*  && nothing_before(start, 2, line) == true */) || \
// 	(start >= 3 && ((line[start - 1] == '\"' || line[start - 1] == '\'') && line[start - 2] == ' ' && line[start - 3] == '<'/*  && line[start - 4] == ' ' */)/*  && nothing_before(start, 3, line) == true */))
// 		return (INFILE);
// 	if ((start >= 3 && (line[start - 1] == ' ' && line[start - 2] == '>' && line[start - 3] == '>'/*  && line[start - 4] == ' ' */)/*  && nothing_before(start, 3, line) == true */) || \
// 	(start >= 2 && (line[start - 1] == '>' && line[start - 2] == '>'/*  && line[start - 3] == ' ' */)/*  && nothing_before(start, 2, line) == true */) || \
// 	(start >= 3 && ((line[start - 1] == '\"' || line[start - 1] == '\'') && line[start - 2] == '>' && line[start - 3] == '>'/*  && line[start - 4] == ' ' */)/*  && nothing_before(start, 3, line) == true */) || \
// 	(start >= 4 && ((line[start - 1] == '\"' || line[start - 1] == '\'') && line[start - 2] == ' ' && line[start - 3] == '>' && line[start - 4] == '>'/*  && line[start - 5] == ' ' */)/*  && nothing_before(start, 4, line) == true */))
// 		return (OUTFILE_APPEND);
// 	if ((start >= 2 && (line[start - 1] == ' ' && line[start - 2] == '>'/*  && line[start - 3] == ' ' */)/*  && nothing_before(start, 2, line) == true */) || \
// 	(start >= 1 && line[start - 1] == '>'/*  && line[start - 1] == ' ' *//*  && nothing_before(start, 1, line) == true */) || \
// 	(start >= 2 && ((line[start - 1] == '\"' || line[start - 1] == '\'') && line[start - 2] == '>'/*  && line[start - 3] == ' ' */)/*  && nothing_before(start, 2, line) == true */) || \
// 	(start >= 3 && ((line[start - 1] == '\"' || line[start - 1] == '\'') && line[start - 2] == ' ' && line[start - 3] == '>'/*  && line[start - 4] == ' ' */)/*  && nothing_before(start, 3, line) == true */))
// 		return (OUTFILE);
// 	if (ft_strncmp(&line[start], "|", 1) == 0)
// 		return (PIPE);
// 	while(start < end)
// 	{
// 		if (line[start] == ' ')
// 			return (ARGUMENT);
// 		start++;
// 	}
// 	return (COMMAND);
// }