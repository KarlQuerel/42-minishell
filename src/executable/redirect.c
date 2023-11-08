/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karl <karl@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 14:41:08 by kquerel           #+#    #+#             */
/*   Updated: 2023/11/07 16:00:00 by karl             ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

/* Opens file given in parameters */
int	ft_infile(char *file)
{
	int	fd;

	fd = open(file, O_RDONLY); // returns -1 on error and errno is set -> utiliser perror
	if (fd < 0)
	{
		perror("open perror"); // pour tester errno
		return (0);
	}
	if (fd > 0 && dup2(fd, STDERR_FILENO) < 0)
	{
		perror("infile dup perror"); //pour tester errno
		return (0);
	}
	if (fd > 0) //on success
		close(fd);
	return(1);
}

/* If >> is present, then we create the outfile */
int	ft_outfile(t_element *cmd)
{
	int	fd;

	if (cmd->type == OUTFILE_APPEND)
		fd = open(cmd->content, O_CREAT | O_RDWR | O_APPEND, 0644);
	else
		fd = open(cmd->content, O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror("open outfile");
		return (0);
	}
	if (fd > 0 && dup2(fd, STDERR_FILENO) < 0)
	{
		perror("outfile dup perror");
		return (0);
	}
	if (fd > 0)
		close(fd);
	return (1);
}

/* A FAIRE
-rediriger dans le cas ou la commande est un built in a coder nous meme
- les << a gerer
*/
int	ft_redirect(t_element *cmd)
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
		else if (tmp->type == INFILE_DELIMITER) //soit >>
		{
			printf(("les << sont encore a gerer\n"));
		}
		else if (tmp->type == OUTFILE || tmp->type == OUTFILE_APPEND) // soit > soit >>
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
