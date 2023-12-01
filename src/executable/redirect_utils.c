/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: casomarr <casomarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 18:13:47 by kquerel           #+#    #+#             */
/*   Updated: 2023/12/01 14:41:28 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* Checks if only redirections are present in the list */
bool	ft_all_redir(t_element *cmd)
{
	t_element	*head;

	head = cmd;
	while (cmd)
	{
		if (cmd->type < 3 && cmd->type != PIPE)
			return (false);
		cmd = cmd->next;
	}
	ft_only_create(head);
	return (true);
}

/* Only creates files in the case of all_redir */
void	ft_only_create(t_element *cmd)
{
	int	fd;

	while (cmd)
	{
		if (cmd->type == OUTFILE || cmd->type == OUTFILE_APPEND)
		{
			if (!only_create_outfile_or_append(cmd, &fd))
				return ;
		}
		else if (cmd->type == INFILE)
		{
			fd = open (cmd->content, O_RDONLY, 0644);
			if (fd < 0)
			{
				(ft_putstr_fd("bash: ", 2), ft_putstr_fd(cmd->content, 2));
				return (perror(" "));
			}
		}
		close (fd);
		cmd = cmd->next;
	}
}

/* Only create - outfile or append */
bool	only_create_outfile_or_append(t_element *cmd, int *fd)
{
	if (cmd->type == OUTFILE)
		*fd = open(cmd->content, O_CREAT | O_RDWR | O_TRUNC, 0644);
	else
		*fd = open(cmd->content, O_CREAT | O_RDWR | O_APPEND, 0644);
	if (*fd < 0)
	{
		perror("bash");
		return (false);
	}
	return (true);
}

/* Unlink all tmp_files */
void	ft_unlink(t_element *cmd)
{
	while (cmd->prev)
		cmd = cmd->prev;
	while (cmd)
	{
		if (cmd->type == HEREDOC)
		{
			unlink(cmd->hd_filename);
			free(cmd->hd_filename);
		}
		cmd = cmd->next;
	}
}
