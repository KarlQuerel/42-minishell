/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: casomarr <casomarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 12:39:52 by casomarr          #+#    #+#             */
/*   Updated: 2023/11/21 11:32:15 by casomarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

/*The first two "if" check if there are errors in the grammar of the command
line and print the associated error. The last function replaces the $ by its
associated value so that the executable receives directly the line completed.*/
bool	line_errors_and_fix(char *line) /*pourquoi double pointeur : 
avant parce que je modifiais la ligne en même temps que je voulais return
un bool mais plus besoin non? Vu que Maintenant c'est erase_spaces etc qui 
modifie line*/
{
	if (!line)
		return (NULL);
	// if (redirecters_error(*line) == false)
	// 	return (false);
	if (first_character_error(line) == false)
		return (false);
	if (pipe_double_or_eof(line) == false)
		return (false);
	return (true);
}

bool	first_character_error(char *line)
{
	bool	status;

	status = false;
	if (line[0] == '<' || line[0] == '>')
		status = redirecters_error(line);
	//KARL j'ai mis ca en commentaire pour tester /usr/bin/ls !
	// else if (line[0] == '/')
	// 	slash_error(line); //du coup je supprimes cette fonction ou pas?
	else if (line[0] == '|') //autre erreur : si les cmd avant/apres la pip ne sont pas des vrai cmd (erreur ligne 57 du google doc)
		pipe_error(line);
	else if (line[0] == '&')
		and_error(line);
	else if (line[0] == '$' && line[1] != '?')
		printf("Command not found\n"); //sur bash ca met en plus le nom du user : avant l erreur mais je sais pas si c est necessaire
	else
		status = true;
	return (status);
}

bool	redirecters_error(char *line)
{
	int	i;

	i = 0;
	if (ft_strlen(line) <= 2 && (is_cmd_in_line(line, "<") || is_cmd_in_line(line, ">")))
	{
		printf("bash: syntax error near unexpected token `newline'\n");
		return (false);
	}
	else if (ft_strlen(line) > 2)
	{
		// if (line[0] == '>' && line[1] == '>')
		// {
		// 	printf("bash: syntax error near unexpected token `>>'\n");
		// 	return (false);
		// }	
		/* else */if (line[0] == '<' && line[1] == '<')
		{
			printf("bash: syntax error near unexpected token `<<'\n");
			return (false);
		}
		// else if (line[0] == '>')
		// {
		// 	printf("bash: syntax error near unexpected token `>'\n");
		// 	return (false);
		// }
		else if (line[0] == '<')
		{
			printf("bash: syntax error near unexpected token `<'\n");
			return (false);
		}
	}
	return (true);
}

void	slash_error(char *line)
{
	int	i;
	int	option;

	i = 0;
	option = 0;
	while (line[i])
	{
		if (ft_isalnum(line[i]) != 0)
		{
			option = 1;
			break ;
		}
		i++;
	}
	if (option == 1)
		printf("bash : %s: No such file or directory\n", line);
	else
		printf("bash : %s: Is a directory\n", line);
}

void	pipe_error(char *line)
{
	int	i;

	i = 0;
	while (line[i] != ' ' && line[i])
		i++;
	if (i == 1)
		printf("bash: syntax error near unexpected token `|'\n");
	else
		printf("bash: syntax error near unexpected token `||'\n");
}
