/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 19:53:28 by casomarr          #+#    #+#             */
/*   Updated: 2023/11/27 13:58:50 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*ft_joinstr2(char *s1, char *s2)
{
	int		i;
	int		j;
	char	*new_str;

	if (s2 == NULL || s2[0] == '\0')
		return (s1);
	if (s1 == NULL)
		new_str = malloc(sizeof(char) * (ft_strlen(s2) + 1));
	else
		new_str = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (new_str == NULL)
		return (NULL);
	i = 0;
	j = 0;
	if (s1 != NULL)
	{
		while (s1[i])
			new_str[j++] = s1[i++];
		j = 0;
	}
	while (s2[j])
		new_str[i++] = s2[j++];
	new_str[i] = '\0';
	free(s1);
	return (new_str);
}

char	*ft_strchr2(char *s, char c)
{
	size_t	i;

	i = 0;
	if (s == NULL)
		return (NULL);
	if (c == '\0')
	{
		while (s[i] != '\0')
			i++;
		return (&s[i]);
	}
	while (s[i] != '\0')
	{
		if (s[i] == c)
			return (&s[i]);
		i++;
	}
	return (NULL);
}

static char	*str_in_memory(char *big)
{
	size_t	i;
	size_t	j;
	char	*in_memory;

	i = 0;
	while (big[i] != '\n' && big[i] != '\0')
		i++;
	if (big[i] != '\0')
	{
		in_memory = malloc((ft_strlen(big) - (i + 1) + 1) * sizeof(char));
		if (in_memory == NULL)
			return (NULL);
	}
	else
	{
		free(big);
		return (NULL);
	}
	i += 1;
	j = 0;
	while (big[i])
		in_memory[j++] = big[i++];
	in_memory[j] = '\0';
	free(big);
	return (in_memory);
}

static char	*str_to_print(char *big)
{
	size_t	i;
	char	*to_print;

	i = 0;
	while (big[i] != '\n' && big[i] != '\0')
		i++;
	to_print = malloc(sizeof(char) * (i + 2));
	if (to_print == NULL)
		return (NULL);
	i = 0;
	while (big[i] != '\n' && big[i] != '\0')
	{
		to_print[i] = big[i];
		i++;
	}
	if (big[i] == '\n')
	{
		to_print[i] = big[i];
		i++;
	}
	to_print[i] = '\0';
	return (to_print);
}

static char	*until_newline(int fd, char *big)
{
	int		bytes_read;
	char	*buffer;

	buffer = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (buffer == NULL)
		return (NULL);
	bytes_read = 1;
	while (ft_strchr2(big, '\n') == NULL && bytes_read > 0)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read == -1)
		{
			free(buffer);
			return (NULL);
		}
		buffer[bytes_read] = '\0';
		big = ft_joinstr2(big, buffer);
	}
	free(buffer);
	return (big);
}

char	*get_next_line(int fd)
{
	char		*to_print;
	static char	*big[1024];

	if (fd == -1 || BUFFER_SIZE <= 0)
		return (NULL);
	big[fd] = until_newline(fd, big[fd]);
	if (big[fd] == NULL)
		return (NULL);
	to_print = str_to_print(big[fd]);
	big[fd] = str_in_memory(big[fd]);
	if (big[fd])
	{
		if (big[fd][0] == '\0')
		{
			free(big[fd]);
			big[fd] = NULL;
		}
	}
	return (to_print);
}
