/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_types.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: casomarr <casomarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 18:58:39 by casomarr          #+#    #+#             */
/*   Updated: 2023/10/12 19:17:19 by casomarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

int	cmd_type(char *command, int len)
{
    int	size;

	size = 0;
    // printf("%slen = %d, command[len] = %c\n%s", YELLOW, len, command[len], RESET);
    while (command[len] != ' ' && command[len] != '\0')
    {
        //printf("%scommand[len] = %c\n%s", YELLOW, command[len], RESET);
        //printf("%ssize = %d, command[len] = %c\n%s", YELLOW, size, command[len], RESET);
        size++;
        len++;
    }
    // printf("%ssize_of_command dans sizeofcmd = %d\n%s", YELLOW, size + 1, RESET);
    //printf("%sTOTAL LEN IS %d\n%s", GREEN, size, RESET);
    return (size);
}

int	key_and_value_type(char *command, int len, int type)
{
    int	size;

	size = 0;
    while (command[len] != '=')
    {
        size++;
        len++;
    }
    if (type == KEY)
        return (size);
    len++;
    while (command[len])
    {
        size++;
        len++;
    }
    return (size); //if type == VALUE
}

int	str_type(char *command, int len)
{
    int	size;

	size = 0;
    while(command[len])
    {
        if (command[len + 1] == '\0' && (command[len + 1] != '\'' || command[len + 1] != '\"'))
            break;
        size++;
        len++;
    }
    return (size);
}
