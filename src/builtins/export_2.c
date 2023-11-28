/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: casomarr <casomarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 17:04:29 by kquerel           #+#    #+#             */
/*   Updated: 2023/11/28 19:41:27 by casomarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* ft_export_continued */
void	ft_export_continued(t_element *cmd, t_env **env)
{
	char **new_key_var;

	if (cmd && !ft_is_valid_key_var(cmd->content))
	{
		ft_putstr_fd("export: ", STDOUT_FILENO);
		ft_putstr_fd(cmd->content, STDOUT_FILENO);
		ft_putendl_fd(": not a valid identifier", STDOUT_FILENO);
		return ;
	}
	else if (cmd && ft_strchr(cmd->content, '='))
	{
		new_key_var = split_var(cmd->content);
		join_new_var(env, new_key_var[0], new_key_var[1]);
		//free(new_key_var[0]);
		//free(new_key_var[1]);
		free (new_key_var);
	}
}
/* Puts the new variable at the end of the environment */
void	put_var_in_env(t_env **env, char *key, char *value)
{
	t_env	*new_node;
	t_env	*head;
	
	head = *env;
	while ((*env)->next)
		*env = (*env)->next;
	new_node = ft_calloc(1, sizeof(t_env));
	if (!new_node)
		return ;
	(*env)->next = new_node;
	(*env)->next->prev = (*env);
	(*env) = (*env)->next;
	(*env)->key = key;
	(*env)->value = value;
	(*env)->next = NULL;
	*env = head;
	// //A VOIR
	// free(key);
	// free(value);
	// //FIN
}

/* ft_strchr modified to returns an int */
int	ft_strchr_int(char *s, char c)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == c)
			return (i);
		i++;
	}
	return (-2);
}
