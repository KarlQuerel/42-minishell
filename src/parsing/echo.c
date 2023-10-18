/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: casomarr <casomarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 12:42:35 by octonaute         #+#    #+#             */
/*   Updated: 2023/10/18 15:04:08 by casomarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

/*Returns the delimiter to look for depending
on the type of string.*/
char	type_of_str(char *line, int i)
{
    char	type;
    
    if (line[i] == '\'')
        type = '\'';
    else if (line[i] == '\"')
        type = '\"';
    else
        type = ' ';
    return (type);
}

int	type_of_str_2(char *cmd)
{
    if (cmd[0] == '\'' && cmd[ft_strlen(cmd) - 1] == '\'')
        return (STR);
    else if (cmd[0] == '\"' && cmd[ft_strlen(cmd) - 1] == '\"')
        return (STR);
    else
        return (CMD);
}

/*Skips quotes and spaces in the command line.*/
int skip(char *line, int i, int option)
{
    if (option == SPACES)
    {
        while(line[i] == ' ' && line[i])
            i++;
    }
    if (option == QUOTES)
    {
        if (line[i] == '\'' || line[i] == '\"')
            i++;
    }
    return (i);
}

void   print_skiping_quotes(char *str)
{
    size_t i;
    
    i = 1;
    while (i < ft_strlen(str) - 1)
        printf("%c", str[i++]);
}

/*CARACTERES QUI NECESSITENT UN \ POUR ETRE ECHO CORRECTEMENT
En mettant chaque signe au milieu de ab et en les faisant echo :
# a#b
( syntax error near unexpected token `('
) syntax error near unexpected token `)'
* a*b
; a (\n) b: command not found
< a puis attend
> attend
` (heredoc)
~ a~b
" (heredoc)
\ ab
*/

/*Prints the arguments that follow the echo command until
the next cmd is a pipe or equals NULL.*/
void	echo(t_element *current)
{
	/*GERER L'OPTION -n : 
	if(line[i] == ' ' && line[i + 1] == '-' && line[i + 2] == 'n' && line[i + 3] == ' '
		i+=3 //ou i+=4 ?*/
	int	type;
    
    if (current->next == NULL)
    {
        printf("\n");
        return;
    }
    current = current->next; 
 	while(current != NULL && current->type != PIPE)
	{
        type = type_of_str_2(current->content);
        if (type == STR)
            print_skiping_quotes(current->content);
        else
	        printf("%s", current->content);
        if (current->next != NULL)
            printf(" ");
        current = current->next;
	}
    printf("\n");
}