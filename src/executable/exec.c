/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 14:46:12 by kquerel           #+#    #+#             */
/*   Updated: 2023/10/24 16:51:53 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

/*
TO DO:
- voir avec Alban, cat Makefile
- gerer open et HEREDOC, en dernier
- redirections
--> DEMANDER A ALBAN
- 	UNSET (doubler l'env ?) --> on utilise notre t_env et pas envp
- 	EXPORT (doubler l'env ?)

- 	necessaire de fork dans execute_command ? 
	  Sur papier, ca ne devrait pas etre le cas mais ca ne marche pas sans
	--> waitpid du coup pas necessaire si pas besoin de fork.
*/

/* Executes the command
---	If a builtin is detected, redirects to commands to avoid sending it 
	to execve.
---	split_path returns the "PATH=" variable from env, split the string by :
	and assigns it to cmd_path.
---	ft_get_command tests all paths variable from cmd_path, joins it with
	the command cmd_tab[i] and tests its validity with access.
---	Execve replaces the existing process by the program within his first argument
	1st arg: cmd->content, pointer to a string that specifies the path
		to the exec file, can be binary or script file.
	2nd arg: array of strings (char **) representing the command-line of arguments
		of the new program.
	3st arg: array of strings (char **) representing the environment variables
		for the new program.
--- Waitpid waits for the process to end before continuing.
 */
void	execute_command(t_element *cmd, t_env *env, t_pipe *exec, t_history *entries, int i)
{
	if (cmd->builtin == true)
	{
		commands(cmd, env, entries);
		return ;
	}
	int	pid;
	if (cmd->content[i] == '\0')
		return ;
	pid = fork();
	if (pid < 0)
	{
		perror("fork:");
		//free
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
	{
	exec->cmd_path = split_path(env);
	if (!exec->cmd_path)
	{
		printf("Split_path failed\n");
		// free des trucs
	}
	cmd->content = ft_get_command(exec->cmd_path, exec->cmd_tab[i]);
	if (!cmd->content)
	{
		if (!exec->cmd_tab[i])
			ft_putstr_fd("\n", 2);
		else
		{
			ft_putstr_fd(exec->cmd_tab[i], 2);
			ft_putstr_fd(": command not found\n", 2);
		}
	}
	ft_print_array(exec->cmd_tab);
	if (execve(cmd->content, exec->cmd_tab, env->env) == -1)
		ft_putstr_fd("execve failed\n", STDOUT_FILENO);
	}
	waitpid(pid, NULL, 0);
}

/* Handles execution 
--- Gets arg number to be able to malloc cmd_tab (char **) to be sent to execve.
--- Fills cmd_tab with every node with cmd->type COMMAND and cmd->type OPTION.
--- Gets every node with cmd->type COMMAND and assigns to cmd_nb.
	cmd_nb will determine the number of childs and pipes to create --> La ou est le probleme
--- If only one COMMAND is detected, execute_command is called, no need for childs.
--- init_pipes mallocs the pipe necessary for dup2
--- ft_redir is called as many times as there are childs.
*/
void	ft_execute(t_element *cmd, t_env *env, t_pipe *exec, t_history *entries)
{
	int	i;
	
	i = 0;
	exec->av_nb = get_args_nb(cmd);
	printf("av_nb = %d\n", exec->av_nb); //  boucle infinie ici
	exec->cmd_tab = ft_calloc(exec->av_nb, sizeof(char *));
	if (!exec->cmd_tab)
		return ;
	fill_cmd_tab(cmd, exec);
	get_cmds_nb(cmd, exec);
	// printf("exec->cmd_nb = %d\n", exec->cmd_nb);
	if (exec->cmd_nb == 1)
		execute_command(cmd, env, exec, entries, 0);
	else
	{
		if (!init_pipes(exec))
				return ;
		exec->pid = ft_calloc(10, sizeof(pid_t));
		// exec->pid = ft_calloc(exec->cmd_nb - 1, sizeof(pid_t));
		if (!exec->pid)
		{
			ft_putendl_fd("Pid has failed to malloc", STDERR_FILENO);
			exit (127); // a enlever apres
			// free
			return ;
		}
		while (i < exec->cmd_nb)
		{
			ft_redir(cmd, env, exec, entries, i);
			i++;
		}
		ft_waitpid(exec->pid, i);
	}
	
	// a voir
	i = 0;
	while (exec->cmd_tab[i])
	{
		free(exec->cmd_tab[i]);
		i++;
	}
	free(exec->cmd_tab);
}

/* Extracts command from char *argument and verify if they are valid
using access */
char	*ft_get_command(char **path, char *argument)
{
	char	*to_free;
	char	*to_return;
	int		i;

	i = 0;
	if (argument && path)
	{
		while (path[i])
		{
			to_free = ft_strjoin(path[i], "/");
			to_return = ft_strjoin(to_free, argument);
			free(to_free);
			// printf("path[%d] = %s\n", i, to_return);
			if (access(to_return, F_OK) == 0)
				return (to_return);
			free(to_return);
			i++;
		}
	}
	return (NULL);
}

// void	redir(t_element *cmd, t_pipe *exec, t_env *env, char *line, char *home_path)
// {
// 	pid_t	pid;
// 	int	pipefd[2];

// 	if (pipe(pipefd) < 0)
// 	{
// 		perror("pipe");
// 		return ;
// 	}
// 	pid = fork();
// 	if (pid) // parent
// 	{
// 		close(pipefd[1]);
// 		dup2(pipefd[0], STDIN_FILENO);
// 		waitpid(pid, NULL, 0);
// 	}
// 	else // child pid == 0
// 	{
// 		close(pipefd[0]);
// 		dup2(pipefd[1], STDOUT_FILENO);
// 		execute_command(cmd, env, exec, line, home_path);
// 		exec->pid = ft_calloc(sizeof(int), exec->cmd_nb + 2);
// 		if (!exec->pid)
// 			return (msg_error(0));
// 		childrens(cmd, exec);
// 	}
// }

/* Checks if there is a pipe before */
bool	ft_is_a_pipe_before(t_element *cmd)
{
	while (cmd)
	{
		if (cmd->type == PIPE)
			return (true);
		cmd = cmd->prev;
	}
	return (false);
}

/* Checks if there is a pipe after */
bool	ft_is_a_pipe_after(t_element *cmd)
{
	while (cmd)
	{
		if (cmd->type == PIPE)
			return (true);
		cmd = cmd->next;
	}
	return (false);
}

/* Malloc all the pipes */
bool	init_pipes(t_pipe *exec)
{
	int	i;

	i = 0;
	if (exec->cmd_nb == 1)
	{
		exec->my_pipes = NULL;
		return (true);
	}
	exec->my_pipes = ft_calloc(2, sizeof(int *));
	if (!exec->my_pipes)
	{	
		ft_putendl_fd("Malloc failed\n", STDERR_FILENO);
		// free
		return (false);
	}
	exec->my_pipes[0] = ft_calloc (2, sizeof(int)); // on malloc 2 fd pour chaque pipe
	if (!exec->my_pipes[0])
	{
		ft_putendl_fd("Malloc failed\n", STDERR_FILENO);
		// free
		return (false);
	}
	exec->my_pipes[1] = ft_calloc (2, sizeof(int)); // on malloc 2 fd pour chaque pipe
	if (!exec->my_pipes[1])
	{
		ft_putendl_fd("Malloc failed\n", STDERR_FILENO);
		// free
		return (false);
	}
	while (i < exec->cmd_nb)
	{
		//exec->my_pipes[0];
		exec->my_pipes[i][0] = 0; // fd[0]
		exec->my_pipes[i][1] = 0; // fd[1]
		i++;
	}
	return (true);
}

// --> Fonction originale, ne pas effacer
// 	i = 0;
// 	if (exec->cmd_nb == 1)
// 	{
// 		exec->my_pipes = NULL;
// 		return (true);
// 	}
// 	printf("exec->cmd_nb = %d\n", exec->cmd_nb);
// 	exec->my_pipes = malloc(sizeof(int) * 2 * (exec->cmd_nb - 1));
// 	if (!exec->my_pipes)
// 		return (false);
// 	exec->my_pipes[i] = malloc (sizeof(int) * (exec->cmd_nb - 1));
// 	if (!exec->my_pipes)
// 		return (false);
// 	while (i < exec->cmd_nb - 1)
// 	{
// 		exec->my_pipes[i] = 0;
// 		// exec->my_pipes[i][0] = 0;
// 		// exec->my_pipes[i][1] = 0;
// 		i++;
// 	}
// 	return (true);
// }

// --> FONCTION DE PERE FOURAS
bool	ft_redir(t_element *cmd, t_env *env, t_pipe *exec, t_history *entries, int i)
{
	
	if (i != exec->cmd_nb) // we are on the last cmd
	{
		if (pipe(exec->my_pipes[i % 2]) < 0)
		{
			perror("pipe");
			return (false);
		}
	}
	
	
	
	// if (pipe(exec->my_pipes[i % 2]) < 0)
	// {
	// 	perror("pipe");
	// 	return (false);
	// }
	exec->pid[i] = fork();
	if (exec->pid[i] < 0)
	{
		perror("fork");
		return (false);
	}
		
	if (exec->pid[i] == 0) // child process
	{
		if (i == 0) // First command, redirect output to the pipe
		{
			//gerer les redirections >> > << <
			close(exec->my_pipes[i % 2][0]);
			dup2(exec->my_pipes[i % 2][1], STDOUT_FILENO); //STDOUT -> [0][1]
		}
		else if (i == exec->cmd_nb - 1) // Last command, redirect input from the pipe
		{
			close(exec->my_pipes[(i + 1) % 2][1]);
			dup2(exec->my_pipes[(i + 1) % 2][0], STDIN_FILENO); // STdIN -> [i][0]
		}
		else // Middle commands, set up input and output redirection
		{
			close(exec->my_pipes[i % 2][0]);
			close(exec->my_pipes[(i + 1) % 2][1]);
			dup2(exec->my_pipes[(i + 1) % 2][0], STDIN_FILENO);
			dup2(exec->my_pipes[i % 2][1], STDOUT_FILENO);
		}
		// Call the function to execute the command, assuming it exists
		execute_command(cmd, env, exec, entries, i);
		// If execute_command fails, exit the child process
		exit(EXIT_FAILURE);
	}
	// Close unnecessary file descriptors in the parent process
	close(exec->my_pipes[i % 2][1]);
	close(exec->my_pipes[(i + 1) % 2][0]);
	waitpid(exec->pid[i], NULL, 0);
	return (true);
}

// --> OG FUNCTION
/* Handles multiples pipes */
// bool	ft_redir(t_element *cmd, t_env *env, t_pipe *exec, t_history *entries, int i)
// {
// 	// A GERER APRES
// 	// if ()//j'ai une / des redirection d'entree
// 	// {
// 	// }
// 	if (pipe(exec->my_pipes[i]) < 0)
// 		perror("pipe");
// 	exec->pid[i] = fork();
// 	if (exec->pid[i] < 0)
// 	{
// 		perror ("fork");
// 		return (false);
// 	}
// 	if (exec->pid[i] == 0) //child
// 	{
// 		if (i == 0) // first command
// 		{
// 			//dans le cas d'infile dup2(fd_infile, 0);
// 			if (dup2(exec->my_pipes[i][1], 1) < 0)
// 				printf("dup2 1 failed\n");
// 			// ft_close(exec->my_pipes[0]);
// 			//close(exec->my_pipes[0][0]);
// 			close(exec->my_pipes[0][1]);
// 		}
// 		else if (i == exec->cmd_nb - 1) // last command
// 		{
// 			//dans le cas outfile dup(fd, 1);
// 			if (dup2(exec->my_pipes[(i + 1) % 2][0], 0) < 0)
// 				printf("dup2 2 failed\n");
// 			// ft_close(exec->my_pipes[1]);
// 			close(exec->my_pipes[(i + 1) % 2][0]);
// 			close(exec->my_pipes[(i + 1) % 2][1]);
// 		}
// 		else // les middles commands
// 		{
// 			if (dup2(exec->my_pipes[(i + 1) % 2][0], 0) < 0) // on alterne entre 0 et 1 (pour alterner les fd)
// 				printf("dup2 3 failed\n");
// 			if (dup2(exec->my_pipes[i % 2][1], 1) < 0)
// 				printf("dup2 4 failed\n");
// 		}
// 		// ft_close infile et outfile
// 		execute_command(cmd, env, exec, entries, i);
// 		//ft_execve();
// 	}
// 	if (i)
// 		ft_close_pipe(exec->my_pipes[(i + 1) % 2]);
// 	// ft_close_all_pipes(exec);
// 	return (true);
// }

//->PABLO
// ici on fair les redirection avec panache
// cas 1 : j'ai pas de pipe avant :
// je ne fais rien
// cas 2 : j'ai une pipe avant :
// je dois dup 2 mon fd 0 vers le fd[0] de la pipe

// cas 1 bis : j'ai pas de pipe apres :
// je ne fais rien
// cas 2 bis : j'ai une pipe apres :
// je dois dup 2 mon fd 1 vers le fd 1 de la pipe


// void	execution(t_element *cmd, t_pipe *exec)
// {
// 	int	i;

// 	if (!ft_give_me_my_pipes(exec))
// 		;// le menage
// 	i = 0;
// 	if (exec->cmd_nb == 1 && cmd->builtin == true)
// 	{
// 		;//je lance le builtin sans lancer de child process
// 		return ;
// 	}
// 	while (i < exec->cmd_nb)
// 	{
// 		//ft_child(cmd, exec, i);
// 		i++;
// 	}
	
// void	ft_close_fd(t_pipe *exec)
// {
// 	close(exec->my_pipes[0][0]);
// 	close(exec->my_pipes[0][1]);
// 	close(exec->my_pipes[1][0]);
// 	close(exec->my_pipes[1][1]);
// }


// fin de ft_redir
// if (!cmd)
// 		return (true);
// 	if (ft_is_a_pipe_before(cmd))
// 	{
// 		printf("i = %d\n", i);
// 		printf("content BEFORE = %s\n", cmd->content);
// 		dup2(exec->my_pipes[i - 1][0], 0);
// 	}

// 	// A GERER APRES
// 	// if ()// j'ai une / des redirections de sortie
// 	// {

// 	// }
	
// 	else if (ft_is_a_pipe_after(cmd))
// 	{
// 		printf("i = %d\n", i);
// 		printf("content AFTER = %s\n", cmd->content);
// 		dup2(exec->my_pipes[i][1], 1);
// 	}
// 	// ft_close_pipe(exec->fd_file); // apres
// 	ft_close_all_pipes(exec);
// 	//ft_execve()
// 	return (true);
// }


// void	ft_child(t_element *cmd, t_pipe *exec, int i)
// {
// 	// etape 1 on redirige les trucs
// 	//if (!ft_redir(cmd, exec, i))
// 		;
// 	// etape 2 on cherche un chemin
// 	// etape 3 on execute l'enfant
// }