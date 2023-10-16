/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: octonaute <octonaute@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 12:34:22 by octonaute         #+#    #+#             */
/*   Updated: 2023/10/16 15:11:02 by octonaute        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

void	cd_home(char *home_path, t_env *env_list)
{
/* 	int		lenpath;
	int		lennew;
	char	*new_path; */
	char	*path;
	char	*bigger;
	t_env	*user;

	path = pwd(NO_PRINT);
	user = find_value_with_key_env(env_list, "USER");
	path = home_path_simplified(path, user);
	//printf("%spath = [%s]%s, %shome_path = [%s]\n%s", GREEN, path, RESET, YELLOW , home_path, RESET);
	bigger = NULL;
	if (ft_strlen(path) > ft_strlen(home_path))
		bigger = path;
	else
		bigger = home_path;
	if(ft_strncmp(path, home_path, ft_strlen(bigger)) != 0 && ft_strlen(home_path) != ft_strlen(path)) //tant que home_path est différent de path
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

		printf("%spath = [%s]\n%s", BRED, path, RESET);
		printf("%shome_path = [%s]\n%s", BRED, home_path, RESET);
		
/*COMPARER PATH ET HOME PATH À PARTIR DU NOM DU PREMIER DOSSIER DE HOME_PATH CAR SINON
ILS NE SONT JAMAIS PAREILS CAR HOME_PATH A /HOME/OCTONAUTE EN PLUS AU DÉBUT :

int i = 0;
while (home_path[i] != '/')
	i++;
char *start_path;
strlcpy(start_path, home_path, i); //start_path = "Documents"
//voir si je peux utiliser la fonction find_command_with_value un truc du genre pour trouver où
se trouve Documents dans path.
i = la fin de Documents dans path (après le slash)
On fait le ft_strncmp ci-dessous en ignorant le début de path jusqu'à documents (jusqu'à i donc).
*/

		int i = 0; // pour eviter boucle infinie, pour test, EFFACER
		while (ft_strncmp(path, home_path, ft_strlen(bigger)) != 0 && ft_strlen(home_path) != ft_strlen(path) && i < 7)
		{
			chdir("..");
			path = pwd(NO_PRINT);
			printf("%spath = [%s]\n%s", BRED, path, RESET);
			printf("%shome_path = [%s]\n%s", BRED, home_path, RESET);
			i++;
		}
		free(path);
	}
	else
		return ;
}

char    *cd_path(char *line, int i, char *path)
{
    int		j;

    j = 0;
    while(line[i] != ' ' && line[i] != '\0') //traite les cas où le nom du dossier contient des espaces
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
    return (path);
}

void	cd_directory(char *line, int i)
{
	char	*path;

	i++; //now i = beggining of the path
	path = malloc(sizeof(char) * size_of_command(line, i, CMD));
	if (!path)
		return ;
    path = cd_path(line, i, path);
	if (chdir(path) != 0)
	{
		printf("bash : cd : %s: No such file or directory\n", path);
		free(path);
		//printf errno
		return ;
	}
	//printf("%spath = [%s]\n%s", YELLOW, path, RESET);
	free(path);
}

/* i = c (= the beggining of the command "cd")*/
void	cd(char *line, char *home_path, t_env *env_list)
{
	int		i;
	
/* 	lenpath = 0;
	lennew = 0;
	new_path = NULL; */
	i = where_is_cmd_in_line(line, "cd");
	if (i == 0)
		return ; //error : cd pas trouve
	//if (size_of_command(line, i, CMD) == 1 ||line[i] == '|' || ft_isalnum(line[i + 1]) != 1 || size_of_command(line, i, CMD) == 2 || line[i] == '\0') // 1 car je rends size + 1 donc si size = 1 c'est que il n'y a rien apres cd / 2 pour le cas "cd | ..."" Plus d'un espace serait efface donc pas plus de 
	if (size_of_command(line, 0, CMD) == 1 || line[i + 1] == '|' || line[i] == '\0') // 1 car je rends size + 1 donc si size = 1 c'est que il n'y a rien apres cd / 2 pour le cas "cd | ..."" Plus d'un espace serait efface donc pas plus de 2
		cd_home(home_path, env_list);
	else
		cd_directory(line, i);
}
