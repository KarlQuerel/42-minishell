/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: casomarr <casomarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 12:42:47 by octonaute         #+#    #+#             */
/*   Updated: 2023/12/02 19:04:24 by casomarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*replace_dollar(char *content, char *key_to_find, t_env *env_list)
{
	t_env	*key_in_env;
	char *ret;

	ret = NULL;
	if (is_key_in_env(env_list, key_to_find) == true)
	{
		key_in_env = find_value_with_key_env(env_list, key_to_find);
		ret = ft_strdup(key_in_env->value);;
	}
	else
	{
		if (ret && ft_strncmp(ret, "$.", 2) != 0 && ft_strlen(ret) == 2) //?
		{
			//free(content);
			content = NULL;
		}
	}

/* 	if (key_to_find != NULL && (ft_strncmp(key_to_find, "EXIT_STATUS", ft_strlen(key_to_find)) != 0 || \
	ft_strlen(key_to_find) != ft_strlen("EXIT_STATUS")))
		free(key_to_find); //pourquoi faut pas tjrs le free???? */
	return (ret);
}

void	new_key(size_t *i, char **key_to_find, char *content)
{
	int	start;
	bool	alpha;

	if (content[(*i)] == '\0') //pas sûre
	{
		if (*key_to_find)
			free(*key_to_find);
		*key_to_find = NULL;
		return ;
	}
	(*i)++;
	start = *i;
	alpha = false;
	while (content[(*i)])
	{
		if (content[(*i)] == '$' && content[(*i) + 1] == '$')
		{
			while(content[(*i) + 1] == '$')
				(*i)++;
		}
		if (ft_isalpha(content[(*i)]) == 1) // if true
			alpha = true;
		if (content[(*i)] == '$' && content[(*i) + 1] != '\0')
			break;
		(*i)++;
	}

	if (alpha == true)
	{
		if (*key_to_find)
			free(*key_to_find);
		*key_to_find = NULL;
		*key_to_find = strlcpy_middle(*key_to_find, content, start, *i - 1);
	}
	
}

/*
1. Checks if the $ is followed by alphanumeric
2. Puts what follows the $ into the variable key_to_find.
3. Cheks that the key exists in env_list (and key_in_env "takes its rank" if true).
4. Replaces the content by the key value.
*/
char	*dollar(char *content, t_env *env_list)
{
	char	*key_to_find;
	char	*ret;
	size_t	i;
	bool	alpha;
	int		j;
	
	ret = NULL;
	key_to_find = NULL;
	alpha = false;
	if (compare(content, "$?") == true)
	{
		key_to_find = "EXIT_STATUS";
		alpha = true;
	}
	if (compare(content, "$.") == true)
		return (ret);
	i = 0;
	while (content[i])
	{
		if (content[i] == '$' && content[i + 1] == '$')
		{
			while(content[i + 1] == '$')
				i++;
		}
		if (ft_isalpha(content[i]) == 1) // if true
		{
			if (alpha == false) //premiere fois
				j = i;
			alpha = true;
		}
		if (content[i] == '$' && content[i + 1] != '\0')
			break;
		i++;
	}
	if (i != ft_strlen(content)) //if multiple $
	{
		if (content[0] != '$') // ou si j != 1 /////////////////////////////
		{
			int start = j;
			while (content[j] != '$')
				j++;
			ret = strlcpy_middle(ret, content, start, j - 1);
		} /////////////////////////////////
		key_to_find = strlcpy_middle(key_to_find, content, 1, i - 1);
		while (i < ft_strlen(content)) //recursive
		{
			ret = ft_strjoin_free(ret, replace_dollar(content, key_to_find, env_list));
			new_key(&i, &key_to_find, content);
		}
		ret = ft_strjoin_free(ret, replace_dollar(content, key_to_find, env_list));	
	}
	else
	{
		if (alpha == true)
		{
			if (key_to_find == NULL)
				key_to_find = strlcpy_middle(key_to_find, content, 1, ft_strlen(content) - 1);
			if (content[0] != '$') // ou si j != 1  ////////////////////////////////
			{
				int start = j;
				while (content[j] != '$')
					j++;
				ret = strlcpy_middle(ret, content, start, j - 1);
			}///////////////////////////////////
			ret = replace_dollar(content, key_to_find, env_list);
		}
		else
		{
			free(content);
			content = NULL;
			return (NULL);
		}
	}
	free (content);
	content = NULL;
	 if (key_to_find != NULL && compare(key_to_find, "EXIT_STATUS") == false)
		free(key_to_find);
	return (ret);
}