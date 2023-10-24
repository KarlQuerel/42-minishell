/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: casomarr <casomarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 17:42:36 by carolina          #+#    #+#             */
/*   Updated: 2023/10/24 17:25:45 by casomarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

/*The first two "if" check if there are errors in the grammar of the command
line and print the associated error. The last function replaces the $ by its
associated value so that the executable receives directly the line completed.*/
char	*line_errors_and_fix(char *line, t_env *env_list)
{
	if (!line)
		return (NULL);
	// fin
	(void)env_list;
	
	if (is_cmd_in_line(line, ">") == true || is_cmd_in_line(line, "<") == true)
		redirecters_error(line);
	else if (line[0] == '<' || line[0] == '>' || \
	line[0] == '/' || line[0] == '|' || line[0] == '&' || \
	line[0] == '\'' || line[0] == '\"')
		first_character_error(line);
	return (line);
}

/*Checks if what is written in the command line corresponds to a command.*/
bool	is_this_command(char *buffer, char* command)
{
	if (!buffer || !command)
		return (false);
	if (ft_strncmp(buffer, command, ft_strlen(buffer)) == 0 && \
	ft_strlen(buffer) == ft_strlen(command))
		return (true);
	else
		return (false);
}

/*Returns the size of a cmd, key, value or str.*/
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

/*Checks if a given commands is in the command line.*/
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

void	commands(t_element *current_cmd, t_env *env_list, t_history *entries)
{
	if (is_this_command(current_cmd->content, "pwd") == true)
		pwd(PRINT);
	else if (is_this_command(current_cmd->content, "history") == true)
		history(entries);
	else if (is_this_command(current_cmd->content, "cd") == true)
		cd(current_cmd, env_list);
	else if (is_this_command(current_cmd->content, "echo") == true)
		echo(current_cmd);
	else if (is_this_command(current_cmd->content, "env") == true)
		ft_env(env_list, current_cmd, 0);
	else if (is_this_command(current_cmd->content, "export") == true)
		ft_export(current_cmd, env_list);
	else if (is_this_command(current_cmd->content, "unset") == true)
		ft_unset(current_cmd, env_list);
	else if (is_this_command(current_cmd->content, "exit") == true)
	{
		//il manque free de new_line
		exit_free(current_cmd, env_list, entries);
		ft_putendl_fd("exit", STDOUT_FILENO);
		exit (1); // voir les codes d'exit (127, 8)
	}
}

