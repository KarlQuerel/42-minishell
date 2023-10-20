/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lstnew.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
<<<<<<< HEAD
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 17:59:32 by casomarr          #+#    #+#             */
/*   Updated: 2023/10/18 14:35:52 by kquerel          ###   ########.fr       */
=======
/*   By: octonaute <octonaute@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 17:59:32 by casomarr          #+#    #+#             */
/*   Updated: 2023/10/19 18:00:41 by octonaute        ###   ########.fr       */
>>>>>>> main
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

/*To add a new node when creating the cmd_list.*/
t_element	*lstnew(char *line, int i, int type)
{
	t_element	*new;

<<<<<<< HEAD
	// new = (t_element *)malloc(sizeof(*new));
=======
	//new = (t_element *)malloc(sizeof(*new));
>>>>>>> main
	new = (t_element *)calloc(1, sizeof(t_element));
	if (!new)
		return (NULL);
	/*LIGNE DU DESSOUS : j'ai mis + 100 pour regler les pb de valgrind mais 
	je ne peux pas le laisser comme ca!!*/
	new -> content = calloc(size_of_command(line, i, type) + 100, sizeof(char));
	new -> type = 0;
	new -> builtin = false;
	new -> next = NULL;
	return (new);
}

/*To add a new node when creating the env_list.*/
t_env	*lstnew_env(char *line, int i)
{
	t_env	*new;

<<<<<<< HEAD
	// new = (t_env *)malloc(sizeof(*new));
=======
	//new = (t_env *)malloc(sizeof(*new));
>>>>>>> main
	new = (t_env *)calloc(1, sizeof(t_env));
	if (!new)
		return (NULL);
	// new -> key = malloc(sizeof(char) * size_of_command(line, i, KEY));
	/*LIGNE DU DESSOUS : j'ai mis + 100 pour regler les pb de valgrind mais 
	je ne peux pas le laisser comme ca!!*/
	new -> key = calloc(size_of_command(line, i, KEY) + 100, sizeof(char));
	// new -> value = malloc(sizeof(char) * size_of_command(line, i, VALUE));
	new -> value = calloc(size_of_command(line, i, VALUE), sizeof(char));
	new -> next = NULL;
	return (new);
}