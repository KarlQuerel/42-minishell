/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 12:42:47 by octonaute         #+#    #+#             */
/*   Updated: 2023/12/07 16:43:06 by kquerel          ###   ########.fr       */
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
	if (ft_isalpha(content[(*i)]) == 1/*  && *dollar_nb == 1 */)
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

int	initialize_values(char *content, size_t *i, size_t *j)
{
	int	dollar_nb;
	int	ret;

	dollar_nb = 0;
	while (content[(*i)])
	{
		ret = initialize_values_loop(content, i, j, &dollar_nb);
		if (ret == 1)
			return (1);
		if (ret == 2)
			return (0);
	}
	return (0);
}

void	multiple_dollars(char *content, size_t i, char **ret, t_env *env)
{
	char	*key_to_find;
	char	*replaced;
	int		j; //i dans one_dollar
	char	*text_after;
	size_t	y; //j dans one_dollar
	size_t	tmp;
	size_t	after;

	key_to_find = NULL;
	replaced = NULL;
	text_after = NULL;


	printf("content[%zu] = ---%c---\n", i, content[i]);
	printf("content[%zu + 1] = ---%c---\n", i +1, content[i +1]);
	
	j = 0;
	while (content[j] && content[j] != '$')
		j++;
	if (content[j + 1] != '$' && content[j + 1])
		j++;

/* /////////////////////
	tmp = j + 1;
	while (content[tmp] && (content[tmp] < 9 || content[tmp] > 13) \
	&& content[tmp] != 32 && content[tmp] != '$')
		tmp++;
	if (content[tmp] != '\0' && content[tmp] != '$')
	{
		after = tmp;
		tmp--;
		text_after = ft_calloc(sizeof(content) + 1, sizeof(char));
		if (!text_after)
			return ;
		y = 0;
		while (content[after] && content[after] != '$')
			text_after[y++] = content[after++];
	}
//////////////////// */
	if (compare(key_to_find, "?") == true)
		key_to_find = ft_strdup("EXIT_STATUS");
	else
		key_to_find = strlcpy_middle(key_to_find, content, j, i - 1);
	printf("key_to_find_1 = ---%s---\n", key_to_find);
	while (i < ft_strlen(content))
	{
		replaced = replace_dollar(key_to_find, env);
		(*ret) = ft_strjoin_free((*ret), replaced);
		free(replaced);
/////////////////////
	printf("*ret_1 = ---%s---\n", *ret);
	if (content[i + 1] != '$' && content[i + 1] != '\0')
	{
		tmp = i + 1;
		while (content[i] && content[i] != '$')
			i++;
		after = tmp;
		text_after = ft_calloc(sizeof(content) + 1, sizeof(char));
		if (!text_after)
			return ;
		y = 0;
	printf("debut text_after = ---%c---\n", content[after]);
		
		while (content[after] && content[after] != '$')
			text_after[y++] = content[after++];
		(*ret) = ft_strjoin_free((*ret), text_after);
	printf("*ret_2= ---%s---\n", *ret);
	
	}
////////////////////
		new_key(&i, &key_to_find, content);
	printf("key_to_find_2 = ---%s---\n", key_to_find);
		
	}
	replaced = replace_dollar(key_to_find, env);
	(*ret) = ft_strjoin_free((*ret), replaced);
	free(replaced);
	free(key_to_find);

	/////////////////////
	if (content[i + 1] != '$' && content[i + 1] != '\0')
	{
		tmp = i + 1;
		while (content[i] && content[i] != '$')
			i++;
		after = tmp;
		text_after = ft_calloc(sizeof(content) + 1, sizeof(char));
		if (!text_after)
			return ;
		y = 0;
	printf("debut text_after = ---%c---\n", content[after]);
		
		while (content[after] && content[after] != '$')
			text_after[y++] = content[after++];
		(*ret) = ft_strjoin_free((*ret), text_after);
	printf("*ret_2= ---%s---\n", *ret);
	
	}
////////////////////
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
	after = text_after(content, &tmp);
	if (is_in_line(content, "$?") == true)
		key_to_find = "EXIT_STATUS";
	else
		key_to_find = strlcpy_middle(key_to_find, content, \
		i + 1, tmp);
	replaced = replace_dollar(key_to_find, env);
	(*ret) = ft_strjoin_free((*ret), replaced);
	free(replaced);
	(*ret) = ft_strjoin_free((*ret), after);
	if (after)
		free(after);
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
	i = 1; //caro avant c'etait i=1
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
