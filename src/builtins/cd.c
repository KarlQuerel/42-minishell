/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 12:34:22 by octonaute         #+#    #+#             */
/*   Updated: 2023/12/01 18:03:03 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*Returns the specified path and handles the case where a directory
contains spaces in its name.
I calloc new_path to the size of path because that's the max length it
will have (in case of no spaces to skip).
The first If's purpouse is to handle white spaces in a filename.*/
char	*fix_path_if_spaces(char *path)
{
	int		j;
	int		i;
	char	*new_path;

	j = 0;
	i = 0;
	new_path = ft_calloc(ft_strlen(path) + 1, sizeof(char));
	while (path[i])
	{
		if (path[i] == '\\' && path[i + 1] == ' ')
		{
			while (path[i] != '/' && (path[i + 1] != ' ' || \
			path[i + 1] != '\0'))
			{
				if (path[i] == '\\')
					i++;
				new_path[j++] = path[i++];
			}
			i++;
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
		msg_error_bash(2, path);
		free(new_path);
		return ;
	}
	free(new_path);
}

/*Handles the case where cd has no specified path afterwards by 
making "cd .." until it arrives to the home folder.
The Else condition is for when we are already in the home file.*/
void	cd_home(t_env *env_list)
{
	t_env	*home;
	char	*current_path;

	current_path = pwd(NO_PRINT);
	if (!is_key_in_env(env_list, "HOME"))
	{
		msg_error_bash(3, "");
		free(current_path);
		return ;
	}
	home = find_value_with_key_env(env_list, "HOME");
	if (is_cmd(current_path, home->value) == false)
	{
		if (is_user_in_path(current_path, env_list) == true)
			go_backwards_until_user(current_path, home->value);
		else
			go_forward_until_user(current_path, home->value);
	}
	else
	{
		free(current_path);
		return ;
	}
}

/*If cd has no specified directory afterwards, calls the function cd_home.
Otherwise, calls the function cd_directory.*/
void	cd(t_element *current, t_env *env_list)
{
	while (current->next && current->next->type >= 3)
		current = current->next;
	if (current->next == NULL || current->next->type == PIPE)
		cd_home(env_list);
	else
		cd_directory(current->next->content, env_list);
}
