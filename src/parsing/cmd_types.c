/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_types.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 18:58:39 by casomarr          #+#    #+#             */
/*   Updated: 2023/10/18 18:38:10 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

/*Returns the size of a cmd.
Command = command line
len = where is the begining of the cmd in the command line*/
int	cmd_type(char *command, int len)
{
    int	size;

	size = 0;
    // printf("%slen = %d, command[len] = %c\n%s", YELLOW, len, command[len], RESET);
    
    // KARL -> j'ai ajoute ca pour regler une seg fault
	if (!command)
		return (0);
	// fin
    
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

/*Returns the size of the environment key or its value,
depending on the trigger.*/
int	key_and_value_type(char *command, int len, int type)
{
    int	size;

	size = 0;

    // KARL -> j'ai ajoute ca pour regler une seg fault
	if (!command)
		return (0);
	// fin
    
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

/*Returns the size of a str (= argument in between two quotes).
Command = command line
len = where is the begining of the string in the command line
(command[len] is the first letter of the string, not the first quote)*/
int	str_type(char *command, int len)
{
    int	size;

	size = 0;
    while(command[len])
    {
        if (command[len + 1] == '\'' || command[len + 1] == '\"')
        {
            size+=1; //car on veut num reel, en dehors de []
            break;
        }
        size++;
        len++;
    }
    //printf("str size = %d\n", size);
    return (size);
}
