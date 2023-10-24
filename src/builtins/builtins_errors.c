/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_errors.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 15:39:41 by kquerel           #+#    #+#             */
/*   Updated: 2023/10/24 18:55:31 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

/* Checks if builtins take an option or an argument (for env) */
bool	check_next_node_builtin(t_element *cmd, int option)
{
	if (option == 0)
	{
		if (cmd->next)
		{
			ft_putstr_fd(cmd->content, STDERR_FILENO);
			ft_putendl_fd(" cannot take arguments", STDERR_FILENO);
			return (false);
		}
	}
	else
	{
		if (cmd->next && cmd->next->type == OPTION)
		{
			ft_putstr_fd(cmd->content, STDERR_FILENO);
			ft_putendl_fd(" cannot take options", STDERR_FILENO);
			return (false);
		}
	}
	return (true);
}
