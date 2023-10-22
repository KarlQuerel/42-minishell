/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karl <karl@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 13:55:33 by casomarr          #+#    #+#             */
/*   Updated: 2023/10/22 20:57:54 by karl             ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../../../includes/minishell.h"
#include "../../../libft/libft.h"

/*Gets the current path and prints it if the 
trigger reqiures it.*/
char	*pwd(int option)
{
	char *path;

	path = getcwd(NULL, 0);
	//if (!path)
		//printf errno
	if (option == PRINT)
		printf("%s\n", path);
	return (path);
}
