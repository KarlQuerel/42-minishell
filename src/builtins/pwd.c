/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 13:55:33 by casomarr          #+#    #+#             */
/*   Updated: 2023/12/01 18:14:26 by kquerel          ###   ########.fr       */
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
		free(key->value);
		temp_pwd = pwd(NO_PRINT);
		key->value = ft_calloc(ft_strlen(temp_pwd) + 1, sizeof(char));
		ft_strlcpy(key->value, temp_pwd, ft_strlen(temp_pwd) + 1);
		free(temp_pwd);
	}
}
