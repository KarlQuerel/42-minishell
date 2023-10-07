/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: casomarr <casomarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 17:42:36 by carolina          #+#    #+#             */
/*   Updated: 2023/10/07 17:13:36 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

char	*commands(char *line, t_env *env_list)
{
	if (is_cmd_in_line(line, "$") == true)
		line = dollar(line, env_list);
	else if (is_this_command(line, "history") == true)
			history(HISTORY);
	else if (is_this_command(line, "pwd") == true)
			pwd();
	else if (is_cmd_in_line(line, "cd") == true)
		cd();
	else if (is_cmd_in_line(line, "echo") == true)
		echo(line);
	else if (is_cmd_in_line(line, ">") == true || is_cmd_in_line(line, "<") == true)
		redirecters_error(line);
	else if (line[0] == '<' || line[0] == '>' || \
	line[0] == '/' || line[0] == '|' || line[0] == '&' || \
	line[0] == '\'' || line[0] == '\"')
		first_character_error(line);
	return (line);
}

/*Checks if what is written in the command line corresponds to a command*/
bool	is_this_command(char *buffer, char* command)
{
	size_t	i;

	i = 0;
	if (!buffer)
		return (false);
	while (buffer[i])
	{
		if (buffer[i] != command[i])
			return (false);
		i++;
	}
	if (ft_strlen(buffer) == ft_strlen(command) && i == ft_strlen(buffer))
		return (true);
	else
		return (false);
}

/*returns the size of a cmd, key, value or str*/
int	size_of_command(char *command, int len, int type)
{
	int	size;

	size = 0;
	if (type == CMD)
	{
		while (command[len + 1] != ' ' && command[len + 1] != '\0')
		{
			size++;
			len++;
		}
	}
	else if (type == KEY)
	{
		while (command[len] != '=')
		{
			size++;
			len++;
		}
	}
	else if (type == VALUE)
	{
		while (command[len] != '=')
			len++;
		len++;
		while (command[len])
		{
			size++;
			len++;
		}
	}
	else if (type == STR)
	{
		while(command[len])
		{
			if (command[len + 1] == '\0' && (command[len + 1] != '\'' || command[len + 1] != '\"'))
				break;
			size++;
			len++;
		}
	}
	return(size + 1);
}

/*Checks if a given commands is in the command line*/
bool	is_cmd_in_line(char *line, char *cmd)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (!cmd || !line)
		return (false);
	while (line[i] != '\0')
	{
		j = 0;
		while (line[i + j] == cmd[j] && cmd[j] != '\0' && line[i + j] != '\0')
		{
			j++;
			if (cmd[j] == '\0')
				return (true);
		}
		i++;
	}
	return (false);
}

/*Returns the position of the end of the cmd found in the cmd line
Returns 0 in case of error since the end of a cmd can't be at the
beggining of the cmd line*/
int	where_is_cmd_in_line(char *line, char *cmd)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (!cmd || !line)
		return (false);
	while (line[i] != '\0')
	{
		j = 0;
		while (line[i + j] == cmd[j] && cmd[j] != '\0' && line[i + j] != '\0')
		{
			j++;
			if (cmd[j] == '\0')
				return (i + j); // i + j = fin de cmd
		}
		i++;
	}
	return (0);
}