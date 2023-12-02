/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 17:00:17 by casomarr          #+#    #+#             */
/*   Updated: 2023/12/02 23:02:53 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	put_env_in_list_loop(char **env, t_env **current, int line)
{
	int	letter;
	int	i;

	letter = 0;
	i = 0;
	while (env[line][letter] != '=')
		(*current)->key[i++] = env[line][letter++];
	(*current)->key[i] = '\0';
	letter++;
	i = 0;
	while (env[line][letter] != '\0')
		(*current)->value[i++] = env[line][letter++];
	(*current)->value[i] = '\0';
	line++;
	if (env[line] != NULL)
	{
		(*current)->next = lstnew_env(env[line], 0);
		(*current)->next->prev = (*current);
		(*current) = (*current)->next;
	}
	return (line);
}

/*Cuts each line of **env into key and value in a t_env list*/
t_env	*put_env_in_list(char **env)
{
	t_env	*head;
	t_env	*current;
	int		line;

	current = lstnew_env(env[0], 0);
	head = current;
	line = 0;
	while (env[line] != NULL)
		line = put_env_in_list_loop(env, &current, line);
	current->next = NULL;
	return (head);
}

/*Parses all the list until it finds a matching key*/
t_env	*find_value_with_key_env(t_env *env_list, char *key)
{
	int		i;
	t_env	*current;

	i = 0;
	current = env_list;
	while (current != NULL)
	{
		i = 0;
		while (current->key[i] == key[i])
		{
			i++;
			if (key[i] == '\0')
				return (current);
		}
		current = current->next;
	}
	return (NULL);
}

/*Checks if the arg following $ in the command line has a 
matching key in **env*/
bool	is_key_in_env(t_env *env_list, char *key)
{
	t_env	*tmp;
	char	*shorter;

	if (!key || !env_list)
		return (false);
	tmp = env_list;
	if (ft_strlen(key) >= ft_strlen(tmp->key))
		shorter = tmp->key;
	else
		shorter = key;
	while (tmp)
	{
		if (compare(key, tmp->key) == true)
			return (true);
		tmp = tmp->next;
	}
	return (false);
}
