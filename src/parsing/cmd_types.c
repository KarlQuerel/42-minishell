/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_types.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 18:58:39 by casomarr          #+#    #+#             */
/*   Updated: 2023/12/08 00:51:28 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*Returns the size of a cmd.
Command = command line
len = where is the begining of the cmd in the command line*/
int	cmd_type(char *command, int len)
{
	int	size;

	size = 0;
	if (!command)
		return (0);
	while (command[len] != ' ' && command[len] != '|' && \
	command[len] != '<' && command[len] != '>' && command[len] != '\0')
		cmd_type_loop(command, &len, &size);
	return (size);
}

void	cmd_type_loop(char *command, int *len, int *size)
{
	if (command[(*len)] == '\'')
	{
		(*len)++;
		while (command[(*len)] && command[(*len)] != '\'')
		{
			(*size)++;
			(*len)++;
		}
		(*len)++;
	}
	else if (command[(*len)] == '\"')
	{
		(*len)++;
		while (command[(*len)] != '\"')
		{
			(*size)++;
			(*len)++;
		}
		(*len)++;
	}
	else
	{
		(*size)++;
		(*len)++;
	}
}

/*Returns the size of the environment key or its value,
depending on the trigger.
The return at the end of function returns the size of the value.*/
int	key_and_value_type(char *command, int len, int type)
{
	int	size;

	size = 0;
	if (!command)
		return (0);
	while (command[len] != '=')
	{
		size++;
		len++;
	}
	if (type == KEY)
		return (size);
	len++;
	while (command[len])
	{
		size++;
		len++;
	}
	return (size);
}
