/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_errors.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 16:24:57 by kquerel           #+#    #+#             */
/*   Updated: 2023/11/29 21:43:04 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* Redirects msg errors */
void	msg_error(int err, char *s)
{
	if (err == 0)
		ft_putendl_fd("No arguments accepted: run again with ./minishell", \
		STDERR_FILENO);
	if (err == 1)
		ft_putendl_fd("Allocation failed", STDERR_FILENO);
	if (err == 2)
	{
		ft_putstr_fd("bash: ", STDERR_FILENO);
		ft_putstr_fd(s, STDERR_FILENO);
	}
	if (err == 3)
	{
		ft_putstr_fd("bash: ", STDERR_FILENO);
		ft_putstr_fd(s, STDERR_FILENO);
		ft_putendl_fd(": command not found", STDERR_FILENO);
	}
	if (err == 4)
		ft_putendl_fd("error 4", STDERR_FILENO);
	if (err == 5)
		ft_putendl_fd("error 4", STDERR_FILENO);
	if (err == 6)
		ft_putendl_fd("error 5", STDERR_FILENO);
	if (err == 7)
		ft_putendl_fd("error 6", STDERR_FILENO);
	if (err == 8)
		ft_putendl_fd("error 7", STDERR_FILENO);
}
