/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: casomarr <casomarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 12:34:22 by octonaute         #+#    #+#             */
/*   Updated: 2023/10/17 15:39:17 by casomarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

/*Handles the case where cd has no specified path afterwards by 
making "cd .." until it arrives to the home folder.*/
void	cd_home(char *home_path, t_env *env_list)
{
	char	*path;
	char	*bigger;
	t_env	*user;

	path = pwd(NO_PRINT);
	user = find_value_with_key_env(env_list, "USER");
	path = home_path_simplified(path, user);
	bigger = NULL;
	if (ft_strlen(path) > ft_strlen(home_path))
		bigger = path;
	else
		bigger = home_path;
	if(ft_strncmp(path, home_path, ft_strlen(bigger)) != 0 && \
	ft_strlen(home_path) != ft_strlen(path)) //tant que home_path est différent de path
	{
		while (ft_strncmp(path, home_path, ft_strlen(bigger)) != 0 && \
		ft_strlen(home_path) != ft_strlen(path))
		{
			if (chdir("..") != 0)
			{
				free(path);
				//printf errno
				return ;
			}
			path = pwd(NO_PRINT);
			user = find_value_with_key_env(env_list, "USER");
			path = home_path_simplified(path, user);
		}
		free(path);
	}
	else
		return ;
}

/*Returns the specified path and handles the case where a directory
contains spaces in its name.*/
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

/*Moves to the specified directory.*/
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
	free(path);
}

/*If cd has no specified directory afterwards, calls the function cd_home.
Otherwise, calls the function cd_directory.*/
void	cd(char *line, char *home_path, t_env *env_list)
{
	int		i;
	
	i = where_is_cmd_in_line(line, "cd");
	if (i == 0)
		return ; //error : cd pas trouve
	if (size_of_command(line, 0, CMD) == 1 || line[i + 1] == '|' || line[i] == '\0')
		cd_home(home_path, env_list);
	else
		cd_directory(line, i);
}
