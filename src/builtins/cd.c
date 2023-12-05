/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: casomarr <casomarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 12:34:22 by octonaute         #+#    #+#             */
/*   Updated: 2023/12/05 18:10:22 by casomarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*Moves to the specified directory.*/
void	cd_directory(char *path, t_env *env_list)
{
	t_env	*home;

	home = find_value_with_key_env(env_list, "HOME");
	if (chdir(path) != 0)
	{
		msg_error_bash(2, path);
		return ;
	}
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
		msg_error_bash(6, "cd: HOME not set");
		free(current_path);
		return ;
	}
	home = find_value_with_key_env(env_list, "HOME");
	if (is_cmd(current_path, home->value) == false)
	{
		if (chdir(home->value) != 0)
		{
			free(current_path);
			perror("bash: ");
			return ;
		}
	}
	free(current_path);
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
