/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: casomarr <casomarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 14:41:08 by kquerel           #+#    #+#             */
/*   Updated: 2023/11/27 20:15:24 by casomarr         ###   ########.fr       */
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

	(void)exec;
	if (!cmd)
		return (0);
	tmp = cmd;
	while (tmp->prev && tmp->prev->type != PIPE)//on revient au tout debut de la cmd
		tmp = tmp->prev;
	if (tmp->type == PIPE)
		tmp = tmp->next;
	while (tmp != NULL && tmp->type != PIPE)
	{
		if (tmp->type == INFILE || tmp->type == HEREDOC)
		{
			if (!ft_infile(tmp->content))
			{
				// gerer les free
				// gerer les unlink
				return (0);
			}
			if (tmp->type == HEREDOC)
				unlink(tmp->content);
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

//a faire avant chaque prompt
//close(fd heredoc)
//unlink(exec->hd_name)
//free exec->hd_name;


/* Creates heredoc */
char	*create_heredoc(char *name, int i, int *fd)
{
	name = ft_strjoin("tmp_file", ft_itoa(i));
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

/* Handles heredoc behavior */
bool	ft_heredoc(t_element *elem)
{
	int			fd;
	char		*words;
	char		*file_name;
	static int	iteration_nb = 1;
	
	words = NULL;
	iteration_nb++;
	g_location = IN_HEREDOC;
	set_signals();
	file_name = create_heredoc(elem->content, iteration_nb, &fd);
	if (!file_name)
		return (false);

	int test_fd = dup(STDIN_FILENO);

	while (1)
	{
		words = readline("> ");
		if (words && *words)
			ft_putendl_fd(words, fd);
		free(words);
		if (g_location == QUIT_HEREDOC)
		{
			dup2(test_fd, STDIN_FILENO);
			close(fd);
			unlink(file_name);
			/*soit changer ici la valeur de $? a 130,
			soit tu preferes eviter d'envoyer env a cette fonction
			et dans ce cas return false au lieu de break et dans la
			fonction plus haut tu fais un si ca return false
			changer la valeur de EXIT_STATUS dans env. Je te changerai
			la valeur moi-meme, juste dis-moi ou tu preferes que je le fasse.*/
			break;
		}
		if (words == NULL)
		{
			ft_putendl_fd("bash: warning: here-document at line 3 delimited by end-of-file", STDERR_FILENO);
			break; //soit return false (je ne sais pas ce que ca change dans tes fonctions)
		}
		if (ft_strncmp(words, elem->content, ft_strlen(words)) == 0 && ft_strlen(words) == ft_strlen(elem->content))
			break; //soit return false (je ne sais pas ce que ca change dans tes fonctions)
	}
	close(fd);
	return (true);
}
