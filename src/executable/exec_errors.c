/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_errors.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karl <karl@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 16:24:57 by kquerel           #+#    #+#             */
/*   Updated: 2023/10/23 23:01:13 by karl             ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

/* Redirects msg errors */
void	msg_error(int err)
{
	ft_putstr_fd("bash: ", STDERR_FILENO);
	if (err == 0)
		ft_putendl_fd("error 0", STDERR_FILENO);
	if (err == 1)
		ft_putendl_fd("error 1", STDERR_FILENO);
	if (err == 2)
		ft_putendl_fd("error 2", STDERR_FILENO);
	if (err == 3)
		ft_putendl_fd("error 3", STDERR_FILENO);
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
