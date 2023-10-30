/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karl <karl@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 12:42:35 by octonaute         #+#    #+#             */
/*   Updated: 2023/10/30 15:26:37 by karl             ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

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
	/*Utiliser ft_putstrfd[] avec un fd envoye par Karl a la place 
	de tous mes printf dans tous mes builtins*/

	// si echo CARO -n, on doit print CARO -n
	int	type;
	bool newline;

	newline = true;
	if (current->next == NULL)
	{
		printf("\n");
		return;
	}
	current = current->next; 
 	while(current != NULL && current->type != PIPE)
	{
		if (current->type == OPTION && ft_strncmp(current->content, "-n", ft_strlen(current->content)) == 0 && current->prev->type == COMMAND)
		{
			newline = false;
			current = current->next;
		}
		type = type_of_str(current->content);
		if (type == STR)
			print_skiping_quotes(current->content);
		else
			printf("%s", current->content); //echo 'caro ne print pas l'apostrophe, idem pour "caro
		if (current->next != NULL)
			printf(" ");
		current = current->next;
	}
	if (newline == true)
		printf("\n");
}
