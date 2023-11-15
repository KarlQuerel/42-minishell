/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karl <karl@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/30 13:41:24 by casomarr          #+#    #+#             */
/*   Updated: 2023/11/15 15:21:43 by karl             ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

/* When exit is called */
void	exit_free(t_element *cmd_list, t_env **env_list, t_pipe *exec)
{
	free_cmd_list(cmd_list);
	history(FREE_HISTORY, 0); // 0 : on s'en fout, on utilise pas len qd on free
	free_env_list(*env_list);
	free (exec->line[0]);
	free (exec->prompt[0]); //a voir
	free_cmd_tab(exec);
	free(exec);
}

void	free_cmd_tab(t_pipe *exec)
{
	int	i = 0;
	while (exec->cmd_tab[i])
		free(exec->cmd_tab[i++]);
	free(exec->cmd_tab);
}


void	free_cmd_list(t_element *cmd_list)
{
	while (cmd_list != NULL)
	{
		free(cmd_list->content);
		cmd_list = cmd_list->next;
	}
	free(cmd_list);
}

void	free_env_list(t_env *env_list)
{
	while(env_list != NULL)
	{
		free(env_list->value);
		free(env_list->key);
		env_list = env_list->next;
	}
	free(env_list);
}
