/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 20:44:11 by kquerel           #+#    #+#             */
/*   Updated: 2023/12/02 19:49:27 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* Handles heredoc behavior */
bool	ft_heredoc(t_element *cmd, t_env *env)
{
	int			fd;
	int			fd_heredoc;
	char		*words;
	char		*file_name;
	static int	iteration_nb = 1;

	iteration_nb++;
	g_location = IN_HEREDOC;
	set_signals();
	file_name = create_heredoc(cmd->content, iteration_nb, &fd);
	if (!file_name)
		return (false);
	cmd->hd_filename = ft_strdup(file_name);
	free(file_name);
	words = NULL;
	fd_heredoc = dup(STDIN_FILENO);
	while (1)
	{
		if (!heredoc_loop(cmd, env, words, fd))
			break ;
	}
	free_dup_heredoc(words, fd, fd_heredoc);
	return (true);
}

/* Fills heredoc as long as heredoc_loop returns (1) */
int	heredoc_loop(t_element *cmd, t_env *env, char *words, int fd)
{
	words = readline("> ");
	if (g_location == QUIT_HEREDOC)
		return (add_exit_status_in_env(&env, 130), 0);
	if (compare(words, cmd->content) == true)
		return (add_exit_status_in_env(&env, 0), 0);
	if (words && *words)
		ft_putendl_fd(words, fd);
	if (words == NULL)
	{
		ft_putstr_fd(HD_ERR, STDERR_FILENO);
		ft_putstr_fd(cmd->content, STDERR_FILENO);
		ft_putendl_fd("')", STDERR_FILENO);
		add_exit_status_in_env(&env, 0);
		return (0);
	}
	free(words);
	words = NULL;
	return (1);
}
