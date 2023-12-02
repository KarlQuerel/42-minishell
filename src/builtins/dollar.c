/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: casomarr <casomarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 12:42:47 by octonaute         #+#    #+#             */
/*   Updated: 2023/12/02 23:58:23 by casomarr         ###   ########.fr       */
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
		return (0);
	if (content[(*i)] == '$' && content[0] == '$')
		return (1);
	if (content[(*i)] == '$' && *dollar_nb == 1)
		return (1);
	if (content[(*i)] == '$')
		*dollar_nb = 1;
	(*i)++;
	return (0);
}

int	initialize_values(char *content, size_t *i, size_t *j)
{
	int	dollar_nb;

	dollar_nb = 0;
	while (content[(*i)])
	{
		if (initialize_values_loop(content, i, j, &dollar_nb) == 1)
			return (1);
	}
	return (0);
}

void	multiple_dollars(char *content, size_t i, char **ret, t_env *env)
{
	char	*key_to_find;
	char	*replaced;
	int		j;

	key_to_find = NULL;
	replaced = NULL;
	j = 0;
	while (content[j] != '$')
		j++;
	if (compare(key_to_find, "?") == true)
		key_to_find = ft_strdup("EXIT_STATUS");
	else
		key_to_find = strlcpy_middle(key_to_find, content, j + 1, i - 1);
	while (i < ft_strlen(content))
	{
		replaced = replace_dollar(key_to_find, env);
		(*ret) = ft_strjoin_free((*ret), replaced);
		free(replaced);
		new_key(&i, &key_to_find, content);
	}
	replaced = replace_dollar(key_to_find, env);
	(*ret) = ft_strjoin_free((*ret), replaced);
	free(replaced);
	free(key_to_find);
}

void	one_dollar(char *content, char **ret, t_env *env)
{
	size_t	i;
	char	*key_to_find;
	char	*replaced;

	i = 0;
	key_to_find = NULL;
	replaced = NULL;
	while (content[i] != '$')
		i++;
	if (is_in_line(content, "$?") == true)
		key_to_find = "EXIT_STATUS";
	else
		key_to_find = strlcpy_middle(key_to_find, content, \
		i + 1, ft_strlen(content));
	replaced = replace_dollar(key_to_find, env);
	(*ret) = ft_strjoin_free((*ret), replaced);
	free(replaced);
	if (key_to_find != NULL && compare(key_to_find, "EXIT_STATUS") == false)
		free(key_to_find);
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
	size_t	j;
	size_t	i;
	int		nb;

	ret = NULL;
	i = 1;
	j = 0;
	nb = initialize_values(content, &i, &j);
	text_before(content, &ret);
	if (nb == 1)
		multiple_dollars(content, i, &ret, env_list);
	else
		one_dollar(content, &ret, env_list);
	free(content);
	content = NULL;
	return (ret);
}
