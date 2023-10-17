/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: casomarr <casomarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 12:42:35 by octonaute         #+#    #+#             */
/*   Updated: 2023/10/17 16:45:48 by casomarr         ###   ########.fr       */
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

/*Prints the arguments that follow the echo command until
a pipe or the end of the command line are encountered.*/
void	echo(char *line)
{
	/*GERER L'OPTION -n : 
	if(line[i] == ' ' && line[i + 1] == '-' && line[i + 2] == 'n' && line[i + 3] == ' '
		i+=3 //ou i+=4 ?*/
	int		i;
	int		j;
	char	*str;
	char	type;
	
	str = NULL;
	i = where_is_cmd_in_line(line, "echo"); //on est sur l'espace
	if (i == 0)
		return ; //error
	if (quotes_can_close(line) == false)
	{
		printf("Error : quotes don't close\n"); //bash n'ecrit pas erreur mais je ne peux pas reproduire le > qui apparait
		return ;
	}
    if (line[i] == '\0')
    {
        printf("\n");
        return;
    }
	j = 0;
	while(line[i] && line[i] != '|')
	{
        i = skip(line, i, SPACES);
        type = type_of_str(line, i);
        i = skip(line, i, QUOTES);
        i = skip(line, i, QUOTES); //deux fois de suite pour sauter strings vides
		// printf("apres avoir passe la premiere apostrophe (si il y en a) : [%c]\n", line[i]);
        // printf("type : [%c]\n", type);
        // printf("premiere string commence par : [%c]\n", line[i]);
        str = ft_joinstr_minishell(line, i, str, type);
        j = ft_strlen(str);
        while(line[i] != type && line[i])
        {
            if (type == ' ' && line[i] == '\\') //pour le test echo hola\ncaro -> doit donner holancaro
                i++;
            else
                str[j++] = line[i++];
        }
        str[j] = '\0';
        // printf("%smot : [%s]%s\n", GREEN, str, RESET);
        i = skip(line, i, QUOTES);
        if (line[i] == ' ')
            str[j++] = line[i++];
	}
	str[j] = '\0';
	printf("%s\n", str);
	free(str);
	// printf("ligne en entier : [%s]/n", str);
	//AVANT de le print il faut pouvoir appliquer l'option -n
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