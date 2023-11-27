/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: casomarr <casomarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 12:42:35 by octonaute         #+#    #+#             */
/*   Updated: 2023/11/27 14:03:01 by casomarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	no_further_args(t_element *cmd)
{
	cmd = cmd->next;
	while(cmd)
	{
		if (cmd->type == ARGUMENT)
			return (false);
		cmd = cmd->next;
	}
	return (true);
}

/*Prints the arguments that follow the echo command until
the next cmd is a pipe or equals NULL.*/
void	echo(t_element *current)
{
	bool	newline;

	newline = true;
	if (no_further_args(current) == true)
	{
		ft_putstr_fd("\n", STDOUT_FILENO);
		return ;
	}
	current = current->next;
	while (current != NULL && current->type != PIPE)
	{
		if (current->type == OPTION)
			newline = false;
		else if (current->type == ARGUMENT)
		{
			ft_putstr_fd(current->content, STDOUT_FILENO);
			if (no_further_args(current) == false)
				ft_putstr_fd(" ", STDOUT_FILENO);
		}
		current = current->next;
	}
	if (newline == true)
		ft_putstr_fd("\n", STDOUT_FILENO);
}
