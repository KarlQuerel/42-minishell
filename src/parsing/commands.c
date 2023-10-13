/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: casomarr <casomarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 17:42:36 by carolina          #+#    #+#             */
/*   Updated: 2023/10/13 16:43:48 by casomarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

char	*commands(char *line, t_env *env_list, char *home_path)
{
	/*a la base j avais fait " cd " etc donc avec espace apres le 
	nom du builtin pour eviter les erreur du type "holacdhey" et 
	que ca compte comme cd mais pb : cd tout seul sans espace avant
	car premiere cmd de la ligne et sans espace apres car c est a 
	moi de remplacer par le home_path alors ca ne marche pas*/
	if (is_cmd_in_line(line, "$") == true)
		line = dollar(line, env_list);
	else if (is_this_command(line, "history") == true)
		history(HISTORY);
	else if (is_this_command(line, "pwd") == true)
		pwd(PRINT);
	else if (is_cmd_in_line(line, "cd") == true)
		line = cd(line, home_path);
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
		size = cmd_type(command, len);
	else if (type == KEY || type == VALUE)
		size = key_and_value_type(command, len, type);
	else if (type == STR)
		size = str_type(command, len);
	return(size + 1); //car pour mallocs besoin d'un +1 pour \0
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