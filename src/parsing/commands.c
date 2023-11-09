/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: octonaute <octonaute@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 17:42:36 by carolina          #+#    #+#             */
/*   Updated: 2023/11/09 16:57:02 by octonaute        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

/*The first two "if" check if there are errors in the grammar of the command
line and print the associated error. The last function replaces the $ by its
associated value so that the executable receives directly the line completed.*/
bool	line_errors_and_fix(char **line) // pourquoi double pointeur
{
	if (!*line)
		return (NULL);
	// if (redirecters_error(*line) == false)
	// 	return (false);
	if (first_character_error(*line) == false)
		return (false);
	if (pipe_double_or_eof(*line) == false)
		return (false);
	return (true);
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

/* Handles builtins redirections */
void	ft_builtins(t_element *cmd, t_env *env_list/* , t_history *entries */, int option)
{
	if (is_this_command(cmd->content, "pwd") == true && check_next_node_builtin(cmd, NO_OPTIONS))
		pwd(PRINT);
	else if (is_this_command(cmd->content, "history") == true && check_next_node_builtin(cmd, HISTORY))
	{
		if (cmd->next && cmd->next->type != PIPE)
			// history(entries, ft_atoi(cmd->next->content));
			history(FT_HISTORY, ft_atoi(cmd->next->content)); //avec numero
		else
			// history(entries, -1);
			history(FT_HISTORY, -1);
	}
	else if (is_this_command(cmd->content, "cd") == true && check_next_node_builtin(cmd, NO_OPTIONS))
		cd(cmd, env_list);
	else if (is_this_command(cmd->content, "echo") == true && check_next_node_builtin(cmd, ECHO))
		echo(cmd, option);
	else if (is_this_command(cmd->content, "env") == true && check_next_node_builtin(cmd, ENV))
		ft_env(env_list, cmd, 0);
	else if (is_this_command(cmd->content, "export") == true && check_next_node_builtin(cmd, NO_OPTIONS))
		ft_export(cmd, env_list);
	else if (is_this_command(cmd->content, "unset") == true && check_next_node_builtin(cmd, NO_OPTIONS))
			ft_unset(cmd, env_list);
	else if (is_this_command(cmd->content, "$?") == true && check_next_node_builtin(cmd, NO_OPTIONS))
		ft_dollar_question_mark(env_list);
	else if (is_this_command(cmd->content, "exit") == true && check_next_node_builtin(cmd, NO_OPTIONS))
		ft_exit(cmd); // peut etre envoyer t_history *entries -> voir avec caro
}
