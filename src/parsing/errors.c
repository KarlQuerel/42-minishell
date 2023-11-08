/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karl <karl@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 12:39:52 by casomarr          #+#    #+#             */
/*   Updated: 2023/11/08 16:17:50 by karl             ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

bool	first_character_error(char *line)
{
	bool	status;
	
	status = false;
	if (line[0] == '<' || line[0] == '>')
		status = redirecters_error(line);
	//KARL j'ai mis ca en commentaire pour tester /usr/bin/ls !
	// else if (line[0] == '/')
	// 	slash_error(line);
	else if (line[0] == '|') //autre erreur : si les cmd avant/apres la pip ne sont pas des vrai cmd (erreur ligne 57 du google doc)
		pipe_error(line);
	else if (line[0] == '&')
		and_error(line);
	else if (line[0] == '\'' || line[0] == '\"')
		str_error(line);
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
	while(line[i])
	{
		if (ft_isalnum(line[i]) != 0)
		{
			option = 1;
			break;
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
	while(line[i] != ' ' && line[i])
		i++;
	if (i == 1)
		printf("bash: syntax error near unexpected token `|'\n");
	else
		printf("bash: syntax error near unexpected token `||'\n");
}

void	and_error(char *line)
{
	int	i;
	
	i = 0;
	while(line[i] != ' ' && line[i])
		i++;
	if (i == 1)
		printf("bash: syntax error near unexpected token `&'\n");
	else
		printf("bash: syntax error near unexpected token `&&'\n");
}

void	str_error(char *line)
{
	int	i;
	int	j;
	char *str;
	char type;

	if (line[0] == '\'')
		type = '\'';
	if (line[0] == '\"')
		type = '\"';
	i = 1;
	while(line[i] != type)
		i++;
	// str = malloc(sizeof(char) * i + 1);
	str = ft_calloc(i + 1, sizeof(char));
	i = 1;
	j = 0;
	while(line[i] != type)
		str[j++] = line[i++];
	str[j] = '\0';
	printf("%s : command not found\n", str);
	free(str);
}

bool	pipe_double_or_eof(char *line)
{
	int	i;

	i = 0;
	while(line[i])
	{
		if(line[i] == '|' && line[i + 1] == '|')
		{
			ft_putendl_fd("bash: syntax error near unexpected token `||'", STDERR_FILENO);
			return (false);
		}
		i++;
	}
	if (line[ft_strlen(line) - 1] == '|')
	{
		ft_putendl_fd("bash: syntax error : | at end of line", STDERR_FILENO);
		return (false);
	}
	return (true);
}
