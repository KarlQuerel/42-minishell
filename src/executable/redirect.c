/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: casomarr <casomarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 14:41:08 by kquerel           #+#    #+#             */
/*   Updated: 2023/11/27 14:03:43 by casomarr         ###   ########.fr       */
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

	if (!cmd)
		return (0);
	tmp = cmd;
	while (tmp->prev && tmp->prev->type != PIPE)//on revient au tout debut de la cmd
		tmp = tmp->prev;
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
			if (!ft_heredoc_test(exec, tmp->content))
			// if (!ft_heredoc(tmp->content, exec))
			{
				close (exec->fd_temp);
				unlink(exec->hd_filename);
				//gerer les free
				return (0);
			}
		}
		else if (tmp->type == OUTFILE || tmp->type == OUTFILE_APPEND)
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

// si control D dans le heredoc, le message 
// bash: warning: here-document at line 1 delimited by end-of-file (wanted `heredoc')


	// etape 1
	/*
		si type HEREDOC
		open heredoc
		>
		close
		stock le nom 
		check si heredoc 1 existe 
		
	
	*/


/* Handles heredoc behavior */
/* int	ft_heredoc(char *heredoc, t_pipe *exec)
{
	int	fd[2];
	
	// int fd_hd;

	// fd_hd = open(heredoc, O_WRONLY | O_CREAT, 0644);
	// if (fd_hd < 0)
	// {
	// 	perror("bash HEREDOC");
	// 	return (0);
	// }
	
	g_location = IN_HEREDOC;
	if (pipe(fd) < 0)
	{
		perror("bash");
		return (-1);
	}
	write_heredoc(heredoc, exec, fd[1]);
	close (fd[1]);
	if (g_signals.exit_status == 130) //pour le CTRL C
	{
		close(fd[0]);
		return (-1);
	}
	return (fd[0]);


	//int fd;
// 	g_location = IN_HEREDOC;
// 	set_signals();
	// fd_hd = open(heredoc, O_WRONLY | O_CREAT, 0644);
	// if (fd_hd < 0)
	// {
	// 	perror("bash HEREDOC");
	// 	return (0);
	// }
// 	exec->hd_filename = heredoc; // on stock le nom dans notre structure
// 	// exec->fd_here_doc = dup(STDIN_FILENO);
// 	while (g_location == IN_HEREDOC)
// 		create_heredoc(heredoc, exec, fd_hd);
// 	// close(exec->fd_here_doc);  // 4
// 	// unlink(heredoc);
// 	return (0);
} */

/* void	write_heredoc(char *safe_word, t_pipe *exec, int fd, t_env *env)
{
	char *words;
	(void)exec;
	t_env	*exit_status;

	exit_status = find_value_with_key_env(*env, "EXIT_STATUS");
	words = readline(">");
	while (ft_strncmp(exit_status->value, ft_itoa(130), 3) != 0 && (ft_strncmp(words, safe_word, ft_strlen(words)) != 0 || \
	ft_strlen(words) != ft_strlen(safe_word)))
	{
		free (words);
		words = readline(">");
		ft_putendl_fd(words, fd);
	}
	free (words);
	g_location = IN_COMMAND;
} */

/* Puts minishell in an interactive mode for here_doc */
void	create_heredoc(char *safe_word, t_pipe *exec, int fd)
{
	char	*words;

	(void)exec;
	words = readline("> ");
	while ((ft_strncmp(words, safe_word, ft_strlen(words)) != 0 || \
	ft_strlen(words) != ft_strlen(safe_word)))
	{
/* 		if (g_location == QUIT_HEREDOC)
		{
			printf("HELLO\n");
			free(words);
			return ;
		} */
		free (words);
		words = readline("> ");
		ft_putendl_fd(words, fd);
	}
	free (words);
	g_location = IN_COMMAND;
}
int	ft_open_hd(t_pipe *exec, int iteration_nb)
{
	if (exec->hd_filename == NULL)
	{
		exec->hd_filename = ft_strjoin("tmp_file", ft_itoa(iteration_nb));
		//fd[0];
		exec->fd_temp = open(exec->hd_filename, O_RDWR | O_CREAT | O_EXCL, 0777);
		if (exec->fd_temp == -1)
		{
			perror("perror : can't open the heredoc");
			close(exec->fd_temp);
			unlink(exec->hd_filename);
			free (exec->hd_filename);
			return (-1);
		}
		return (1);
	}
	else
	{
		close(exec->fd_temp);
		unlink(exec->hd_filename);
		exec->hd_filename = ft_strjoin("tmp_file", ft_itoa(iteration_nb));
		//fd[0];
		exec->fd_temp = open(exec->hd_filename, O_RDWR | O_CREAT | O_EXCL, 0777);
		if (exec->fd_temp == -1)
		{
			perror("\nperror : can't open the heredoc");
			printf("| Error Code : %d \n", errno);
			close(exec->fd_temp);
			unlink(exec->hd_filename);
			free (exec->hd_filename);
			return (-1);
		}
	}
	return (0);
}

//a faire avant chaque prompt
//close(fd heredoc)
//unlink(exec->hd_name)
//free exec->hd_name;


int	ft_heredoc_test(t_pipe *exec, char *heredoc)
{
	int	fd;
	char		*words;
	static int	iteration_nb = 1;
	char *filename;

	g_location = IN_HEREDOC;
	// ft_open_hd(exec, iteration_nb);

	while (1)
	{
		filename = ft_strjoin("tmp_file", ft_itoa(iteration_nb)); // le foutre dans le /tmp
		if (!filename)
			return (0);
		if (access(filename, F_OK))
			break;
		free(filename);
		iteration_nb++;
	}
	
	fd = open(filename, O_WRONLY | O_CREAT | O_EXCL, 0774);
	if (fd < 0)
	{
		perror("bash");
		return (0);
	}
	exec->hd_filename = filename;
	exec->fd_here_doc = dup(STDIN_FILENO);

	while (g_location == IN_HEREDOC)
	{
		words = readline("> ");
		if (!words)
			return (0);
		//CAS CTRL + D pour que ca quitte le heredoc
		if (ft_strncmp(words, heredoc, ft_strlen(words)) == 0 && \
			ft_strlen(words) == ft_strlen(heredoc))
		{
			close (fd);
			return (1);
		}
		write(fd, words, ft_strlen(words));
		free(words);
	}
	if (g_location == QUIT_HEREDOC)
	{
		close(fd);
		free(filename);
		g_location = IN_PROMPT;
		return (0);
	}
	close(fd);
	close(exec->fd_here_doc);
	g_location = IN_PROMPT;
	return (1);
}
