/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: casomarr <casomarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 13:55:33 by casomarr          #+#    #+#             */
/*   Updated: 2023/11/27 14:03:26 by casomarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*Gets the current path and prints it if the 
trigger reqiures it.*/
char	*pwd(int option)
{
	char *path;

	/*KARL
	if (!check_next(cmd, 1))
	{
		//free
		return (NULL);
	}
	*/

	//on doit envoyer t_element *cmd ici pour check les options cmd->next->type == OPTION
	path = getcwd(NULL, 0);
	//if (!path)
		//printf errno
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
		key->value = ft_calloc(ft_strlen(temp_pwd) + 100, sizeof(char));
		ft_strlcpy(key->value, temp_pwd, ft_strlen(temp_pwd) + 1);
		free(temp_pwd);
	}
}
