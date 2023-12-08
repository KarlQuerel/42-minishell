/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 17:42:36 by carolina          #+#    #+#             */
/*   Updated: 2023/12/08 00:50:50 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*Checks if what is written in the command line corresponds to a command.*/
bool	is_cmd(char *buffer, char *command)
{
	if (!buffer || !command)
		return (false);
	if (compare(buffer, command) == true)
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

void	ft_builtins_history(t_element *cmd)
{
	if (cmd->next && cmd->next->type != PIPE)
		history(FT_HISTORY, ft_atoi(cmd->next->content));
	else
		history(FT_HISTORY, -1);
}

/* Handles builtins redirections.
For the history builtin : the first If is to handle an option. If
no option was given, the option is set to -1, so that the totality
of history is printed.*/
void	ft_builtins(t_element *cmd, t_env **env_list, t_pipe *exec)
{
	if (is_cmd(cmd->content, "$?") == false)
		add_exit_status_in_env(env_list, 0);
	if (is_cmd(cmd->content, "pwd") == true && check_next(cmd, NONE))
		pwd(PRINT);
	else if (is_cmd(cmd->content, "history") == true && \
	check_next(cmd, HISTORY))
		ft_builtins_history(cmd);
	else if (is_cmd(cmd->content, "cd") == true && check_next(cmd, CD))
		cd(cmd, *env_list);
	else if (is_cmd(cmd->content, "echo") == true && check_next(cmd, ECHO))
		echo(cmd);
	else if (is_cmd(cmd->content, "env") == true && check_next(cmd, ENV))
		ft_env(*env_list, cmd, 0);
	else if (is_cmd(cmd->content, "export") == true && check_next(cmd, NONE))
		ft_export(cmd, env_list);
	else if (is_cmd(cmd->content, "unset") == true && check_next(cmd, NONE))
		ft_unset(cmd, env_list);
	else if (is_cmd(cmd->content, "exit") == true && check_next(cmd, NONE))
		ft_exit(cmd, env_list, exec);
	else
		add_exit_status_in_env(env_list, 127);
}
