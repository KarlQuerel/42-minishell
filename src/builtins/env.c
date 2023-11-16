/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: octonaute <octonaute@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 18:21:09 by karl              #+#    #+#             */
/*   Updated: 2023/11/16 15:29:52 by octonaute        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

/* Reproduces the env command 
if option == 1, 
reproduces export behaviour when used without an argument
*/
void	ft_env(t_env *env, t_element *cmd, int option)
{
	if (!check_next(cmd, 0))
		return ;
	while (env)
	{
		if (option == 1)
			ft_putstr_fd("export ", STDOUT_FILENO);
		ft_putstr_fd(env->key, STDOUT_FILENO);
		ft_putstr_fd("=", STDOUT_FILENO);
		ft_putendl_fd(env->value, STDOUT_FILENO);
		env = env->next;
	}
}
