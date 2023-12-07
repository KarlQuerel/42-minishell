/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 13:55:33 by casomarr          #+#    #+#             */
/*   Updated: 2023/12/07 13:42:03 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*Gets the current path and prints it if the 
trigger requires it.*/
char	*pwd(int option)
{
	char	*path;

	path = getcwd(NULL, 0);
	if (!path)
		perror("bash: ");
	if (option == PRINT)
	{
		ft_putendl_fd(path, STDOUT_FILENO);
		free(path);
		return (NULL);
	}
	return (path);
}

void	pwd_update_in_env(t_env **env_list)
{
	t_env	*key;
	char	*temp_pwd;

	if (is_key_in_env((*env_list), "PWD"))
	{
		key = find_value_with_key_env((*env_list), "PWD");
		if (!key)
			return ;
		temp_pwd = pwd(NO_PRINT);
		if (!temp_pwd)
			return ;
		free(key->value);
		key->value = ft_strdup(temp_pwd);
		free(temp_pwd);
	}
}
