/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/02 20:32:19 by casomarr          #+#    #+#             */
/*   Updated: 2023/12/07 16:39:00 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	new_key_loop(size_t *i, char *content, int *alpha)
{
	while (content[(*i)])
	{
		if (content[(*i)] == '$' && content[(*i) + 1] == '$')
		{
			while (content[(*i) + 1] == '$')
				(*i)++;
		}
		if (ft_isalpha(content[(*i)]) == 1)
			(*alpha) = 1;
		if (content[(*i)] == '$' && content[(*i) + 1] != '\0')
			return ;
		if (content[(*i)] == '$')
			return ;
		(*i)++;
	}
}

void	new_key(size_t *i, char **key_to_find, char *content)
{
	int	start;
	int	alpha;

	if (content[(*i)] == '\0')
	{
		if (*key_to_find)
			free(*key_to_find);
		*key_to_find = NULL;
		return ;
	}
	(*i)++;
	start = *i;
	alpha = 0;
	new_key_loop(i, content, &alpha);
	if (*key_to_find)
		free(*key_to_find);
	*key_to_find = strlcpy_middle(*key_to_find, content, start, *i - 1);
	if (compare(*key_to_find, "?") == true)
	{
		free(*key_to_find);
		*key_to_find = ft_strdup("EXIT_STATUS");
	}
}

char	*replace_dollar(char *key_to_find, t_env *env_list)
{
	t_env	*key_in_env;
	char	*ret;

	ret = NULL;
	if (is_key_in_env(env_list, key_to_find) == true)
	{
		key_in_env = find_value_with_key_env(env_list, key_to_find);
		ret = ft_strdup(key_in_env->value);
	}
	return (ret);
}

void	text_before(char *content, char **ret)
{
	int	start;

	start = 0;
	if (content[0] != '$')
	{
		while (content[start] != '$')
			start++;
		(*ret) = strlcpy_middle((*ret), content, 0, start - 1);
	}
}

char	*text_after(char *content, size_t *tmp)
{
	size_t	after;
	size_t	j;
	char	*text_after;

	text_after = NULL;
	while (content[(*tmp)] && (content[(*tmp)] < 9 || content[(*tmp)] > 13) \
	&& content[(*tmp)] != 32)
		(*tmp)++;
	if (content[(*tmp)] != '\0')
	{
		after = (*tmp);
		(*tmp)--;
		text_after = ft_calloc(ft_strlen(content) + 1, sizeof(char));
		if (!text_after)
			return (NULL);
		j = 0;
		while (content[after])
			text_after[j++] = content[after++];
	}
	return (text_after);
}
