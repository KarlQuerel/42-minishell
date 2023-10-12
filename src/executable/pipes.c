/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 15:02:53 by kquerel           #+#    #+#             */
/*   Updated: 2023/10/12 15:43:06 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

/* Initiate pipe and create all pipe ends according to commands number */
void	ft_create_pipe(t_pipe *exec)
{
	int	i;

	i = 0;
	while (i < exec->pipe_nb)
	{
		printf("avant - pipe_end = %ls\n", exec->pipe_end);
		if (pipe(exec->pipe_end + 2 * i) < 0) // ne marche pas
		{
			printf("pipe_end = %ls\n", exec->pipe_end);
			//gerer les close and free
			perror("pipe_end");
			exit(EXIT_FAILURE);
		}
		i++;
	}
}

/* Close all pipe ends */
void	ft_close_pipe(t_pipe *exec)
{
	int	i;

	i = 0;
	while (i < exec->pipe_nb)
	{
		close(exec->pipe_end[i]);
		i++;
	}
}

/* Use waitpid function to wait for every child process */
int	ft_waitpid(int *pid, int n)
{
	int	i;
	int	status;

	i= 0;
	while (i < n)
	{
		waitpid(pid[i], &status, 0);
		i++;
	}
	waitpid(pid[i], &status, 0);
	return(EXIT_SUCCESS);
}


