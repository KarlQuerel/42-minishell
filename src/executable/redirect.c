/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 14:41:08 by kquerel           #+#    #+#             */
/*   Updated: 2023/11/22 14:47:46 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

/* Opens file given in parameters */
int	ft_infile(char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY, 0644);
	if (fd < 0)
	{
		ft_putstr_fd("bash: ", STDERR_FILENO);
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

/* Handles redirections depending on cmd->type */
int	ft_redirect(t_element *cmd, t_pipe *exec)
{
	t_element *tmp;

	tmp = cmd;
	if (tmp->type == PIPE)
		tmp = tmp->next;
	while (tmp != NULL && tmp->type != PIPE)
	{
		if (tmp->type == INFILE)
		{
			if (!ft_infile(tmp->content))
				// gerer les free
				return (0);
		}
		else if (tmp->type == HEREDOC)
		{
			if (!ft_heredoc(tmp->content, exec))
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

// si control D dans le heredoc, le message 
// bash: warning: here-document at line 1 delimited by end-of-file (wanted `heredoc')


/* Handles heredoc behavior */
int	ft_heredoc(char *heredoc, t_pipe *exec)
{
	int	fd;
	
	g_signals.location = IN_HEREDOC;
	fd = open(heredoc, O_WRONLY | O_CREAT, 0644); // 3
	if (fd < 0)
	{
		perror("bash HEREDOC");
		return (0);
	}
	exec->fd_here_doc = dup(STDIN_FILENO); //4
	while (g_signals.location == IN_HEREDOC)
		create_heredoc(heredoc, exec, fd);
	close(exec->fd_here_doc);  // 4
	close(fd); // 3
	unlink(heredoc);
	return (1);
}

/* Puts minishell in an interactive mode for here_doc */
void	create_heredoc(char *safe_word, t_pipe *exec, int fd)
{
	char	*words;

	(void)exec;
	words = readline("> ");
	while ((ft_strncmp(words, safe_word, ft_strlen(words)) != 0 || \
	ft_strlen(words) != ft_strlen(safe_word)))
	{
		free (words);
		words = readline("> ");
		ft_putendl_fd(words, fd);
	}
	free (words);
	g_signals.location = IN_COMMAND;
}
