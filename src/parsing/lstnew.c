/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lstnew.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: casomarr <casomarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 17:59:32 by casomarr          #+#    #+#             */
/*   Updated: 2023/10/17 16:14:32 by casomarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

/*To add a new node when creating the cmd_list.*/
t_element	*lstnew(char *line, int i, int type)
{
	t_element	*new;

	new = (t_element *)malloc(sizeof(*new));
	if (!new)
		return (NULL);
	new -> content = calloc(size_of_command(line, i, type), sizeof(char));
	new -> type = 0;
	new -> builtin = false;
	new -> next = NULL;
	return (new);
}

/*To add a new node when creating the env_list.*/
t_env	*lstnew_env(char *line, int i)
{
	t_env	*new;

	new = (t_env *)malloc(sizeof(*new));
	if (!new)
		return (NULL);
	new -> key = malloc(sizeof(char) * size_of_command(line, i, KEY));
	new -> value = malloc(sizeof(char) * size_of_command(line, i, VALUE));
	new -> next = NULL;
	return (new);
}