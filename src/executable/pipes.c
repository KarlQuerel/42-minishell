/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 15:02:53 by kquerel           #+#    #+#             */
/*   Updated: 2023/10/20 13:56:26 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"


// int	*pipe_end[2];
// pipe_end[0];
// pipe_end[1];


// pipe(pipe_end);



/* Initiate pipe and create all pipe ends according to commands number */
void	ft_create_pipe(t_pipe *exec, int *pipe_end)
{
	int	i;

	i = 0;
	while (i < exec->cmd_nb)
	{
		if (pipe(pipe_end + 2 * i) < 0) // ne marche pas
		{
			//gerer les close and free
			perror("pipe_end");
			exit(EXIT_FAILURE);
		}
		i++;
	}
}

// close un fd avec panache
void	ft_close(int *fd)
{
	if (*fd > 2)
	{
		close(*fd);
		*fd = 0;
	}
}

// fonction qui ferme un pipe avec panache
void	ft_close_pipe(int pip[2])
{
	ft_close(&pip[0]);
	ft_close(&pip[1]);
}

// ferme toutes les pipes de notre exec avec panache
void	ft_close_all_pipes(t_pipe *exec)
{
	int	i;

	while (i < exec->cmd_nb - 1)
	{
		ft_close_pipe(exec->my_pipes[i]);
		i++;
	}
}


// pantheon des fonctions
/* Close all pipe ends */
// void	ft_close_pipe(t_pipe *exec)
// {
// 	int	i;

// 	i = 0;
// 	while (i < exec->cmd_nb)
// 	{
// 		close(exec->pipe_end[i]);
// 		i++;
// 	}
// }

/* Use waitpid function to wait for every child process */
int	ft_waitpid(int *pid, int n)
{
	int	i;
	int	status;

	i = 1;
	// printf("%sn = %d\n%s\n", BGRE, n, WHT);
	while (i < n)
	{
		// printf("%spid[%d] = %d\n%s\n", BGRE, i, pid[i], WHT);
		// printf("%si = %d\n%s\n", BGRE, i, WHT);
		waitpid(pid[i], &status, 0);
		i++;
	}
	waitpid(pid[i], &status, 0);
	return(EXIT_SUCCESS);
}


