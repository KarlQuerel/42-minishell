/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 13:55:33 by casomarr          #+#    #+#             */
/*   Updated: 2023/10/27 17:27:12 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

/*Gets the current path and prints it if the 
trigger reqiures it.*/
char	*pwd(int option)
{
	char *path;

	
	/*KARL
	if (!check_next_node_builtin(cmd, 1))
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
		printf("%s\n", path);
		//ft_putstr_fd(path, STDOUT_FILENO/* exec->fd[] */); // doit ecrire dans l'enfant exec->fd[0] pwd | wc -l exec->fd ls -a | wc -l fd[i][1]
	return (path);
}

t_env	*pwd_update_in_env(t_env *env_list)
{
	t_env	*temp;
	
	temp = find_value_with_key_env(env_list, "PWD");
	free(temp->value);
	temp->value = ft_calloc(ft_strlen(pwd(NO_PRINT)) + 100, sizeof(char));
	ft_strlcpy(temp->value, pwd(NO_PRINT), ft_strlen(pwd(NO_PRINT)) + 1);
	return(env_list);
}