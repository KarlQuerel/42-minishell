/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: casomarr <casomarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 12:42:35 by octonaute         #+#    #+#             */
/*   Updated: 2023/10/18 15:25:14 by casomarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

void   print_skiping_quotes(char *str)
{
    size_t i;
    
    i = 1;
    while (i < ft_strlen(str) - 1)
        printf("%c", str[i++]);
}

int	type_of_str(char *cmd)
{
    if (cmd[0] == '\'' && cmd[ft_strlen(cmd) - 1] == '\'')
        return (STR);
    else if (cmd[0] == '\"' && cmd[ft_strlen(cmd) - 1] == '\"')
        return (STR);
    else
        return (CMD);
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
        type = type_of_str(current->content);
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