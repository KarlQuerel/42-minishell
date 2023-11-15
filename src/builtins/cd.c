/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karl <karl@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 12:34:22 by octonaute         #+#    #+#             */
/*   Updated: 2023/11/15 14:36:06 by karl             ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

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

void	go_backwards_until_user(char *current_path, char *home_value)
{
	while (is_this_command(current_path, home_value) == false)
	{
		// -----> cd qd plus haut que user devrait nous faire avancer jusqu'à après le user!!!!
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

size_t	size_of_word(char *path, int i)
{
	if (path[i] == '/')
		i+=1;
	while(path[i] != '/' && path[i])
		i++;
	return(i + 1);
}

void	go_forward_until_user(char *current_path, char *home_value)
{
	//on retrouve ou est current_path compare a home_path
	// on fait cd() de chaque word entre slashes jusqu a ce que 
	// current_path = home_value

	// marche bien sauf qd je fais cd en etant tout en haut (sur /)
	
	int 	end;
	int		start;
	char	*word;

	end = 0;
	while(current_path[end] == home_value[end])
		end++;
	if (ft_strncmp(current_path, "/", ft_strlen(current_path)) != 0)
		end+=1;
	start = end;
	word = ft_calloc(size_of_word(home_value, start) + 1, sizeof(char));
	while(is_this_command(current_path, home_value) == false)
	{
		while(home_value[end] != '/' && home_value[end])
			end++;
		word = strlcpy_middle(word, home_value, start, end - 1);
		if (chdir(word) != 0)
		{
			//free
			return ;
		}
		if (home_value[end + 1] == '\0')
			break;
		end+=1;
		start = end;
		free(word);
		word = ft_calloc(size_of_word(home_value, start) + 1, sizeof(char));
	}
}

/*Handles the case where cd has no specified path afterwards by 
making "cd .." until it arrives to the home folder.*/
void	cd_home(t_env *env_list)
{
	t_env	*home;
	char *current_path;

	current_path = pwd(NO_PRINT);
	if (!is_key_in_env(env_list, "HOME"))
	{
		ft_putendl_fd("bash: cd: HOME not set", STDERR_FILENO);
		free(current_path);
		return ;
	}
	home = find_value_with_key_env(env_list, "HOME");
	if(is_this_command(current_path, home->value) == false)
	{
		if (is_user_in_path(current_path, env_list) == true)
			go_backwards_until_user(current_path, home->value);
		else
			go_forward_until_user(current_path, home->value);
		free(current_path);
	}
	else //already in home
	{
		free(current_path);
		return ;
	}
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
	new_path = ft_calloc(ft_strlen(path) + 1, sizeof(char)); //new_path fera max la len de path (en cas de non espaces)
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
	t_env	*home;
	
	home = find_value_with_key_env(env_list, "HOME");
	new_path = fix_path_if_spaces(path);
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
