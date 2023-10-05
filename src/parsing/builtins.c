/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: casomarr <casomarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 13:55:33 by casomarr          #+#    #+#             */
/*   Updated: 2023/10/05 18:33:54 by casomarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

/*
1. Checks if the $ is followed by alphanumeric and preceded by a space
2. Puts what follows the $ into the variable key.
3. Cheks that the key exists in env_list (and node "takes its rank").
4. Replaces the key ($...) by its value in the line (returns new_line).
*/
char	*dollar(char *line, t_env *env_list)
{
	int		i;
	int		j;
	int		len;
	char	*key;
	char	*new_line;
	t_env	*node;

	i = where_is_cmd_in_line(line, "$");
	if (i == 0 || ft_isalpha(line[i + 1]) == 0 || line[i - 1] != ' ')
		return ("\n"); //error
	i ++; //now i = beggining of the key
	j = 0;
//trouver la key
	key = malloc(sizeof(char) * size_of_command(line, i, KEY) + 1);
	if (!key)
		return (NULL);
	while(line[i] && (line[i + 1] != ' '))
		key[j++] = line[i++];
	key[j] = '\0';
//check si la key existe dans env
	if (is_key_in_env(env_list, key) == false)
		return ("\n"); // meme si autres commandes dans la ligne, ca n imprime rien, juste une ligne vide!!
//chercher la key dans env
	node = find_value_with_key_env(env_list, key);
// remplacer key par sa value a l'interieur de la ligne (pour rendre new_line)
	i = where_is_cmd_in_line(line, "$");
	j = 0;
	len = 0;
	new_line = malloc(sizeof(char) * (ft_strlen(line) - size_of_command(line, i, CMD) + ft_strlen(node->value)) + 1);
	if (!new_line)
		return (NULL);
	while(line[i + 1] != '$')
		new_line[len++] = line[i++];
	i++;
	while(line[i] != ' ') // pret pour la suite de la ligne
		i++;
	i++;
	while(node->value[j])
		new_line[len++] = node->value[j++];
	while(line[i])
		new_line[len++] = line[i++];
	new_line[len] = '\0';
	free(key);
	return(new_line);
	//free (new_line)
}

/* i = c (= the beggining of the command "cd")*/
void	cd(char *line) //KARL faudra voir cette fonction avec toi pq je pense que ca a avoir avec l'executable
{
	int		i;
	int		j;
	char	*path;
	
	i = where_is_cmd_in_line(line, "cd");
	if (i == 0)
		return ; //error : cd pas trouve
	if (size_of_command(line, i, CMD) == 1) // 1 car je rends size + 1 donc si size = 1 c'est que il n'y a rien apres cd donc erreur		
		return ; // pas de path apres cd 
	i ++; //now i = beggining of the path
	j = 0;
	path = malloc(sizeof(char) * size_of_command(line, i, CMD) + 1);
	if (!path)
		return ;
	while(line[i] != ' ' && line[i] != '\0') // plus complique que ca : le path peut avoir des espaces TYPE : tronc \commun (je crois)
		path[j++] = line[i++];
	path[j] = '\0';
	if (chdir(path) != 0) // pour verifier que ca marche il faut avoir plusieurs dossiers
	{
		//printf errno
		return ;
	}
	free(path);
}

void	echo(char *line)
{
	//GERER AUSSI LE CAS echo "caro" "caro" --> doit ecrire les deux caro avec un espace entre les deux
	// idem pour echo   caro    caro -> caro caro
	//GERER AUSSI L'OPTION -n
	int		i;
	int		j;
	char	*str;
	char	type;
	
	str = NULL;
	i = where_is_cmd_in_line(line, "echo");
	if (i == 0)
		return ; //error
	i++;
	while(line[i] == ' ')
		i++; //now i = beggining of the str
	j = 0;
	//verifier s'il y a bien deux apostrophes minimum (dans le cas contraire il se passe quoi?)
	while(line[i] && line[i] != '|') //principale
	{
		if (line[i] == '\'')
		{
			type = '\'';
			i++;
		}
		else if (line[i] == '\"')
		{
			type = '\"';
			i++;
		}
		else
			type = ' ';
		str = ft_joinstr_minishell(line, i, str, type);
		if (str[0] == '\0') //premiere mot
			j = 0;
		else //plus d'un mot
			j = ft_strlen(str);
		while(line[i] != type && line[i])
		{
			if (type == ' ' && line[i] == '\\')
				i++;
			else
				str[j++] = line[i++];
		}
		str[j] = '\0';
		if (type == '\'' || type == '\"')
			i++;
		if (line[i] == ' ')
		{
			str[j++] = line[i++];
			while(line[i] == ' ' && line[i])
				i++;
		}
	}
	str[j] = '\0';
	printf("%s\n", str);
	free(str);
}
/*CARACTERES QUI NECESSITENT UN \ POUR ETRE ECHO CORRECTEMENT
#
(
)
*
;
<
>
`
~
"
\
*/

void	pwd()
{
	char *path;

	path = getcwd(NULL, 0);
	if (path != NULL)
		printf("%s\n", path);
	//else
		//printf errno
	free(path);
}
