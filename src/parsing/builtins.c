/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karl <karl@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 13:55:33 by casomarr          #+#    #+#             */
/*   Updated: 2023/10/17 01:01:07 by karl             ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

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
char	*cd(char *line, char *home_path)
{
	int		i;
	int		j;
	int		lenpath;
	int		lenhomepath;
	char	*path;
	bool	free_needed;
	
	free_needed = false;
	lenpath = 0;
	lenhomepath = 0;
	i = where_is_cmd_in_line(line, "cd");
	if (i == 0)
		return (line); //error : cd pas trouve
	//if (size_of_command(line, i, CMD) == 1 ||line[i] == '|' || ft_isalnum(line[i + 1]) != 1 || size_of_command(line, i, CMD) == 2 || line[i] == '\0') // 1 car je rends size + 1 donc si size = 1 c'est que il n'y a rien apres cd / 2 pour le cas "cd | ..."" Plus d'un espace serait efface donc pas plus de 
	if (size_of_command(line, 0, CMD) == 1 || line[i + 1] == '|' || line[i] == '\0') // 1 car je rends size + 1 donc si size = 1 c'est que il n'y a rien apres cd / 2 pour le cas "cd | ..."" Plus d'un espace serait efface donc pas plus de 2
	{
		path = pwd(NO_PRINT);
		printf("%spath = [%s]%s, %shome_path = [%s]\n%s", GREEN, path, RESET, YELLOW , home_path, RESET);
		if(ft_strncmp(path, home_path, ft_strlen(home_path)) != 0 && ft_strlen(home_path) != ft_strlen(path))
		{
			//path = la diff entre le path (pwd) et le home_path
			while (path[lenhomepath] == home_path[lenhomepath])
				lenhomepath++;
			while (home_path[lenhomepath])
				path[lenpath++] = home_path[lenhomepath++];
			path[lenpath++] = '\0';
			printf("%spath = [%s]%s, %shome_path = [%s]\n%s", GREEN, path, RESET, YELLOW , home_path, RESET);
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


/* Reproduces the env command */
void	ft_env(t_env *env)
{
	int	i;

	i = 0;
	while (env->env[i])
	{
		ft_putendl_fd(env->env[i], STDOUT_FILENO);
		i++;
	}
}
/* Reproduces the export command */
int	ft_export(t_element *cmd_list, char **av)
{
	int	ret;
	char	**new_var;
	
	ret = 1;
	if (!cmd_list->next->content) // CAS A REGARDER A 42, si on ecrit export sans rien
		ft_env(cmd_list->env);
	while (cmd_list && cmd_list->next)
	{
		if (!ft_is_valid(cmd_list->next->content)) // si on envoie un mauvais argument a export
		{
			ft_putendl_fd("export: %s: not a valid identifier", STDOUT_FILENO);
			return (0);
		}
		else if (ft_strchr(cmd_list->next->content, '=')) // si on trouve un = dans la string
		{
			new_var = new_env_var(cmd->nex->content);
			put_new_var(cmd_list, nev_var[0], new_var[1]);
		}
		printf("content = %s\n",  cmd_list->content);
		cmd_list = cmd_list->next;
	}
	return (1);
}

/* Checks if the argument is a valid name for a variable*/
int	ft_is_valid(char *s)
{
	int	i = 0;
	if (!ft_isalpha(s[i]) && s[i] != '_')
		return (0);
	while (s[i] && s[i] != '=')
	{
		if (ft_isalnum(s[i]) && s[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

/* Returns an array of strings with the new variable */
char **new_env_var(char *s)
{
	char **ret;
	char *position_equal;
	
	position_equal = ft_strchr(s, '='); // on prend la position du egal
	ret[0] = ft_substr(s, 0, position_equal - s);
	ret[1] = ft_substr(position_equal, 1, ft_strlen(position_equal));
	ret[3] = NULL;
	return (ret);
}

int	put_new_var(t_element *cmd_list, char *key, char *content)
{
	int		i;
	char	*to_free;
	t_env	*env;

	env = malloc(sizeof(t_env)); // a free apres
	if (content == NULL)
		content = ""; // pour quand meme ecrire le new env (exemple:"Hello=")
	t_env = find_value_with_key_env(env, key);
	if (env == NULL)
	{
		free(env);
		//peut etre un return, a tester a 42
	}
	to_free = ft_strjoin("=", content);
	if (!to_free)
	{
		free(env);
		return (0);
	}
	
	return (1);
}

/*

int	env_len(t_env * env)
alloc_new_env mais utilise une global variable