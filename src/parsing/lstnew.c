/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lstnew.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: casomarr <casomarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 17:59:32 by casomarr          #+#    #+#             */
/*   Updated: 2023/10/24 15:44:08 by casomarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

/*To add a new node when creating the cmd_list.*/
t_element	*lstnew(char *line, int i, int type)
{
	t_element	*new;

	//new = (t_element *)malloc(sizeof(*new));
	new = (t_element *)ft_calloc(1, sizeof(t_element));
	if (!new)
		return (NULL);
	/*LIGNE DU DESSOUS : j'ai mis + 100 pour regler les pb de valgrind mais 
	je ne peux pas le laisser comme ca!!*/
	new -> content = ft_calloc(size_of_command(line, i, type) + 100, sizeof(char));
	new -> type = 0;
	new -> builtin = false;
	new -> next = NULL;
	return (new);
}

/*To add a new node when creating the env_list.*/
t_env	*lstnew_env(char *line, int i)
{
	t_env	*new;

	// new = (t_env *)malloc(sizeof(*new));
	new = (t_env *)ft_calloc(1, sizeof(t_env));
	if (!new)
		return (NULL);
	// new -> key = malloc(sizeof(char) * size_of_command(line, i, KEY));
	/*LIGNE DU DESSOUS : j'ai mis + 100 pour regler les pb de valgrind mais 
	je ne peux pas le laisser comme ca!!*/
	new -> key = ft_calloc(size_of_command(line, i, KEY) + 100, sizeof(char));
	// new -> value = malloc(sizeof(char) * size_of_command(line, i, VALUE));
	new -> value = ft_calloc(size_of_command(line, i, VALUE), sizeof(char));
	new -> next = NULL;
	return (new);
}

t_history	*lstnew_history(char *line, int size_of_list)
{
	t_history	*new;

	new = (t_history *)ft_calloc(1, sizeof(t_history));
	if (!new)
		return (NULL);
	new->cmd = ft_calloc(ft_strlen(line) + 1, sizeof(char));
	ft_strlcpy(new->cmd, line, ft_strlen(line) + 1);
	new->nb = size_of_list;
	new->next = NULL;
	return (new);
}