/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: casomarr <casomarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 14:50:30 by casomarr          #+#    #+#             */
/*   Updated: 2023/10/05 16:04:13 by casomarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

char	*ft_joinstr_minishell(char *line, int len, char *str, char type)
{
	int		i;
	int		j;
	char	*new_str;

    // printf("%sDANS JOINSTR%s\n", YELLOW, RESET);
    // printf("%stype = [%c]%s\n", GREEN, type, RESET);
    
    if (type == '\'' || type == '\"')
    {
        printf("%sApostrophe%s\n", YELLOW, RESET);
        if (str == NULL)
        {
            printf("%s!str%s\n", YELLOW, RESET);
            new_str = ft_calloc(size_of_command(line, len, STR) + 1, sizeof(char));
            return (new_str);
        }
        else
        {
            printf("%sstr existe%s\n", YELLOW, RESET);
            new_str = ft_calloc((ft_strlen(str) + size_of_command(line, len, STR) + 1), sizeof(char));
		    //new_str = malloc(sizeof(char) * (ft_strlen(str) + size_of_command(line, len, STR) + 1));
        }
    }
    else
    {
        printf("%sPas d'apostrophe%s\n", YELLOW, RESET);
        if (str == NULL)
        {
            new_str = ft_calloc(size_of_command(line, len, CMD) + 1, sizeof(char));
            return (new_str);
        }
        else
            new_str = ft_calloc((ft_strlen(str) + size_of_command(line, len, CMD) + 1), sizeof(char));
		//new_str = malloc(sizeof(char) * (ft_strlen(str) + size_of_command(line, len, CMD) + 1));
    }
    if (!new_str)
		return (NULL);
	i = 0;
	j = 0;
    while(str[i])
    {
        new_str[j++] = str[i++];
    }
    //new_str[(ft_strlen(str) + size_of_command(line, len, STR) + 1)] = '\0';
	free(str);
	return (new_str);
}