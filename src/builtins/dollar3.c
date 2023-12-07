/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 22:43:51 by kquerel           #+#    #+#             */
/*   Updated: 2023/12/08 00:24:25 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	first_values(size_t *end, char *content, size_t *tmp)
{
	size_t	start;
	char	*after;

	after = NULL;
	start = 0;
	while (content[start] && content[start] != '$')
		start++;
	(*end) = start + 1;
	while (content[(*end)] && ft_isalpha(content[*end]) == 1 \
	&& content[(*end)] != '$')
		(*end)++;
	(*tmp) = (*end);
	after = text_after(content, tmp);
	free (after);
	return (start);
}

char	*first_key_to_find(char *content, size_t start, size_t end)
{
	char	*key_to_find;

	key_to_find = NULL;
	if (compare(key_to_find, "?") == true)
		key_to_find = ft_strdup("EXIT_STATUS");
	else
		key_to_find = strlcpy_middle(key_to_find, content, start + 1, end - 1);
	return (key_to_find);
}

void	replace_dollar_in_ret(char **ret, char *key_to_find, t_env *env)
{
	char	*replaced;

	replaced = NULL;
	replaced = replace_dollar(key_to_find, env);
	(*ret) = ft_strjoin_free((*ret), replaced);
	free(replaced);
}

char	*final_values(size_t *start, char *content, size_t end)
{
	char	*after;

	after = NULL;
	while (content[(*start)] != '$')
		(*start)--;
	(*start)++;
	while (content[*start] && ft_isalpha(content[*start]) == 1)
		(*start)++;
	after = strlcpy_middle(after, content, *start, end);
	return (after);
}

void	multiple_dollars(char *content, char **ret, t_env *env)
{
	char	*key_to_find;
	size_t	end;
	char	*after;
	size_t	tmp;
	size_t	start;

	start = first_values(&end, content, &tmp);
	key_to_find = first_key_to_find(content, start, end);
	while (end < ft_strlen(content))
	{
		replace_dollar_in_ret(ret, key_to_find, env);
		after = text_after(content, &end);
		start = end;
		(*ret) = ft_strjoin_free((*ret), after);
		free(after);
		new_key(&end, &key_to_find, content);
	}
	replace_dollar_in_ret(ret, key_to_find, env);
	after = final_values(&start, content, end);
	(*ret) = ft_strjoin_free((*ret), after);
	free(after);
	free(key_to_find);
	if (content[ft_strlen(content) - 1] == '$' && content[ft_strlen(content)] \
	== '\0')
		(*ret) = ft_strjoin_free((*ret), "$");
}
