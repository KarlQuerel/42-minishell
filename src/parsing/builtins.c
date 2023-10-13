/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: casomarr <casomarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 13:55:33 by casomarr          #+#    #+#             */
/*   Updated: 2023/10/13 18:25:17 by casomarr         ###   ########.fr       */
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
	key = malloc(sizeof(char) * size_of_command(line, i, KEY));
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
	new_line = malloc(sizeof(char) * (ft_strlen(line) - size_of_command(line, i, CMD) + ft_strlen(node->value)));
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
char	*cd(char *line, char *home_path, t_env *env_list)
{
	int		i;
	int		j;
	int		lenpath;
	int		lennew;
	char	*path;
	char	*new_path;
	char	*bigger;
	bool	free_needed;
	t_env	*user;
	
	free_needed = false;
	lenpath = 0;
	lennew = 0;
	new_path = NULL;
	i = where_is_cmd_in_line(line, "cd");
	if (i == 0)
		return (line); //error : cd pas trouve
	//if (size_of_command(line, i, CMD) == 1 ||line[i] == '|' || ft_isalnum(line[i + 1]) != 1 || size_of_command(line, i, CMD) == 2 || line[i] == '\0') // 1 car je rends size + 1 donc si size = 1 c'est que il n'y a rien apres cd / 2 pour le cas "cd | ..."" Plus d'un espace serait efface donc pas plus de 
	if (size_of_command(line, 0, CMD) == 1 || line[i + 1] == '|' || line[i] == '\0') // 1 car je rends size + 1 donc si size = 1 c'est que il n'y a rien apres cd / 2 pour le cas "cd | ..."" Plus d'un espace serait efface donc pas plus de 2
	{
		path = pwd(NO_PRINT);
		user = find_value_with_key_env(env_list, "USER");
		path = home_path_simplified(path, user);
		printf("%spath = [%s]%s, %shome_path = [%s]\n%s", GREEN, path, RESET, YELLOW , home_path, RESET);
		if(ft_strncmp(path, home_path, ft_strlen(path)) != 0 && ft_strlen(home_path) != ft_strlen(path))
		{
			//if du dessus ne va pas : dans strncmp mettre le ft_strlen du path le plus long
			//new_path = la diff entre le path (pwd) et le home_path
/* 			while (path[lenpath] == home_path[lenpath])
				lenpath++;
			new_path = malloc(sizeof(char) * (ft_strlen(home_path) - lenpath + 2));
			lenpath+=1;
			while (home_path[lenpath])
				new_path[lennew++] = home_path[lenpath++];
			new_path[lennew] = '\0';
			printf("%snew_path = [%s]\n%s", BRED, new_path, RESET); */
 			
			bigger = NULL;
			if (ft_strlen(path) > ft_strlen(home_path))
				bigger = path;
			else
				bigger = home_path;
			while (ft_strncmp(path, home_path, ft_strlen(bigger)) != 0 && ft_strlen(home_path) != ft_strlen(path))
			{
				chdir("..");
				path = pwd(NO_PRINT);
			}
			
			
			// free(path);
			// path = new_path;
		}
		else
			return (line);
	}	
	else
	{
		i++; //now i = beggining of the path
		path = malloc(sizeof(char) * size_of_command(line, i, CMD));
		if (!path)
			return (line); //?
		j = 0;
		while(line[i] != ' ' && line[i] != '\0')
		{
			if (line[i] == '\\' && line[i + 1] == ' ')
			{
				while(line[i] != '/' && (line[i + 1] != ' ' || line[i + 1] != '\0'))
				{
					if (line[i] == '\\')
						i++;
					path[j++] = line[i++];
				}
				i++; //pour sauter le / de fin
			}
			if (line[i] != '\0')
				path[j++] = line[i++];
		}	
		path[j] = '\0';
		free_needed = true;
	}
	if (chdir(path) != 0) // pour verifier que ca marche il faut avoir plusieurs dossiers
	{
		printf("bash : cd : %s: No such file or directory\n", path);
		free(path);
		//printf errno
		return (line); //??
	}
	//printf("%spath = [%s]\n%s", YELLOW, path, RESET);
	if (free_needed == true)
		free(path);
	//faudra free (new_path) aussi
	return (line);
}

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
		// printf("Error : quotes don't close\n"); //bash n'ecrit pas erreur mais je ne peux pas reproduire le > qui apparait
		return ;
	}
	while(line[i] == ' ')
		i++;
	while (ft_isprint(line[i]) != 1 && line[i] != '\'' && line[i] != '\"')
		i++;
	// printf("debut des str : [%c]\n", line[i]);
	j = 0;
	while(line[i] && line[i] != '|')
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
		// printf("apres avoir passe la premiere apostrophe (si il y en a) : [%c]\n", line[i]);
		if (line[i] == type) //pour sauter les strings vides
			i+=1;
		else
		{
			// printf("type : [%c]\n", type);
			// printf("premiere string commence par : [%c]\n", line[i]);
			str = ft_joinstr_minishell(line, i, str, type);
			if (str[0] == '\0') //premiere mot
			{
				j = 0;
				// printf("%spremier mot%s\n", YELLOW, RESET);
			}
			else //plus d'un mot
			{
				j = ft_strlen(str);
				// printf("%smot suivant%s\n", YELLOW, RESET);
			}
			while(line[i] != type && line[i])
			{
				if (type == ' ' && line[i] == '\\') //pour le test echo hola\ncaro -> doit donner holancaro
					i++;
				else
					str[j++] = line[i++];
			}
			str[j] = '\0';
			// printf("%smot : [%s]%s\n", GREEN, str, RESET);
			if (type == '\'' || type == '\"')
				i++;
			if (line[i] == ' ')
			{
				str[j++] = line[i++];
				while(line[i] == ' ' && line[i])
					i++;
			}
		}
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

char	*pwd(int option)
{
	char *path;

	path = getcwd(NULL, 0);
	if (path != NULL && option == PRINT)
		printf("%s\n", path);
	//else
		//printf errno
	//free(path);
	return (path);
}
