/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_and_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 21:18:09 by kquerel           #+#    #+#             */
/*   Updated: 2023/11/29 21:40:00 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* Badass welcome message made with love */
void	ft_welcome(void)
{
	printf("%s%s\n\n", BGRE, PC);
	printf("%s", WHT);
}

/* Init for exec struct */
t_pipe	*init_exec()
{
	t_pipe*	exec;

	exec = ft_calloc(1, sizeof(t_pipe));
	if (!exec)
	{
		msg_error(1, "");
		exit(EXIT_FAILURE);
	}
	return (exec);
}
