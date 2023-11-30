/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 18:21:09 by karl              #+#    #+#             */
/*   Updated: 2023/11/30 14:45:55 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
		if (ft_strncmp(env->key, "EXIT_STATUS", ft_strlen("EXIT_STATUS")) \
			== 0 && ft_strlen(env->key) == ft_strlen("EXIT_STATUS"))
			env = env->next;
		else
		{
			if (option == 1)
				ft_putstr_fd("export ", STDOUT_FILENO);
			ft_putstr_fd(env->key, STDOUT_FILENO);
			ft_putstr_fd("=", STDOUT_FILENO);
			ft_putendl_fd(env->value, STDOUT_FILENO);
			env = env->next;
		}
	}
}
