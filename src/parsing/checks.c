/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: casomarr <casomarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 17:07:47 by casomarr          #+#    #+#             */
/*   Updated: 2023/10/20 18:19:42 by casomarr         ###   ########.fr       */
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
	if (ft_strncmp(cmd_content, "echo", ft_strlen("echo")) == 0 || \
	ft_strncmp(cmd_content, "cd", ft_strlen("cd")) == 0 || \
	ft_strncmp(cmd_content, "pwd", ft_strlen("pwd")) == 0 || \
	ft_strncmp(cmd_content, "export", ft_strlen("export")) == 0 || \
	ft_strncmp(cmd_content, "unset", ft_strlen("unset")) == 0 || \
	ft_strncmp(cmd_content, "env", ft_strlen("env")) == 0 || \
	/* ft_strncmp(cmd_content, "exit", ft_strlen("exit")) == 0 || \ */
	ft_strncmp(cmd_content, "history", ft_strlen("history")) == 0)
		return (true);
	else
		return (false);
}