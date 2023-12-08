/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 14:41:08 by kquerel           #+#    #+#             */
/*   Updated: 2023/12/08 01:00:54 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* Opens file given in parameters */
int	ft_infile(char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY, 0644);
	if (fd < 0)
	{
		ft_putstr_fd("bash : ", STDERR_FILENO);
		ft_putstr_fd(filename, STDERR_FILENO);
		perror(" ");
		return (0);
	}
	if (fd > STDERR_FILENO && dup2(fd, STDIN_FILENO) < 0)
	{
		perror("bash");
		return (0);
	}
	if (fd > STDERR_FILENO)
		close (fd);
	return (1);
}

/* Handles > and >> behavior
--- if > : create a file and replace it if it already exists
--- if >> : create a file and writes at the end of if it already exists */
int	ft_outfile(t_element *cmd)
{
	int	fd;

	if (cmd->type == OUTFILE)
		fd = open(cmd->content, O_CREAT | O_RDWR | O_TRUNC, 0644);
	else
		fd = open(cmd->content, O_CREAT | O_RDWR | O_APPEND, 0644);
	if (fd == -1)
	{
		perror("bash");
		return (0);
	}
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		perror("bash");
		close(fd);
		return (0);
	}
	close(fd);
	return (1);
}

/* Handles redirections depending on cmd->type */
int	ft_redirect(t_element *cmd)
{
	t_element	*tmp;

	if (!cmd)
		return (0);
	tmp = cmd;
	ft_top_of_list(&tmp);
	while (tmp != NULL && tmp->type != PIPE)
	{
		if (!ft_redir_while(cmd, tmp))
			return (0);
		tmp = tmp->next;
	}
	tmp = cmd;
	return (1);
}

/* For the norminette */
int	ft_redir_while(t_element *cmd, t_element *tmp)
{
	if (tmp->type == INFILE)
	{
		if (!ft_infile(tmp->content))
			return (0);
	}
	else if (tmp->type == HEREDOC)
	{
		if (!ft_infile(ft_palbon(cmd)))
			return (0);
		unlink(tmp->content);
	}
	else if (tmp->type == OUTFILE || tmp->type == OUTFILE_APPEND)
	{
		if (!ft_outfile(tmp))
			return (0);
	}
	return (1);
}

/* Goes at the start of a list */
void	ft_top_of_list(t_element **cmd)
{
	while ((*cmd)->prev && (*cmd)->prev->type != PIPE)
		(*cmd) = (*cmd)->prev;
	if ((*cmd)->type == PIPE)
		(*cmd) = (*cmd)->next;
}
