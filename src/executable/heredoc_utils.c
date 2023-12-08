/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/02 19:30:11 by kquerel           #+#    #+#             */
/*   Updated: 2023/12/08 01:00:31 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* Free and dups heredoc - Norm */
void	free_dup_heredoc(char *words, int fd, int fd_heredoc)
{
	free(words);
	words = NULL;
	close(fd);
	dup2(fd_heredoc, STDIN_FILENO);
	close(fd_heredoc);
}

/* Minishell's backbone */
char	*ft_palbon(t_element *cmd)
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
