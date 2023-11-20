/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 14:41:08 by kquerel           #+#    #+#             */
/*   Updated: 2023/11/20 11:32:42 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

/* Opens file given in parameters */ // utiliser des static pour les fonctions
int	ft_infile(char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		strerror(errno);
		perror("bash");
		ft_putendl_fd(filename, STDOUT_FILENO);
		return (0);
	}
	if (fd > STDERR_FILENO && dup2(fd, STDOUT_FILENO) < 0)
	{
		perror("bash");
		return (0);
	}
	if (fd > STDERR_FILENO)
		close(fd);
	return(1);
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
	if (fd > STDERR_FILENO && dup2(fd, STDOUT_FILENO) < 0)
	{
		perror("bash");
		close(fd);
		return (0);
	}
	if (fd > STDERR_FILENO)
		close(fd);
	return (1);
}

/* 
A FAIRE
-rediriger dans le cas ou la commande est un built in a coder nous meme
- les << a gerer

Handles redirections depending on cmd->type */
int	ft_redirect(t_element *cmd/* , int option */)


//utiliser const t_element *cmd au lieu des tmps
// const void *cmd -> ce qui est pointe ne change pas
// void *const cmd -> le pointeur cmd ne change pas
// const void *const cmd -> ce qui est pointe et le pointeur cmd ne changent pas
{
	t_element *tmp;

	tmp = cmd;
	while (tmp != NULL && tmp->type != PIPE)
	{
		if (tmp->type == INFILE)
		{
			if (!ft_infile(tmp->content))
				// gerer les free
				return (0);
		}
		else if (tmp->type == INFILE_DELIMITER)
		{
			if (!ft_heredoc(tmp->content))
			{
				//gerer les free
				return (0);
			}
		}
		else if (tmp->type == OUTFILE || tmp->type == OUTFILE_APPEND)
		{
			if (ft_outfile(tmp) == 0)
				// gerer les free
				return (0);
		}
		tmp = tmp->next;
	}
	tmp = cmd;
	return (1);
}

/* Handles heredoc behavior */
int	ft_heredoc(char *heredoc)
{
	ft_putstr_fd(heredoc, STDOUT_FILENO);
	return (1);
}
