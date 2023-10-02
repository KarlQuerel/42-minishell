/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lstnew.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: casomarr <casomarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 17:59:32 by casomarr          #+#    #+#             */
/*   Updated: 2023/09/28 14:01:26 by casomarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

t_element	*lstnew(char *line, int i)
{
	t_element	*new;

	new = (t_element *)malloc(sizeof(*new));
	if (new == NULL)
		return (NULL);
	new -> content = malloc(sizeof(char) * size_of_command(line, i, CMD) + 1);
	new -> type = 0;
	new -> next = NULL;
	//new -> prev = NULL; //
	return (new);
}

t_env	*lstnew_env(char *line, int i)
{
	t_env	*new;

	new = (t_env *)malloc(sizeof(*new));
	if (new == NULL)
		return (NULL);
	new -> key = malloc(sizeof(char) * size_of_command(line, i, KEY) + 1);
	new -> value = malloc(sizeof(char) * size_of_command(line, i, VALUE) + 1);
	new -> next = NULL;
	return (new);
}