/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: casomarr <casomarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 17:08:27 by casomarr          #+#    #+#             */
/*   Updated: 2023/10/17 16:16:39 by casomarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

/*Malloc for the new command line that replaces the original
command line after all the extra spaces are deleted.*/
char	*erase_spaces_malloc(char *line)
{
	char	*new_line;
	int		i;
	int		spaces;

	i = 0;
	spaces = 0;
	while (line[i])
	{
		if (line[i] == '\'' && quotes_can_close(line) == true) //pour ne pas compter les espaces entre quotes (ex: dans les str de echo)
		{
			while(line[i] != '\'')
				i++;
		}
		if (line[i] == '\"' && quotes_can_close(line) == true)
		{
			while(line[i] != '\"')
				i++;
		}
		if(line[i] == ' ' && (line[i + 1] == ' ' || line[i + 1] == '\0'))
			spaces++;
		i++;
	}
	new_line = malloc(sizeof(char) * (ft_strlen(line) - spaces) + 1); // +2 au lieu de +1 a regle des pb de valgrind
	if (!new_line)
	{
		perror("Error");
		return (NULL); //il faut qd meme un return qd on utilise perror??
	}
	return (new_line);
}

/*Malloc for the joinstr found in the ft_joinstr_minishell function.*/
char	*joinstr_minishell_malloc(char *line, int len, char *str, char type)
{
	char	*new_str;

	if (type == '\'' || type == '\"')
	{
		if (str == NULL)
			new_str = ft_calloc(size_of_command(line, len, STR) \
			+ 1, sizeof(char));
		else
			new_str = ft_calloc((ft_strlen(str) + \
			size_of_command(line, len, STR) + 1), sizeof(char));
	}
	else
	{
		if (str == NULL)
			new_str = ft_calloc(size_of_command(line, len, CMD) \
			+ 1, sizeof(char));
		else
			new_str = ft_calloc((ft_strlen(str) \
			+ size_of_command(line, len, CMD) + 1), sizeof(char));
	}
	if (!new_str)
		return (NULL);
	return (new_str);
}
