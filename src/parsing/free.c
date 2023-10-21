/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: octonaute <octonaute@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/30 13:41:24 by casomarr          #+#    #+#             */
/*   Updated: 2023/10/21 13:22:40 by octonaute        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

void	final_free(char *line, t_env *env_list, t_history *entries)
{
	free(line);
	free_history(entries);
	while (env_list != NULL)
	{
		free(env_list->key);
		free(env_list->value);
		env_list = env_list->next;
	}
	free(env_list);
}

void	free_cmd_list(t_element *cmd_list)
{
	// // KARL -> j'ai ajoute ca pour regler une seg fault
	// if (!cmd_list)
	// 	return ;
	// // fin
  
	while (cmd_list != NULL)
	{
		free(cmd_list->content);
		cmd_list = cmd_list->next;
	}
	free(cmd_list);
}

/* void	free_env_list(t_env *env_list)
{
	while(env_list != NULL)
	{
		free(env_list->value);
		free(env_list->key);
		env_list = env_list->next;
	}
	free(env_list);
} */
