/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carolina <carolina@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 14:50:30 by casomarr          #+#    #+#             */
/*   Updated: 2023/10/11 19:28:22 by carolina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

/*J'ai fait +2 dans les mallocs de toute cette fonction car en plus du \0
il y a potentiellement un espace de plus (dans le cas ou il y a plusieurs
strings a echo et que comme c'est calloc et non malloc on s'en fout que
ce ne soit pas le bon nombre exact je pense car au pire tout est deja des \0)*/
char	*ft_joinstr_minishell(char *line, int len, char *str, char type)
{
	int		i;
	int		j;
	char	*new_str;

	// printf("%sDANS JOINSTR%s\n", YELLOW, RESET);
	// printf("%stype = [%c]%s\n", GREEN, type, RESET);
	
	if (type == '\'' || type == '\"')
	{
		if (str == NULL)
		{
			new_str = ft_calloc(size_of_command(line, len, STR) \
			+ 1, sizeof(char));
			return (new_str);
		}
		else
			new_str = ft_calloc((ft_strlen(str) + \
			size_of_command(line, len, STR) + 1), sizeof(char));
	}
	else
	{
		if (str == NULL)
		{
			new_str = ft_calloc(size_of_command(line, len, CMD) \
			+ 1, sizeof(char));
			return (new_str);
		}
		else
			new_str = ft_calloc((ft_strlen(str) \
			+ size_of_command(line, len, CMD) + 1), sizeof(char));
	}
	if (!new_str)
		return (NULL);
	i = 0;
	j = 0;
	while(str[i])
	{
		new_str[j++] = str[i++];
	}
	new_str[j] = '\0';
	if (str != NULL)
		free(str);
	return (new_str);
}


char	*ft_join_pour_cd(char *line_begining, char *path)
{
	int		i;
	int		j;
	char	*new_str;
	
	new_str = ft_calloc((ft_strlen(line_begining) + ft_strlen(path) + 1), sizeof(char));
	if (!new_str)
		return (NULL);
	i = 0;
	j = 0;
	while(line_begining[i])
	{
		new_str[j++] = line_begining[i++];
	}
	i = 0;
	while(path[i])
	{
		new_str[j++] = path[i++];
	}
	new_str[j] = '\0';
	free(line_begining);
	return (new_str);
}

bool	only_spaces_after_cmd(char *line, size_t i)
{
	while(line[i])
	{
		if(line[i] != ' ')
			return (false);
		i++;
	}
	return (true);
}
/*
char	*erase_spaces(char *line, int option, int start)
{
	
	char	*new_line;
	int		j;
	int		start_copy;
	int		spaces;

	j = 0;
	start_copy = start;
	spaces = 0;
	if (option == BEGINING) // start = 0
	{
		start = 0;
		if (line[start] != ' ')	
			return (line);
		while (line[start] == ' ')
			start++;
		new_line = malloc(sizeof(char) * (ft_strlen(line) - start) + 1); //verifier
		if (!new_line)
		{
			perror("Error");
			return (NULL); //il faut qd meme un return qd on utilise perror??
		}
		while(line[start])
			new_line[j++] = line[start++];
		new_line[j] = '\0';	
	}
	else if (option == MIDDLE) // start = fin de cmd (sur l'espace)
	{
		start = start + 1; //debut de la cmd suivante
		if (line[start] == '|')
			return (line);
		while(line[start_copy] != '|')
		{
			if (line[start_copy] == ' ')
				spaces++;
			start_copy++;
		}
		new_line = malloc(sizeof(char) * (ft_strlen(line) - spaces) + 1);
		if (!new_line)
		{
			perror("Error");
			return (NULL); //il faut qd meme un return qd on utilise perror??
		}
		while(j <= start)
		{
			new_line[j] = line[j];
			j++;
		}
		while(line[start] != '|')
		{
			if (line[start] == ' ')
				start++;
			else
				new_line[j++] = line[start++]; // pas sure
		}
		while(line[start])
			new_line[j++] = line[start++];
		new_line[j] = '\0';
	}
	else //if (option == END) //start = espace apres la fin de la cmd
	{
		if (line[start] == '\0')
			return (line);
		while(line[start_copy] != '\0')
		{
			if (line[start_copy] == ' ')
				spaces++;
			start_copy++;
		}
		new_line = malloc(sizeof(char) * (ft_strlen(line) - spaces) + 1);
		if (!new_line)
		{
			perror("Error");
			return (NULL); //il faut qd meme un return qd on utilise perror??
		}
		while(j <= start)
		{
			new_line[j] = line[j];
			j++;
		}
		while(line[start])
		{
			if (line[start] == ' ')
				start++;
			else
				new_line[j++] = line[start++];
		}
		new_line[j] = '\0';
	}
	free(line);
	return (new_line);
}
*/


char	*erase_spaces(char *line)
{
	
	char	*new_line;
	int		i;
	int		j;
	int		spaces;

	i = 0;
	j = 0;
	spaces = 0;

	while (line[i])
	{
		if (line[i] == '\'') //pour ne pas compter les espaces entre quotes (ex: dans les str de echo)
		{
			while(line[i] != '\'')
				i++;
		}
		if (line[i] == '\"')
		{
			while(line[i] != '\"')
				i++;
		}
		if(line[i] == ' ' && line[i + 1] == ' ')
			spaces++;
		i++;
	}
	new_line = malloc(sizeof(char) * (ft_strlen(line) - spaces) + 1); // +2 au lieu de +1 a regle des pb de valgrind
	if (!new_line)
	{
		perror("Error");
		return (NULL); //il faut qd meme un return qd on utilise perror??
	}
	i = 0;
	while (line[i])
	{
		if (line[i] == '\'') //pour ne pas compter les espaces entre quotes (ex: dans les str de echo)
		{
			while(line[i] != '\'')
				new_line[j++] = line[i++];
		}
		else if (line[i] == '\"')
		{
			while(line[i] != '\"')
				new_line[j++] = line[i++];
		}
		else if(line[i] == ' ' && line[i + 1] == ' ')
		{
			spaces++;
			i++;
		}
		else
			new_line[j++] = line[i++];
	}
	new_line[j] = '\0';
	free(line);
	return (new_line);
}
