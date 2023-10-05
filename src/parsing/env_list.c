/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 17:00:17 by casomarr          #+#    #+#             */
/*   Updated: 2023/10/02 18:47:24 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

/*Cuts each line of **env into key and value in a t_env list*/
t_env	*put_env_in_list(char **env)
{
	t_env   *head;
	t_env   *current;
	int line;
	int letter;
	int i;

	i = 0;
	current = lstnew_env(env[i], 0);
	head = current;
	line = 0;
	while(env[line] != NULL)
	{
		letter = 0;
		i = 0;
		while(env[line][letter] != '=')
			current->key[i++] = env[line][letter++];
		current->key[i] = '\0';
		//printf("key = %s\n", current->key);
		letter++;
		i = 0;
		while(env[line][letter] != '\0')
			current->value[i++] = env[line][letter++];
		current->value[i] = '\0';
		//printf("value = %s\n", current->value);
		line++;
		if (env[line] != NULL)
		{
			current->next = lstnew_env(env[line], 0);
			current = current->next;
		}
	}
	current->next = NULL;
	return (head);
}

/*Parses all the list until it finds a matching key*/
t_env   *find_value_with_key_env(t_env *env_list, char *key)
{
	int i;

	i = 0;
	while (env_list != NULL)
	{
		i = 0;
		while (env_list->key[i] == key[i])
		{
			i++;
			if (key[i] == '\0')
				return (env_list);
		}
		env_list = env_list->next;
	}
	return (env_list); //ne devrait jamais arriver ici
}

/*Checks if the arg following $ in the command line has a 
matching key in **env*/
bool	is_key_in_env(t_env *env_list, char *key)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (!key || !env_list)
		return (false);
	while (env_list != NULL)
	{
		j = 0;
		while (env_list->key[i + j] == key[j] && key[j] != '\0' && env_list->key[i + j] != '\0')
		{
			j++;
			if (key[j] == '\0')
				return (true);
		}
		i++;
	}
	return (false);
}
