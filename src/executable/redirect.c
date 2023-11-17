/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 14:41:08 by kquerel           #+#    #+#             */
/*   Updated: 2023/11/17 16:46:00 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

/* Opens file given in parameters */
int	ft_infile(char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		perror("open perror");
		return (0);
	}
	if (fd > STDERR_FILENO && dup2(fd, STDOUT_FILENO) < 0)
	{
		perror("infile dup perror");
		return (0);
	}
	if (fd > STDERR_FILENO)
		close(fd);
	return(1);
}

/* If >> is present, then we create the outfile */
int	ft_outfile(t_element *cmd)
{
	int	fd;

	if (cmd->type == OUTFILE)
		fd = open(cmd->content, O_CREAT | O_RDWR | O_TRUNC, 0644);
	else
		fd = open(cmd->content, O_CREAT | O_RDWR | O_APPEND, 0644);
	if (fd == -1)
	{
		perror("open outfile");
		return (0);
	}
	if (fd > STDERR_FILENO && dup2(fd, STDOUT_FILENO) < 0) //fd >= 0
	{
		perror("outfile dup perror");
		// close(fd);
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
{
	t_element *tmp;

	tmp = cmd;
	while (tmp)
	{
		if (tmp->type == INFILE) // <
		{
			if (!ft_infile(tmp->content))
				// gerer les free
				return (0);
		}
		else if (tmp->type == INFILE_DELIMITER) //soit <<
		{
			// if (!ft_infile_def(tmp->content))
			{
			// 	//gerer les free
				return (0);
			}
		}
		else if (tmp->type == OUTFILE || tmp->type == OUTFILE_APPEND) // soit > soit >> on cree le fichier
		{
			if (!ft_outfile(tmp))
				// gerer les free
				return (0);
		}
		tmp = tmp->next;
	}
	tmp = cmd;
	return (1);
}
