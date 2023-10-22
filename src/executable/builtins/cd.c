/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karl <karl@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 12:34:22 by octonaute         #+#    #+#             */
/*   Updated: 2023/10/22 20:57:42 by karl             ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../../../includes/minishell.h"
#include "../../../libft/libft.h"

char	*split_at_user(char *big_path, char *user)
{
	char	*str; //a la fois les mots entre slash et a la fin le path simplifie
	int	end;
	int start;

	end = 0;
	start = 1;
	str = NULL;
	while(big_path[end])
	{
		if (big_path[end + 1] == '/')
		{
			str = strlcpy_middle(str, big_path, start, end);
			// printf("%sstr = [%s]\n%s", GREEN, str, RESET);
			// printf("%suser = [%s]\n%s", GREEN, user, RESET);
			// printf("%sbig_path = [%s]\n%s", YELLOW, big_path, RESET);
			if (ft_strncmp(str, user, ft_strlen(user)) == 0)
			{
				free(str);
				str = NULL;
				str = strlcpy_middle(str, big_path, 0, end);
				//printf("%sstr = [%s]\n%s", GREEN, str, RESET);
				return (str);
			}
			else
			{
 				free(str);
				str = NULL;
				start = end + 2;
				end+=1;
			}
		}
		end++;
	}
	return (NULL); //ne devrait jamais arriver la
}

/*Handles the case where cd has no specified path afterwards by 
making "cd .." until it arrives to the home folder.*/
void	cd_home(t_env *env_list)
{
	//t_env	*path;
	// t_env	*user;
	t_env	*home;
	//char *home_path;
	char *current_path;

	current_path = pwd(NO_PRINT);
	// user = find_value_with_key_env(env_list, "USER");
	// path = find_value_with_key_env(env_list, "PATH");
	home = find_value_with_key_env(env_list, "HOME");
	//home_path = split_at_user(path->value, user->value);
	//printf("path simplified = %s\n", home_path);
	if(ft_strncmp(current_path, home->value, ft_strlen(current_path)) != 0 && \
	ft_strlen(home->value) != ft_strlen(current_path))
	{
		while (ft_strncmp(current_path, home->value, ft_strlen(current_path)) != 0 && \
	ft_strlen(home->value) != ft_strlen(current_path))
		{
			if (chdir("..") != 0)
			{
				free(current_path);
				//printf errno
				return ;
			}
			current_path = pwd(NO_PRINT);
			//user = find_value_with_key_env(env_list, "USER");
			//path = home_path_simplified(path, user);
		}
		free(current_path);
	}
	else
		return ;
}

/*Returns the specified path and handles the case where a directory
contains spaces in its name.*/
char    *fix_path_if_spaces(char *path)
{
    int		j;
	int		i;
	char	*new_path;

    j = 0;
	i = 0;
	new_path = calloc(ft_strlen(path) + 1, sizeof(char)); //new_path fera max la len de path (en cas de non espaces)
    while(path[i]) //traite les cas où le nom du dossier contient des espaces
	{
		if (path[i] == '\\' && path[i + 1] == ' ')
		{
			while(path[i] != '/' && (path[i + 1] != ' ' || path[i + 1] != '\0'))
			{
				if (path[i] == '\\')
					i++;
				new_path[j++] = path[i++];
			}
			i++; //pour sauter le / de fin
		}
		if (path[i] != '\0')
			new_path[j++] = path[i++];
	}	
	new_path[j] = '\0';
    return (new_path);
}

/*Moves to the specified directory.*/
void	cd_directory(char *path, t_env *env_list)
{
	char	*new_path;
	t_env *home;
	int i;

	/* char *test;
	test = NULL; */

////////////////////////////////
	//POUR EVITER QUE QD ON FAIT CD PUIS CD .. CA ECRIVE (NULL)$ SUR LE PROMPT
	// POUR L'INSTANT NE MARCHE PAS
	home = find_value_with_key_env(env_list, "HOME");
	i = ft_strlen(pwd(NO_PRINT)) - 2; //pour sauter le dernier slash
    new_path = fix_path_if_spaces(path);
	
//----------------------------------- Le fait de le mettre ce qu'il y a ci-dessous en commentaire remet le pb du prompt mais ne devrait pas être géré ici pour permettre de monter aussi haut qu'on veut
// 	if (ft_strncmp(new_path, "..", ft_strlen(new_path)) == 0)
// 	{
// 		while(i > 0)
// 		{
// 			if (pwd(NO_PRINT)[i - 1] == '/')
// 				break;
// 			i--;
// 		}
// /* 		printf("home : [%s]\n", home->value);
// 		test = strlcpy_middle(test, pwd(NO_PRINT), 0, ft_strlen(pwd(NO_PRINT - i + 1)));
// 		printf("path : [%s]\n", test);
// 		printf("compare = [%d]\n", ft_strncmp(pwd(NO_PRINT), home->value, ft_strlen(pwd(NO_PRINT - i + 1)))); */
// 		if (ft_strncmp(pwd(NO_PRINT), home->value, ft_strlen(pwd(NO_PRINT)) - i + 1) == 0)
// 		{
// 			//ne rien faire :
// 			free(new_path);
// 			return ;
// 		}
// 	}
//-----------------------------------


///////////////////////////
	if (chdir(new_path) != 0)
	{
		printf("bash : cd : %s: No such file or directory\n", path);
		free(new_path);
		//printf errno
		return ;
	}
	free(new_path);
}

/*If cd has no specified directory afterwards, calls the function cd_home.
Otherwise, calls the function cd_directory.*/
void	cd(t_element *current, t_env *env_list)
{
	if (current->next == NULL || current->next->type == PIPE)
		cd_home(env_list);
	else
		cd_directory(current->next->content, env_list);
}
