/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: casomarr <casomarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/30 13:41:24 by casomarr          #+#    #+#             */
/*   Updated: 2023/11/24 17:09:01 by casomarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

/* When exit is called */
void	exit_free(t_element *cmd_list, t_env **env_list, t_pipe *exec)
{
	free_cmd_list(cmd_list);
	history(FREE_HISTORY, 0); // 0 : on s'en fout, on utilise pas len qd on free
	free_env_list(*env_list);
/* 	free (exec->line[0]);
	free (exec->prompt[0]); //a voir */
/*fAIRE UN BREAK ICI POUR VOIR CE QUI N EST PAS FREE A LA FIN DE AL FONCTION*/
	close(exec->std_in);
	close(exec->std_out);
	free (*exec->line); //NE SE FREE PAS BIEN
	free (*exec->prompt); //NE SE FREE PAS BIEN
	free_cmd_arr(exec); //NE SE FREE PAS BIEN
	free(exec);
}

void	ctrld_free(char *line, char *prompt, t_env *env, t_pipe *exec)
{
	int i;

	i = 0;
	free(line);
	free(prompt);
	free_env_list(env);
	history(FREE_HISTORY, 0);
	free(exec);
	exit(0); // g_signals.exit_status;
}

void	free_cmd_arr(t_pipe *exec)
{
	int	i;

	i = 0;
	free(exec->cmd_tab);
/* 	if (exec->cmd_tab)
	{	
		while (exec->cmd_tab[i])
			free(exec->cmd_tab[i++]);
		free(exec->cmd_tab);
	} */
	if (exec->cmd_path)
	{
		i = 0;
		while (exec->cmd_path[i])
			free(exec->cmd_path[i++]);
		free(exec->cmd_path);
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
