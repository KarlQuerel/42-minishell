/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: casomarr <casomarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/02 20:32:19 by casomarr          #+#    #+#             */
/*   Updated: 2023/12/02 20:47:48 by casomarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

char	*replace_dollar(char *content, char *key_to_find, t_env *env_list)
{
	(void)content; ///
	t_env	*key_in_env;
	char *ret;

	ret = NULL;
	if (is_key_in_env(env_list, key_to_find) == true)
	{
		key_in_env = find_value_with_key_env(env_list, key_to_find);
		ret = ft_strdup(key_in_env->value);
	}
	return (ret);
}

