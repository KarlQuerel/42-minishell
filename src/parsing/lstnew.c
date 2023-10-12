/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lstnew.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: casomarr <casomarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 17:59:32 by casomarr          #+#    #+#             */
/*   Updated: 2023/10/12 14:32:33 by casomarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

t_element	*lstnew(char *line, int i)
{
	t_element	*new;

	// printf("%sLSTNEW\n%s", GREEN, RESET);
	new = (t_element *)malloc(sizeof(*new));
	if (!new)
		return (NULL);
	// printf("%ssize_of_command dans lstnew = %d\n%s", YELLOW, size_of_command(line, i, CMD), RESET);
	// new -> content = malloc(sizeof(char) * size_of_command(line, i, CMD));
	new -> content = calloc(size_of_command(line, i, CMD), sizeof(char));
	new -> type = 0;
	new -> builtin = false;
	new -> next = NULL;
	//new -> prev = NULL; //
	return (new);
}

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