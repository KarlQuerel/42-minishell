/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 17:07:47 by casomarr          #+#    #+#             */
/*   Updated: 2023/10/13 14:06:51 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

bool	check_commands_grammar(char *commands)
{
	// if (commands == NULL)
	// 	return (true);
	if (commands[0] == '|') // rajouter des conditions
	{
		printf("Error\n");
		return (false);
	}
	return (true);
}

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