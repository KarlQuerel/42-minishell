/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 18:54:48 by kquerel           #+#    #+#             */
/*   Updated: 2023/12/02 15:06:04 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* Transforms a t_env list in an array of strings */
char	**ft_transform_env(t_env *env)
{
	int		i;
	char	**new;
	t_env	*head;

	head = env;
	i = 0;
	while (head)
	{
		head = head->next;
		i++;
	}
	new = ft_calloc(i + 1, sizeof(char *));
	i = 0;
	while (env)
	{
		new[i] = ft_strjoin_env(env->key, env->value);
		env = env->next;
		i++;
	}
	new[i] = NULL;
	return (new);
}

/* Modified strjoin adding "=" between s1 and s2 */
char	*ft_strjoin_env(char const *s1, char const *s2)
{
	int		i;
	int		j;
	int		ft_strlen_total;
	char	*new_str;

	if (s1 == NULL || s2 == NULL)
		return (NULL);
	ft_strlen_total = ft_strlen(s1) + ft_strlen(s2) + 1 + 1;
	new_str = ft_calloc(ft_strlen_total, sizeof(char));
	if (new_str == NULL)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		new_str[i] = s1[i];
		i++;
	}
	new_str[i++] = '=';
	j = 0;
	while (s2[j])
		new_str[i++] = s2[j++];
	new_str[i] = '\0';
	return (new_str);
}

/* Joins every splitted string from the PATH= env variable with the command
	then tests its validity with access */
char	*ft_get_command(char **path, char *argument)
{
	char	*to_free;
	char	*to_return;
	int		i;

	if (is_only_dots(argument) == true)
		return (NULL);
	i = 0;
	if (argument && path)
	{
		while (path[i])
		{
			to_free = ft_strjoin(path[i], "/");
			to_return = ft_strjoin(to_free, argument);
			free(to_free);
			to_free = NULL;
			if (access(to_return, F_OK) == 0)
				return (to_return);
			free(to_return);
			to_return = NULL;
			i++;
		}
	}
	return (NULL);
}

/* Checks if a command is only "." or ".." */
bool	is_only_dots(char *s)
{
	if (ft_strncmp(s, "..", ft_strlen("..")) == 0 \
		&& ft_strlen(s) == ft_strlen(".."))
		return (true);
	if (ft_strncmp(s, ".", ft_strlen(".")) == 0 \
		&& ft_strlen(s) == ft_strlen("."))
		return (true);
	return (false);
}
