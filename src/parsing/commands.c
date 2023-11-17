/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 17:42:36 by carolina          #+#    #+#             */
/*   Updated: 2023/11/17 17:51:17 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

/*Checks if what is written in the command line corresponds to a command.*/
bool	is_cmd(char *buffer, char *command)
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
	return (size + 1);
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
void	ft_builtins(t_element *cmd, t_env **env_list, t_pipe *exec, int option)
{
	if (is_cmd(cmd->content, "pwd") == true && check_next(cmd, NONE))
		pwd(PRINT);
	else if (is_cmd(cmd->content, "history") == true && check_next(cmd, HISTORY))
	{
		if (cmd->next && cmd->next->type != PIPE)
			history(FT_HISTORY, ft_atoi(cmd->next->content)); //avec numero en option
		else
			history(FT_HISTORY, -1); //sans option (print totalité de history)
	}
	else if (is_cmd(cmd->content, "cd") == true && check_next(cmd, NONE))
		cd(cmd, *env_list);
	else if (is_cmd(cmd->content, "echo") == true && check_next(cmd, ECHO))
		echo(cmd, option);
	else if (is_cmd(cmd->content, "env") == true && check_next(cmd, ENV))
		ft_env(*env_list, cmd, 0);
	else if (is_cmd(cmd->content, "export") == true && check_next(cmd, NONE))
		ft_export(cmd, env_list);
	else if (is_cmd(cmd->content, "unset") == true && check_next(cmd, NONE))
		ft_unset(cmd, env_list);
	else if (is_cmd(cmd->content, "$?") == true && check_next(cmd, NONE))
		ft_dollar_question_mark();
	else if (is_cmd(cmd->content, "exit") == true && check_next(cmd, NONE))
		ft_exit(cmd, env_list, exec);
}
