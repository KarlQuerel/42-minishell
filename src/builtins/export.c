/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 13:55:33 by casomarr          #+#    #+#             */
/*   Updated: 2023/11/23 15:44:22 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

/* Reproduces the export command */
int	ft_export(t_element *cmd, t_env **env)
{
	char	**new_key_var;

	if (cmd->next == NULL)
	{
		ft_env((*env), cmd, 1);
		return (0);
	}
	cmd = cmd->next;
	while (cmd && cmd->type != PIPE)
	{
		while (cmd && cmd->type >= 3)
			cmd = cmd->next;
		if (cmd && !ft_is_valid_key_var(cmd->content))
		{
			ft_putstr_fd("export: ", STDOUT_FILENO);
			ft_putstr_fd(cmd->content, STDOUT_FILENO);
			ft_putendl_fd(": not a valid identifier", STDOUT_FILENO);
			return (0);
		}
		else if (cmd && ft_strchr(cmd->content, '='))
		{
			new_key_var = split_var(cmd->content);
			join_new_var(env, new_key_var[0], new_key_var[1]);
			free (new_key_var);
		}
		if (cmd)
			cmd = cmd->next;
	}
	return (1);
}

/* Checks if the argument is a valid name for a variable*/
bool	ft_is_valid_key_var(char *s)
{
	int	i;

	i = 0;
	if (!ft_isalpha(s[i]) && s[i] != '_')
		return (false);
	while (s[i] && s[i] != '=')
	{
		if (!ft_isalnum(s[i]) && s[i] != '_')
			return (false);
		i++;
	}
	return (true);
}

/* Splits the command to fit the env struct */
char	**split_var(char *s)
{
	char	**ret;
	char	*position_equal;

	ret = ft_calloc(3, sizeof(ret));
	if (!ret)
	{
		ft_putendl_fd("Malloc failed", STDERR_FILENO);
		return (NULL);
	}
	position_equal = ft_strchr(s, '=');
	ret[0] = strlcpy_middle(ret[0], s, 0, position_equal - s - 1);
	ret[1] = strlcpy_middle(ret[1], position_equal, 1, ft_strlen(position_equal));
	ret[2] = NULL;
	return (ret);
}

/* If the variable already exits in env, free the value and replace it */
void	join_new_var(t_env **env, char *key, char *value)
{
	if (value == NULL)
		value = "\0";
	if (!is_key_in_env(*env, key))
	{
		put_var_in_env(env, key, value);
		return ;
	}
	replace_var(env, key, value);
	free (key);
	// free (value);
	return ;
}

/* Replaces the existing var with a new value */
void	replace_var(t_env **env, char *key, char *value)
{
	t_env	*tmp;

	tmp = *env;
	tmp = find_value_with_key_env(*env, key);
	free(tmp->value);
	tmp->value = NULL;
	tmp->value = value;
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
}
