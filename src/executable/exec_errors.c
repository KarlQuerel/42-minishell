/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_errors.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 16:24:57 by kquerel           #+#    #+#             */
/*   Updated: 2023/12/02 18:21:55 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* Redirects msg errors */
void	msg_error(int err, char *s)
{
	(void)s;
	if (err == 0)
		ft_putendl_fd("No arguments accepted: run again with ./minishell", \
		STDERR_FILENO);
	else if (err == 1)
		ft_putendl_fd("Allocation failed", STDERR_FILENO);
}

/* Redirects msg errors starting with "bash" */
void	msg_error_bash(int err, char *s)
{
	ft_putstr_fd("bash: ", STDERR_FILENO);
	if (err == 0)
		ft_putstr_fd(s, STDERR_FILENO);
	else if (err == 1)
	{
		ft_putstr_fd(s, STDERR_FILENO);
		ft_putendl_fd(": command not found", STDERR_FILENO);
	}
	else if (err == 2)
	{
		ft_putstr_fd("cd: ", STDERR_FILENO);
		ft_putstr_fd(s, STDERR_FILENO);
		ft_putendl_fd(": No such file or directory", STDERR_FILENO);
	}
	else if (err == 3)
	{
		ft_putstr_fd("history: ", STDERR_FILENO);
		ft_putstr_fd(s, STDERR_FILENO);
		ft_putendl_fd(" numeric argument required", STDERR_FILENO);
	}
	else
		msg_error_bash_2(err, s);
}

/* Msg_error_bash_continued */
void	msg_error_bash_2(int err, char *s)
{
	if (err == 6)
		ft_putendl_fd(s, STDERR_FILENO);
	else if (err == 7)
		ft_putendl_fd("syntax error near unexpected token `|'", STDERR_FILENO);
	else if (err == 8)
		ft_putendl_fd("syntax error near unexpected token `||'", STDERR_FILENO);
	else if (err == 9)
		ft_putendl_fd(s, STDERR_FILENO);
}
