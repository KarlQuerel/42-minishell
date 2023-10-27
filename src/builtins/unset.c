/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 18:25:43 by karl              #+#    #+#             */
/*   Updated: 2023/10/27 17:09:38 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

/* Reproduces the unset command */
int	ft_unset(t_element *cmd_list, t_env *env)
{
	t_env *tmp;
	
	tmp = env;
	while (cmd_list && cmd_list->next)
	{
		if (!ft_is_valid_key_var(cmd_list->next->content) || ft_strchr(cmd_list->next->content, '='))
		{
			ft_putstr_fd("unset: ", STDOUT_FILENO);
			ft_putstr_fd(cmd_list->next->content, STDOUT_FILENO);
			ft_putendl_fd("not a valid identifier", STDOUT_FILENO);
			return (0);
		}
		else
		{
			if (is_key_in_env(tmp, cmd_list->next->content) == true)
			{
				tmp = find_value_with_key_env(tmp, cmd_list->next->content);
				if (tmp)
					ft_delete_node(tmp);
			}
		}
		cmd_list = cmd_list->next;
	}
	return (1);
}

/* Delete the node passed in parameters */
void	ft_delete_node(t_env *to_delete)
{
	if (!to_delete)
		return ;
	t_env *tmp;
	if (to_delete->next)
	{
		tmp = to_delete->next;
		printf("to_delete = %s\n", to_delete->value);
		free(to_delete);
		to_delete = NULL;
		to_delete = tmp;
	}
	else
	
	return ;
}
