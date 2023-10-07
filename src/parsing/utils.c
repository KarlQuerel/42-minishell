/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: casomarr <casomarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 14:50:30 by casomarr          #+#    #+#             */
/*   Updated: 2023/10/07 19:31:11 by casomarr         ###   ########.fr       */
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

    // printf("%sDANS JOINSTR%s\n", YELLOW, RESET);
    // printf("%stype = [%c]%s\n", GREEN, type, RESET);
    
    if (type == '\'' || type == '\"')
    {
        if (str == NULL)
        {
            new_str = ft_calloc(size_of_command(line, len, STR) \
            + 2, sizeof(char));
            return (new_str);
        }
        else
            new_str = ft_calloc((ft_strlen(str) + \
            size_of_command(line, len, STR) + 2), sizeof(char));
    }
    else
    {
        if (str == NULL)
        {
            new_str = ft_calloc(size_of_command(line, len, CMD) \
            + 2, sizeof(char));
            return (new_str);
        }
        else
            new_str = ft_calloc((ft_strlen(str) \
            + size_of_command(line, len, CMD) + 2), sizeof(char));
    }
    if (!new_str)
		return (NULL);
	i = 0;
	j = 0;
    while(str[i])
    {
        new_str[j++] = str[i++];
    }
    new_str[j] = '\0';
    if (str != NULL)
	    free(str);
	return (new_str);
}