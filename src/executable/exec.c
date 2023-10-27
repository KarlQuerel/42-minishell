/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 14:46:12 by kquerel           #+#    #+#             */
/*   Updated: 2023/10/27 17:13:16 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

/*
-- CARO :
--> Pour le parsing, les cas suivants ne doivent pas etre acceptes:
	Deux pipes qui se suivent APRES la premiere commande
	--> "ls wc -l || hi" ne doit pas etre accepte
	Pas de pipe a la fin
	--> "ls -a | wc -l |" ne doit pas etre accepte
--> POUR ECHO: 
	-- on gere pas
	echo -nnnnnnnnnnnnnnn ca marche par contre echo -nnnnnnnnnt doit print le -nnnnnnnnt

--> segfault: (lancer avec valgrind pour check d'ou ca vient)
	"ls -a | wc -l | exit"
	"ls -a | echo hi | wc -l"

--> pour les builtins:
	" pwd | ls -l" tourne dans le vide, il faut envoyer fd a ft_builtins pour utiliser ft_putstrfd dans tous nos builtins
-- faire en sorte que tous les builtins prennent t_cmd pour check next node builtin
	--> du coup plus besoin mais peut etre plus tard, je le laisse la dans le doute

--> gerer l'exit code dans une variable globale pour les signaux (exit)
	man waitpid, regarder tous les WIF et wstatus
	si c'est $?, cas special
	les signaux prennent 125 + l'int que rend le signal
	CTRL + C = 5 + 125 = 130
	127 command not found (2)


	utiliser strerror;
	

	
TO DO:
- gerer open et HEREDOC, en dernier
- redirections
- finir export et unset (env ne change pas)
*/

//---------------------------------------------------------------------------------------------------------


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
		ft_builtins(cmd, env, entries);
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
	//ft_print_array(exec->cmd_tab);
	if (execve(cmd->content, exec->cmd_tab, env->env) == -1)
		ft_putstr_fd("execve failed\n", STDOUT_FILENO);
	}
	waitpid(pid, NULL, 0);
}

/* Handles the execution part
--- Gets size_cmd to alloc memory accordingly
--- Gets the amount of pipe to detect if we need to create childs.
--- ft_calloc takes a +1 for the NULL string at the end of cmd_tab
--- If no pipes are detected, fills cmd_tab and calls single_command function
--- Else we calls multipe_commands function
*/
void	ft_execute(t_element *cmd, t_env *env, t_pipe *exec, t_history *entries)
{
	int	i;
	int	size_cmd;
	
	if (!cmd)
		return ;
	i = 0;
	size_cmd = get_size_cmd(cmd);
	exec->pipe_nb = ft_count_pipes(cmd);
	exec->cmd_tab = ft_calloc(size_cmd + 1, sizeof(char *));
	if (!exec->cmd_tab)
		return ;
	if (exec->pipe_nb == 0)
	{
		fill_array(cmd, exec);
		single_command(cmd, env, exec, entries);
	}
	else
		multiple_commands(cmd, env, exec, entries);
}

/* If no pipes are present
--- If a builtin is detected, ft_builtins is called, preventing
	from reaching handle_command
---	if child pid is detected, handle_command is called
*/
void	single_command(t_element *cmd, t_env *env, t_pipe *exec, t_history *entries)
{
	int	pid;

	if (cmd && cmd->builtin == true)
	{
		ft_builtins(cmd, env, entries);
		return ;
	}
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		exit(127);
	}
	if (pid == 0)
		handle_command(cmd, env, exec, entries);
	waitpid(pid, NULL, 0);
	// signaux pour caro
	// if (WIFEXITED(status))
	// 	g_global.error_num = WEXITSTATUS(status);
}

/* Separates the pipes according to their number
--- If we are on the (1 to n -1) range, we call midde_pipes
	--> when we are on a pipe and the pipe is
---	If we are on the last pipe we call last_pipe
*/
void	multiple_commands(t_element *cmd, t_env *env, t_pipe *exec, t_history *entries)
{
	int	i = 0;
	int	fd_temp; // pour le cas du CTRL+D et heredoc, on gerera apres
	int	fd[2]; // exec->fd[2];
	
	fd_temp = 0;
	while (i <= exec->pipe_nb)
	{
		fill_array(cmd, exec);
		if ( i < exec->pipe_nb)
		{
			middle_pipes(fd, &fd_temp, cmd, env, exec, entries);
			while (cmd->next && cmd->type != PIPE)
			{
				cmd = cmd->next;
				if (cmd->type == PIPE && cmd->next)
				// "&& cmd->next" ne devrait plus etre necessaire avec le fix du parsing de caro
				{
					cmd = cmd->next;
					break; 
				}
			}
		}
		else
			last_pipe(fd, &fd_temp, cmd, env, exec, entries);
		i++;
	}
	wait(NULL); // ou waitpid
}

/* Handles all middle pipes behaviour
--- Calls middle_dup function if child process is created 
---	If fd_temp exists, we dup the reading fd and assigns it to fd_temp*/
void	middle_pipes(int *fd, int *fd_temp, t_element *cmd, t_env *env, t_pipe *exec, t_history *entries)
{
	int	pid;

	if (pipe(fd) < 0)
		perror("Pipe");
	pid = fork();
	if (pid < 0)
		perror("Fork");
	if (pid == 0)
		middle_dup(fd, *fd_temp, cmd, env, exec, entries);
	else
	{
		if (*fd_temp)
			close(*fd_temp);
		*fd_temp = dup(fd[0]);
		close(fd[0]);
		close(fd[1]); // nouveau
		waitpid(pid, NULL, 0); // ca change rien avec ou sans
	}
}

/* Handles last pipe behaviour */
void	last_pipe(int *fd, int *fd_temp, t_element *cmd, t_env *env, t_pipe *exec, t_history *entries)
{
	int	pid;

	pid = fork();
	if (pid < 0)
		perror("fork");
	if (pid == 0)
		last_dup(fd, *fd_temp, cmd, env, exec, entries);
	else
	{
		if (*fd_temp)
			close(*fd_temp);
		close(fd[0]);
		waitpid(pid, NULL, 0);
	}
}
