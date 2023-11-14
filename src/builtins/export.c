/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karl <karl@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 13:55:33 by casomarr          #+#    #+#             */
/*   Updated: 2023/11/14 23:50:42 by karl             ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

/*
	TO DO -> a regarder sur bash --posix a 42 des le defreeze
	--> example : variable=hello
	variable =hello -> ne doit pas marcher
	varable = hello -> ne doit pas marcher
	variable= hello -> donne "variable="
*/
/* Reproduces the export command */
int	ft_export(t_element *cmd, t_env *env)
{
	char	**new_key_var;

	if (cmd->next == NULL)
	{
		ft_env(env, cmd, 1);
		return (0);
	}
	while (cmd && cmd->next)
	{
		if (!ft_is_valid_key_var(cmd->next->content))
		{
			ft_putstr_fd("export: ", STDOUT_FILENO);
			ft_putstr_fd(cmd->next->content, STDOUT_FILENO);
			ft_putendl_fd(": not a valid identifier", STDOUT_FILENO);
			return (0);
		}
		else if (ft_strchr(cmd->next->content, '='))
		{
			new_key_var = split_var(cmd->next->content);
			join_new_var(env, new_key_var[0], new_key_var[1]);
		}
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

	ret = malloc(sizeof(ret) * 3); // 3 car ca sera toujours 3 string, variable, hello et la string vide
	if (!ret)
	{
		ft_putendl_fd("Malloc failed", STDERR_FILENO);
		return (NULL);
	}
	position_equal = ft_strchr(s, '='); // on prend la position du egal
	ret[0] = ft_substr(s, 0, position_equal - s); // on ecrit le premier argument avant le egal "variable"
	ret[1] = ft_substr(position_equal, 1, ft_strlen(position_equal)); // on passe le egal et on ecrit le mot apres "hello"
	ret[3] = NULL;
	return (ret);
}

/* If the variable already exits in env, free the value and replace it */
void	join_new_var(t_env *env, char *key, char *value)
{
	t_env	*head;
	char	*new_var;

	if (value == NULL)
		value = "\0";
	head = env;
	if (!is_key_in_env(env, key))
	{
		put_var_in_env(env, key, value);
		return ;
	}
	replace_var(env, value);
	env = head;
	return ;
}

/* Replaces the existing var with a new value */
void	replace_var(t_env *env, char *value)
{
	free(env->value);
	env->value = NULL;
	env->value = value;
}

/* Puts the new variable at the end of the environment */
void	put_var_in_env(t_env *env, char *key, char *value)
{
	t_env	*new_node;

	while (env->next) // je veux juste arriver a la fin de env
		env = env->next; //	 env est vide
	new_node = ft_calloc(1, sizeof(t_env));
	if (!new_node)
		return ;
	new_node->key = key;
	new_node->value = value;
	new_node->next = NULL;
	new_node->prev = env->prev;
}
