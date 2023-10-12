/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 14:41:08 by kquerel           #+#    #+#             */
/*   Updated: 2023/10/12 14:54:55 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

/* Opens file given in parameters */
int	ft_infile(char *file)
{
	int	fd;

	fd = open(file, O_RDONLY); // returns -1 on error and errno is set -> utiliser perror
	if (fd < 0)
	{
		perror("bash"); // pour tester errno
		return (EXIT_FAILURE);
	}
	if (fd > 0 && dup2(fd, STDERR_FILENO) < 0)
	{
		perror("dup2"); //pour tester errno
		return (EXIT_FAILURE);
	}
	if (fd > 0) //on success
		close(fd);
	return(EXIT_SUCCESS);
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
		return (EXIT_FAILURE);
	}
	if (fd > 0 && dup2(fd, STDERR_FILENO) < 0)
	{
		perror("open");
		return (EXIT_FAILURE);
	}
	if (fd > 0)
		close(fd);
	return (EXIT_SUCCESS);
}

/* A FAIRE
-rediriger dans le cas ou la commande est un built in a coder nous meme
	- echo -n
	- cd
	- pwd
	- export
	- unset
	- env
	- exit
- les << a gerer
*/
int	ft_redirect(t_element *s)
{
	t_element *tmp;
	tmp = s;
	while (tmp)
	{
		if (tmp->type == INFILE) // <
		{
			if (ft_infile(tmp->content))
				// gerer les free
				return (EXIT_FAILURE);
		}
		else if (tmp->type == INFILE_DELIMITER) //soit >>
		{
			printf(("les << sont encore a gerer\n"));
		}
		else if (tmp->type == OUTFILE || tmp->type == OUTFILE_APPEND) // soit > soit >>
		{
			if (ft_outfile(tmp))
				// gerer les free
				return (EXIT_FAILURE);
		}
		tmp = tmp->next;
	}
	tmp = s;
	return (EXIT_SUCCESS);
}
