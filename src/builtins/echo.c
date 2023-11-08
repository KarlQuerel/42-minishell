/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karl <karl@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 12:42:35 by octonaute         #+#    #+#             */
/*   Updated: 2023/11/08 18:20:22 by karl             ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

void	print_skiping_quotes(char *str,/*t_pipe *exec */ int option)
{
	size_t i;
	
	i = 1;
	// while (i < ft_strlen(str) - 1)
	// 	printf("%c", str[i++]);

	while (i < ft_strlen(str) - 1)
	{
		if (option == PRINT)
			ft_putchar_fd(str[i], STDOUT_FILENO);
		// else
		// 	ft_putchar_fd(&str[i], exec->);
		i++;
	}
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
void	echo(t_element *current,/* t_pipe *exec */ int option)
{
	/*Utiliser ft_putstrfd[] avec un fd envoye par Karl a la place 
	de tous mes printf dans tous mes builtins*/

	// si echo CARO -n, on doit print CARO -n
	int	type;
	bool newline;
	char *str;

	newline = true;
	if (current->next == NULL)
	{
		if (option == PRINT)
			ft_putstr_fd("\n", STDOUT_FILENO);
		// else
		// 	ft_putstr_fd("\n", exec->fd_temp[]); // a voir
		return;
	}
	current = current->next; 
 	while(current != NULL && current->type != PIPE && current->type != INFILE && \
				current->type != INFILE_DELIMITER && \
				current->type != OUTFILE && current->type != OUTFILE_APPEND)
	{
		if (current->type == OPTION && ft_strncmp(current->content, "-n", ft_strlen(current->content)) == 0 && current->prev->type == COMMAND)
		{
			newline = false;
			current = current->next;
		}
		type = type_of_str(current->content);
		if (type == STR)
			print_skiping_quotes(current->content, option);
		else
		{
			if (option == PRINT)
				ft_putstr_fd(current->content, STDOUT_FILENO);
			// else
			// 	ft_putstr_fd(current->content, exec->fd_temp[]); // a voir
		}
			//printf("%s", current->content); //echo 'caro ne print pas l'apostrophe, idem pour "caro
		if (current->next != NULL)
		{
			if (option == PRINT)
				ft_putstr_fd(" ", STDOUT_FILENO);
			// else
			// 	ft_putstr_fd(" ", exec->fd_temp[]); // a voir
		}
			//printf(" ");
		current = current->next;
	}
	if (newline == true)
	{
		if (option == PRINT)
			ft_putstr_fd("\n", STDOUT_FILENO);
		// else
			// 	ft_putstr_fd("\n", exec->fd_temp[]); // a voir
	}
		//printf("\n");
}
