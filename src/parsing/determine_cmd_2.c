/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   determine_cmd_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/02 23:12:42 by kquerel           #+#    #+#             */
/*   Updated: 2023/12/02 23:14:19 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
	if (outfile(line, start) == true)
		return (OUTFILE);
	if (ft_strncmp(&line[start], "|", 1) == 0)
		return (PIPE);
	while (start < end)
	{
		if (line[start] == ' ')
			return (ARGUMENT);
		start++;
	}
	return (COMMAND);
}
