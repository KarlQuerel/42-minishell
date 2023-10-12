/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carolina <carolina@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/30 13:41:24 by casomarr          #+#    #+#             */
/*   Updated: 2023/10/11 17:17:43 by carolina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

void	final_free(char *line, t_env *env_list)
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
}

void	free_cmd_list(t_element *cmd_list)
{
	while (cmd_list->next != NULL)
	{
		free(cmd_list->content);
		cmd_list = cmd_list->next;
	}
	free(cmd_list);
}
