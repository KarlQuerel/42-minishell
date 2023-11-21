/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 12:42:35 by octonaute         #+#    #+#             */
/*   Updated: 2023/11/20 18:44:43 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

/*Prints the arguments that follow the echo command until
the next cmd is a pipe or equals NULL.*/
void	echo(t_element *current)
{
	bool	newline;

	newline = true;
	if (current->next == NULL)
	{
		ft_putstr_fd("\n", STDOUT_FILENO);
		return ;
	}
	current = current->next;
	while (current != NULL && current->type < 3)
	{
		if (current->type == OPTION && ft_strncmp(current->content, "-n", \
		ft_strlen(current->content)) == 0 && \
		current->prev->type == COMMAND)
		{
			newline = false;
			current = current->next;
		}
		ft_putstr_fd(current->content, STDOUT_FILENO);
		if (current->next != NULL)
			ft_putstr_fd(" ", STDOUT_FILENO);
		current = current->next;
	}
	if (newline == true)
		ft_putstr_fd("\n", STDOUT_FILENO);
}
