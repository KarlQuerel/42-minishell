/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: octonaute <octonaute@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 14:50:30 by casomarr          #+#    #+#             */
/*   Updated: 2023/11/02 14:39:39 by octonaute        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

//EFFACER LE COMMENTAIRE CI DESSOUS
/*COMMENTAIRE QUI JE CROIS N'EST PLUS D'ACTUALITE MAIS JE GARDES AU CAS OU CA 
M AIDERAIT A REGLER DES ERREURS DE VALGRIND :
J'ai fait +2 dans les mallocs de toute cette fonction car en plus du \0
il y a potentiellement un espace de plus (dans le cas ou il y a plusieurs
strings a echo et que comme c'est calloc et non malloc on s'en fout que
ce ne soit pas le bon nombre exact je pense car au pire tout est deja des \0)*/

/*Function used in the echo function to join all the arguments encountered
between the "echo" cmd and a pipe or the end of the command line.*/
char	*ft_joinstr_minishell(char *line, int len, char *str, char type)
{
	int		i;
	int		j;
	char	*new_str;
	
	new_str = joinstr_minishell_malloc(line, len, str, type);
	if (str == NULL)
		return (new_str);
	i = 0;
	j = 0;
	while(str[i])
		new_str[j++] = str[i++];
	new_str[j] = '\0';
	free(str);
	return (new_str);
}

/*Function used in the cd function to join all the arguments encountered
between the "cd" cmd and a pipe or the end of the command line : it is 
needed in the case where a folder's name has spaces.*/
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

/*Returns a new_line that is the same as the original command line
except all the superfulous spaces have been erased.*/
char	*erase_spaces(char *line)
{
	char	*new_line;
	int		i;
	int		j;

	i = 0;
	j = 0;

	// KARL -> j'ai ajoute ca pour regler une seg fault
	if (!line)
		return (NULL);
	// fin
	
	new_line = erase_spaces_malloc(line);
	while (line[i])
	{
		/*utiliser ici la fonction type_of_str pour gagner bcp de lignes : 
		le premier if/else pourrait etre merge en un seul if.
		Voir aussi si je ne peux pas utiliser la fx skip pour skip 
		les espaces et quotes.*/
		if (line[i] == '\'' && quotes_can_close(line) == true) //pour ne pas compter les espaces entre quotes (ex: dans les str de echo)
		{
			new_line[j++] = line[i++];
			while(line[i] != '\'' && line[i])
				new_line[j++] = line[i++];
			new_line[j++] = line[i++];
		}
		else if (line[i] == '\"' && quotes_can_close(line) == true)
		{
			new_line[j++] = line[i++];
			while(line[i] != '\"' && line[i])
				new_line[j++] = line[i++];
			new_line[j++] = line[i++];
		}
		else if(line[i] == ' ' && line[i + 1] == ' ')
			i+=2;
		else if(line[i] == ' ' && line[i + 1] == '\0')
			i+=1;
		else
			new_line[j++] = line[i++];
	}
	if (j == 0)
		return (NULL);
	new_line[j] = '\0';
	free(line);
	return (new_line);
}

/*Same as the strlcpy function except it takes a start parameter in addition
to an end parameter.*/
char *strlcpy_middle(char *dst, const char *src, size_t start, size_t end)
{	
	int	i;
	
	i = 0;
	// dst = malloc(sizeof(char) * (end - start + 2));
	dst = ft_calloc(end - start + 2, sizeof(char));
	while (src[start] && start <= end)
		dst[i++] = src[start++];
	dst[i] = '\0';
	return (dst);
}