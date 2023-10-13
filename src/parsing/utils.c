/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: casomarr <casomarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 14:50:30 by casomarr          #+#    #+#             */
/*   Updated: 2023/10/13 15:07:06 by casomarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

/*J'ai fait +2 dans les mallocs de toute cette fonction car en plus du \0
il y a potentiellement un espace de plus (dans le cas ou il y a plusieurs
strings a echo et que comme c'est calloc et non malloc on s'en fout que
ce ne soit pas le bon nombre exact je pense car au pire tout est deja des \0)*/
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
	while(str[i])
		new_str[j++] = str[i++];
	new_str[j] = '\0';
	free(str);
	return (new_str);
}


char	*ft_join_pour_cd(char *line_begining, char *path)
{
	int		i;
	int		j;
	char	*new_str;
	
	new_str = ft_calloc((ft_strlen(line_begining) + ft_strlen(path) + 1), sizeof(char));
	if (!new_str)
		return (NULL);
	i = 0;
	j = 0;
	while(line_begining[i])
	{
		new_str[j++] = line_begining[i++];
	}
	i = 0;
	while(path[i])
	{
		new_str[j++] = path[i++];
	}
	new_str[j] = '\0';
	free(line_begining);
	return (new_str);
}

char	*erase_spaces(char *line)
{
	char	*new_line;
	int		i;
	int		j;

	i = 0;
	j = 0;
	new_line = erase_spaces_malloc(line);
	while (line[i])
	{
		if (line[i] == '\'' && quotes_can_close(line) == true) //pour ne pas compter les espaces entre quotes (ex: dans les str de echo)
		{
			new_line[j++] = line[i++];
			while(line[i] != '\'' && line[i])
				new_line[j++] = line[i++];
			new_line[j++] = line[i++];
		}
		else if (line[i] == '\"' && quotes_can_close(line) == true)
		{
			new_line[j++] = line[i++];
			while(line[i] != '\"' && line[i])
				new_line[j++] = line[i++];
			new_line[j++] = line[i++];
		}
		else if(line[i] == ' ' && (line[i + 1] == ' ' || line[i + 1] == '\0'))
			i++;
		else
			new_line[j++] = line[i++];
	}
	new_line[j] = '\0';
	free(line);
	return (new_line);
}

char *strlcpy_middle(char *dst, const char *src, size_t start, size_t end)
{	
	int	i;
	
	i = 0;
	dst = malloc(sizeof(char) * (end - start + 2));
	while (src[start] && start <= end)
		dst[i++] = src[start++];
	dst[i] = '\0';
	return (dst);
}