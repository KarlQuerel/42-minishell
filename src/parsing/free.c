/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/30 13:41:24 by casomarr          #+#    #+#             */
/*   Updated: 2023/11/28 23:26:12 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* When exit is called */
void	exit_free(t_element *cmd_list, t_env **env_list, t_pipe *exec)
{
	free_cmd_list(cmd_list);
	history(FREE_HISTORY, 0);
	free_env_list(*env_list);
	close(exec->std_in);
	close(exec->std_out);
	free (*exec->line);
	free (*exec->prompt);
	free_cmd_arr(exec);
	free(exec);
}

void	ctrld_free(char *line, char *prompt, t_env *env, t_pipe *exec)
{
	int i;
	t_env	*exit_status;
	int	temp;

	i = 0;
	free(line);
	free(prompt);
	history(FREE_HISTORY, 0);
	free(exec);
	exit_status = find_value_with_key_env(env, "EXIT_STATUS");
	temp = ft_atoi(exit_status->value);
	free_env_list(env);
	exit(temp);

}

void	free_cmd_arr(t_pipe *exec)
{
	int	i;

	free(exec->cmd_tab);
	if (exec->cmd_path)
	{
		i = 0;
		while (exec->cmd_path[i])
			free(exec->cmd_path[i++]);
		free(exec->cmd_path);
	}
	// exec->env_execve = NULL;
	if (exec->env_execve)
	{
		i = 0;
		while (exec->env_execve[i])
			free(exec->env_execve[i++]);
		free(exec->env_execve);
	}
}

/*Goes back to the head of cmd_list if needed and
frees everything.*/
void	free_cmd_list(t_element *cmd_list)
{
	t_element	**head;

	head = &cmd_list;
	while (cmd_list->prev != NULL)
		cmd_list = cmd_list->prev;
	while (cmd_list != NULL)
		ft_delete_node_cmd(head, cmd_list); //ca fait cmd = cmd->next
	free(cmd_list);
}

void	free_env_list(t_env *env_list)
{
	t_env	**head;

	head = &env_list;
	while (env_list != NULL)
	{
		// free(env_list->value);
		// free(env_list->key);
		// env_list = env_list->next;
		ft_delete_node_env(head, env_list);
	}
	free(env_list);
}

//free(exec->hd_filename);
//exec->hd_filename = NULL;
