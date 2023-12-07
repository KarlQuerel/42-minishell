/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_and_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 21:18:09 by kquerel           #+#    #+#             */
/*   Updated: 2023/12/07 22:21:19 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	set_struct_null(t_env **env, t_element **cmd, t_pipe **exec)
{
	*env = NULL;
	*cmd = NULL;
	*exec = NULL;
}

/* Inits the structure */
t_pipe	*init_struct(t_pipe *exec, t_env **env_list, char **env)
{
	*env_list = put_env_in_list(env);
	using_history();
	exec = ft_calloc(1, sizeof(t_pipe));
	if (!exec)
	{
		msg_error(1, "");
		exit(EXIT_FAILURE);
	}
	exec->prompt = NULL;
	return (exec);
}

int	ft_mini_prompt(t_env **env_list, char **path, t_pipe *exec, char **line)
{
	g_location = IN_PROMPT;
	if (set_signals() == EXIT_FAILURE)
	{
		ft_putendl_fd("FAILURE", STDERR_FILENO);
		return (1);
	}
	if (is_key_in_env(*env_list, "EXIT_STATUS") == false)
		add_exit_status_in_env(env_list, 0);
	*path = ft_prompt(*env_list, NO_PRINT);
	exec->prompt = ft_strjoin(*path, "$ ");
	if (compare(*path, "/") == false && compare(*path, "") == false)
		free(*path);
	*line = readline(exec->prompt);
	if (g_location == QUIT)
		add_exit_status_in_env(env_list, 130);
	add_history(*line);
	return (0);
}

void	line_null(char *line, t_env **env_list, t_pipe *exec)
{
	if (g_location == IN_PROMPT)
	{
		ft_putendl_fd("exit", STDERR_FILENO);
		ctrld_free(line, exec->prompt, *env_list, exec);
	}
	else if (g_location == IN_COMMAND)
	{
		ft_putchar_fd('\n', STDERR_FILENO);
		rl_reset_line_state();
	}
}

void	line_not_null(char **line, t_element *cmd_list, t_env **env_list, \
t_pipe *exec)
{
	if (is_in_line(*line, "\\") == true || quotes_can_close(*line, 0) == false)
	{
		ft_putendl_fd("Syntax error", STDERR_FILENO);
		return ;
	}
	*line = erase_spaces(*line);
	if ((*line && *line[0] == '\0') || *line == NULL)
		return ;
	if (line_errors_and_fix(*line) == true)
	{
		cmd_list = parsing(*line, *env_list);
		if (cmd_list != NULL)
		{
			exec->line = *line;
			ft_execute(cmd_list, env_list, exec);
			free_cmd_arr(exec);
			ft_unlink(cmd_list);
			if (compare(*line, "$") == false)
			{
				if (free_cmd_list_parent(cmd_list) == 1)
					cmd_list = NULL;
			}
		}
	}
}
