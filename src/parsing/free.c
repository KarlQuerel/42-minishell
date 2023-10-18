/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/30 13:41:24 by casomarr          #+#    #+#             */
/*   Updated: 2023/10/18 15:11:09 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

void	final_free(char *line, t_env *env_list, t_env *path, char *new_path)
{
	free(line);
	history(FREE_HISTORY);
	while (env_list->next != NULL)
	{
		free(env_list->key);
		free(env_list->value);
		env_list = env_list->next;
	}
	free(env_list);
	free(path);
	free(new_path);
}

void	free_cmd_list(t_element *cmd_list)
{
	
	// // KARL -> j'ai ajoute ca pour regler une seg fault
	// if (!cmd_list)
	// 	return ;
	// // fin
	
	while (cmd_list && cmd_list->next != NULL)
	{
		free(cmd_list->content);
		cmd_list = cmd_list->next;
	}
	free(cmd_list);
}
