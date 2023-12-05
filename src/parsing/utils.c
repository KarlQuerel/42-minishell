/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 14:50:30 by casomarr          #+#    #+#             */
/*   Updated: 2023/12/05 18:51:57 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*Function used in the echo function to join all the arguments encountered
between the "echo" cmd and a pipe or the end of the command line.*/
char	*ft_joinstr_minishell(char *line, int len, char *str, char type)
{
	int		i;
	int		j;
	char	*new_str;

	new_str = joinstr_minishell_malloc(line, len, str, type);
	if (str == NULL)
		return (new_str);
	i = 0;
	j = 0;
	while (str[i])
		new_str[j++] = str[i++];
	new_str[j] = '\0';
	free(str);
	return (new_str);
}

/*Function used in the cd function to join all the arguments encountered
between the "cd" cmd and a pipe or the end of the command line : it is 
needed in the case where a folder's name has spaces.*/
char	*ft_join_for_cd(char *line_begining, char *path)
{
	int		i;
	int		j;
	char	*new_str;

	new_str = ft_calloc((ft_strlen(line_begining) + \
	ft_strlen(path) + 1), sizeof(char));
	if (!new_str)
		return (NULL);
	i = 0;
	j = 0;
	while (line_begining[i])
	{
		new_str[j++] = line_begining[i++];
	}
	i = 0;
	while (path[i])
	{
		new_str[j++] = path[i++];
	}
	new_str[j] = '\0';
	free(line_begining);
	return (new_str);
}

/*Same as the strlcpy function except it takes a start parameter in addition
to an end parameter.*/
char	*strlcpy_middle(char *dst, const char *src, size_t start, size_t end)
{
	int	i;

	i = 0;
	if (start > end)
		start = end;
	dst = ft_calloc(end - start + 2, sizeof(char));
	while (src[start] && start <= end)
		dst[i++] = src[start++];
	dst[i] = '\0';
	return (dst);
}

char	*ft_strjoin_free(char const *s1, char *s2)
{
	int		i;
	int		j;
	int		ft_strlen_total;
	char	*new_str;

	if (s2 == NULL)
		return ((char *)s1);
	if (s1 == NULL)
		ft_strlen_total = ft_strlen(s2);
	else
		ft_strlen_total = ft_strlen(s1) + ft_strlen(s2);
	new_str = malloc((sizeof(char)) * (ft_strlen_total + 1));
	if (new_str == NULL)
		return (NULL);
	i = 0;
	if (s1 != NULL)
		str_join_fill(s1, new_str, &i);
	j = 0;
	while (s2[j])
		new_str[i++] = s2[j++];
	new_str[i] = '\0';
	free((char *)s1);
	return (new_str);
}

void	str_join_fill(const char *s1, char *new_str, int *i)
{
	while (s1[(*i)])
	{
		new_str[(*i)] = s1[(*i)];
		(*i)++;
	}
}
