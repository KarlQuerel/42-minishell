/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: casomarr <casomarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 20:44:11 by kquerel           #+#    #+#             */
/*   Updated: 2023/12/02 13:01:20 by casomarr         ###   ########.fr       */
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
	fd_heredoc = dup(STDIN_FILENO);
	while (1)
	{
		words = readline("> ");
		if (g_location == QUIT_HEREDOC)
		{
			add_exit_status_in_env(&env, 130);
			break;
		}
		//if (ft_strncmp(words, cmd->content, ft_strlen(words)) == 0 && ft_strlen(words) == ft_strlen(cmd->content))
		if (compare(words, cmd->content) == true)
		{
			add_exit_status_in_env(&env, 0);
			break;
		}
		if (words && *words)
			ft_putendl_fd(words, fd);
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
	close(fd);
	dup2(fd_heredoc, STDIN_FILENO);
	close(fd_heredoc);
	return (true);
}

/* Creates heredoc */
char	*create_heredoc(char *name, int i, int *fd)
{
	name = ft_strjoin_free_s2("tmp_file", ft_itoa(i));
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

/* str_join_free_s2 */
char	*ft_strjoin_free_s2(char *s1, char *s2)
{
	int		i;
	int		j;
	int		ft_strlen_total;
	char	*new_str;

	if (s1 == NULL || s2 == NULL)
		return (NULL);
	ft_strlen_total = ft_strlen(s1) + ft_strlen(s2);
	new_str = malloc((sizeof(char)) * (ft_strlen_total + 1));
	if (new_str == NULL)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		new_str[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j])
		new_str[i++] = s2[j++];
	new_str[i] = '\0';
	free(s2);
	return (new_str);
}
