/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: casomarr <casomarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 12:42:35 by octonaute         #+#    #+#             */
/*   Updated: 2023/11/18 15:07:04 by casomarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

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
	bool newline;

	newline = true;
	if (current->next == NULL)
	{
		ft_putstr_fd("\n", STDOUT_FILENO);
		return;
	}
	current = current->next; 
 	while(current != NULL && current->type < 3)
	{
		if (current->type == OPTION && ft_strncmp(current->content, "-n", ft_strlen(current->content)) == 0 && \
		current->prev->type == COMMAND)
		{
			newline = false;
			current = current->next;
		}
		ft_putstr_fd(current->content, STDOUT_FILENO);
		//printf("%s", current->content); //echo 'caro ne print pas l'apostrophe, idem pour "caro
		if (current->next != NULL)
				ft_putstr_fd(" ", STDOUT_FILENO);
		current = current->next;
	}
	if (newline == true)
			ft_putstr_fd("\n", STDOUT_FILENO);
}
