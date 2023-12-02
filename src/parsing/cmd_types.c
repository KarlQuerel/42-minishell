/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_types.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 18:58:39 by casomarr          #+#    #+#             */
/*   Updated: 2023/12/02 22:49:26 by kquerel          ###   ########.fr       */
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
	{
		size++;
		len++;
	}
	return (size);
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

/*Returns the size of a str (= argument in between two quotes).
If quote found returns size + 1 to get the real size. Otherwise,
no need to add 1 because the last size++ does the job.
Command = command line
len = where is the begining of the string in the command line
(command[len] is the first letter of the string, not the first quote)*/
int	str_type(char *command, int len)
{
	int		size;
	char	quote;

	size = 0;
	quote = command[len];
	while (command[len])
	{
		if (command[len + 1] && command[len + 1] == quote)
		{
			size += 1;
			break ;
		}
		size++;
		len++;
	}
	return (size);
}
