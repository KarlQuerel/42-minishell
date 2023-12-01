/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 13:55:33 by casomarr          #+#    #+#             */
/*   Updated: 2023/12/01 18:33:20 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* Reproduces the export command */
int	ft_export(t_element *cmd, t_env **env)
{
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
		ft_export_continued(cmd, env);
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
	if (!ft_isalpha(s[i]) && s[i] != '_' )
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
	int		i;

	ret = ft_calloc(3, sizeof(ret));
	if (!ret)
	{
		ft_putendl_fd("Malloc failed", STDERR_FILENO);
		return (NULL);
	}
	i = ft_strchr_int(s, '=') + 1;
	if (i != -1)
		ft_assign_key_and_value(i, ret, s);
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
	free(key);
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
