/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 20:44:11 by kquerel           #+#    #+#             */
/*   Updated: 2023/11/29 20:56:01 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* Handles heredoc behavior */
bool	ft_heredoc(t_element *cmd, t_env *env)
{
	int			fd;
	char		*words;
	char		*file_name;
	static int	iteration_nb = 1;
	
	iteration_nb++;
	g_location = IN_HEREDOC;
	set_signals();
	file_name = create_heredoc(cmd->content, iteration_nb, &fd);
	if (!file_name)
		return (false);
	cmd->hd_filename = ft_strdup(file_name); // peut etre leak

	int test_fd = dup(STDIN_FILENO);

	// env->fd_heredoc = dup(STDIN_FILENO);
	while (1)
	{
		words = readline("> ");
		if (ft_strncmp(words, cmd->content, ft_strlen(words)) == 0 && ft_strlen(words) == ft_strlen(cmd->content))
		{
			add_exit_status_in_env(&env, 0);
			break;
		}
		if (words && *words)
			ft_putendl_fd(words, fd);
		if (g_location == QUIT_HEREDOC)
		{
			add_exit_status_in_env(&env, 130);
			break;
		}
		if (words == NULL)
		{
			ft_putstr_fd("bash: warning: here-document delimited by end-of-file (wanted `", STDERR_FILENO);
			ft_putstr_fd(cmd->content, STDERR_FILENO);
			ft_putendl_fd("')", STDERR_FILENO);
			add_exit_status_in_env(&env, 0);
			break;
		}
		free(words);
		words = NULL;
	}
	free(words);
	words = NULL;
	// dup2(env->fd_heredoc, STDIN_FILENO);
	dup2(test_fd, STDIN_FILENO);
	close(fd);
	// close(env->fd_heredoc);
	// unlink(file_name);
	return (true);
}

/* Creates heredoc */
char	*create_heredoc(char *name, int i, int *fd)
{
	name = ft_strjoin("tmp_file", ft_itoa(i));
	if (!name)
		return (NULL);
	*fd = open(name, O_WRONLY | O_CREAT | O_EXCL, 0777);
	if (*fd < 0)
	{
		perror("bash");
		return (NULL);
	}
	if (access(name, F_OK))
	{
		free(name);
		return (NULL);
	}
	return (name);
}

/* Minishell's backbone */
char	*ft_alban(t_element *cmd)
{
	char	*ret;

	ret = NULL;
	while (cmd->prev && cmd->prev->type != PIPE)
	{
		if (cmd->type == HEREDOC)
		{
			ret = cmd->hd_filename;
			break ;
		}
		cmd = cmd->prev;
	}
	while (cmd && cmd->type != PIPE)
	{
		if (cmd->type == HEREDOC)
			ret = cmd->hd_filename;
		if (!cmd->next)
			break ;
		cmd = cmd->next;
	}
	return (ret);
}