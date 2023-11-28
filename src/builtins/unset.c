/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: casomarr <casomarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 18:25:43 by karl              #+#    #+#             */
/*   Updated: 2023/11/28 14:51:02 by casomarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* Reproduces the unset command */
int	ft_unset(t_element *cmd, t_env **env)
{
	t_env	*tmp;

	while (cmd && cmd->type != PIPE)
	{
		tmp = *env;
		while (cmd && cmd->type >= 3)
			cmd = cmd->next;
		if (cmd && is_key_in_env(tmp, cmd->content) == true)
		{
			tmp = find_value_with_key_env(tmp, cmd->content);
			if (tmp)
				ft_delete_node_env(env, tmp);
		}
		if (cmd)
			cmd = cmd->next;
	}
	return (1);
}

/* Delete the node passed in parameters */
void	ft_delete_node_env(t_env **head, t_env *to_delete)
{
	if (!to_delete)
		return ;
	if (to_delete->prev)
		to_delete->prev->next = to_delete->next;
	else
		*head = to_delete->next;
	if (to_delete->next)
		to_delete->next->prev = to_delete->prev;
	if (ft_strncmp(to_delete->key, "EXIT_STATUS", ft_strlen(to_delete->key)) != 0)
	{
		free(to_delete->key);
		free(to_delete->value);
	}
	else
		free(to_delete->value); //test
	free(to_delete);
}

void	ft_delete_node_cmd(t_element **head, t_element *to_delete)
{
	if (!to_delete)
		return ;
	if (to_delete->prev)
		to_delete->prev->next = to_delete->next;
	else
		*head = to_delete->next;
	if (to_delete->next)
		to_delete->next->prev = to_delete->prev;
	if (to_delete->content)
		free(to_delete->content);
	free(to_delete);
}
