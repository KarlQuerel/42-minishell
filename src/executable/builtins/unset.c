/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karl <karl@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 18:25:43 by karl              #+#    #+#             */
/*   Updated: 2023/10/22 20:59:53 by karl             ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../../../includes/minishell.h"
#include "../../../libft/libft.h"

/* Reproduces the unset command */
int	ft_unset(t_element *cmd_list, t_env *env)
{
	t_env *tmp;
	
	tmp = env;
	while (cmd_list && cmd_list->next)
	{
		printf("cmd ->next->content = %s\n", cmd_list->next->content);
		if (!ft_is_valid(cmd_list->next->content) || ft_strchr(cmd_list->next->content, '='))
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
				printf("tmp->key = %s\n", tmp->key);
				ft_delete_node(tmp);
			}
		}
		cmd_list = cmd_list->next;
	}
	return (1);
}

// fonction inutile pour l'instant mais je laisse au cas ou
/* Removes the variable *to_remove from a list */
/*void	ft_remove_var(t_element *cmd_list, t_env *env, t_env *to_remove)
{
	if (!env || !to_remove)
		return ;
	while (cmd_list && cmd_list->next)
	{
		if (ft_strncmp(cmd_list->next->content, to_remove, ft_strlen(to_remove)))
			ft_delete_node(env->next);
		cmd_list = cmd_list->next;
	}
	return ;
}*/
/* Delete the node passed in parameters */
void	ft_delete_node(t_env *to_delete)
{
	if (!to_delete)
		return ;

	t_env *tmp;
	tmp = to_delete->next;
	if (!to_delete->next)
	{
		free(to_delete);
		to_delete = NULL;
	}
	// --> dans le cas ou je devrai transformer env en une liste doublement chainee
	// if (to_delete->prev)
	// 	to_delete->prev->next = to_delete->next;
	to_delete = tmp;
	return ;
}
