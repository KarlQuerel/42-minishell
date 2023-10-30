/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karl <karl@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 19:36:13 by kquerel           #+#    #+#             */
/*   Updated: 2023/10/30 11:11:24 by karl             ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

int	ft_exit(t_element *cmd, t_env *env, t_history *entries)
{
	//exit_free(cmd, env, entries); seg fault double free
	ft_putendl_fd("exit", STDOUT_FILENO);
	exit(0); // on doit gerer les sorties
	return (0);
}
