/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: casomarr <casomarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 17:07:47 by casomarr          #+#    #+#             */
/*   Updated: 2023/10/17 16:57:10 by casomarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

/*When a quote is found in the command line, this function
checks if there is another quote of the same type further in
the command line. If yes, it returns true.*/
bool	quotes_can_close(char *line)
{
	size_t		i;
	char	type;
	
	i = 0;
	while (line[i])
	{
		if (line[i] == '\'' || line[i] == '\"')
		{
			if (line[i] == '\'')
				type = '\'';
			else if (line[i] == '\"')
				type = '\"';
			i++;
			if (line[i] == '\0') //new quote at end of line
				return (false);
			if (line[i] != type)
			{
				while(line[i] && line[i] != type)
					i++;
				if (line[i] == '\0')
					return (false);
			}
		}
			i++;
	}
	return (true);
}

/*Checks if the cmd sent to this function is a builtin.*/
bool		is_builtin(char *cmd_content)
{
	if (strncmp(cmd_content, "echo", ft_strlen("echo")) == 0 || \
	strncmp(cmd_content, "cd", ft_strlen("cd")) == 0 || \
	strncmp(cmd_content, "pwd", ft_strlen("pwd")) == 0 || \
	strncmp(cmd_content, "export", ft_strlen("export")) == 0 || \
	strncmp(cmd_content, "unset", ft_strlen("unset")) == 0 || \
	strncmp(cmd_content, "env", ft_strlen("env")) == 0) //j'ai pas mis exit, ce qui se pase qd on ecrit exit apres une pipe sur bash n est pas clair
		return (true);
	else
		return (false);
}