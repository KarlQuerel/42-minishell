/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 12:42:47 by octonaute         #+#    #+#             */
/*   Updated: 2023/12/07 22:56:07 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	initialize_values_loop(char *content, size_t *i, size_t *j, int *dollar_nb)
{
	int	first;

	first = 0;
	if (content[(*i)] == '$' && content[(*i) + 1] == '$')
	{
		while (content[(*i) + 1] == '$')
			(*i)++;
	}
	if (ft_isalpha(content[(*i)]) == 1)
	{
		if (first == 0)
			(*j) = (*i);
		first = 1;
	}
	if (content[(*i)] == '$' && content[(*i) + 1] == '\0')
		return (2);
	if (content[(*i)] == '$' && content[0] == '$')
		return (1);
	if (content[(*i)] == '$' && *dollar_nb == 1)
		return (1);
	if (content[(*i)] == '$')
		*dollar_nb = 1;
	(*i)++;
	return (0);
}

int	initialize_values(char *content)
{
	size_t	j;
	size_t	i;
	int		dollar_nb;
	int		ret;

	i = 1;
	j = 0;
	dollar_nb = 0;
	while (content[i])
	{
		ret = initialize_values_loop(content, &i, &j, &dollar_nb);
		if (ret == 1)
			return (1);
		if (ret == 2)
			return (0);
	}
	return (0);
}

void	one_dollar(char *content, char **ret, t_env *env)
{
	char	*key_to_find;
	char	*after;
	char	*replaced;
	size_t	i;
	size_t	tmp;

	i = 0;
	key_to_find = NULL;
	replaced = NULL;
	while (content[i] != '$')
		i++;
	tmp = i + 1;
	while (content[tmp] && (content[tmp] < 9 \
	|| content[tmp] > 13) && content[tmp] != 32)
		tmp++;
	after = text_after(content, &tmp);
	if (is_in_line(content, "$?") == true)
		key_to_find = "EXIT_STATUS";
	else
		key_to_find = strlcpy_middle(key_to_find, content, \
		i + 1, tmp - 1);
	replace_dollar_in_ret(ret, key_to_find, env);
	(*ret) = ft_strjoin_free((*ret), after);
	free(after);
	free_and_last_dollar(&key_to_find, content, ret);
}

/*
1. Checks if the $ is followed by alphanumeric
2. Puts what follows the $ into the variable key_to_find.
3. Checks that the key exists in env_list (and key_in_env 
   "takes its rank" if true).
4. Replaces the content by the key value.
*/
char	*dollar(char *content, t_env *env_list)
{
	char	*ret;
	int		nb;

	ret = NULL;
	nb = initialize_values(content);
	text_before(content, &ret);
	if (nb == 1)
		multiple_dollars(content, &ret, env_list);
	else
		one_dollar(content, &ret, env_list);
	free(content);
	content = NULL;
	return (ret);
}
