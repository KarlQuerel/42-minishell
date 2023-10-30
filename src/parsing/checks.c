/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karl <karl@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 17:07:47 by casomarr          #+#    #+#             */
/*   Updated: 2023/10/30 17:45:35 by karl             ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

/*When a quote is found in the command line, this function
checks if there is another quote of the same type further in
the command line. If yes, it returns true.*/
bool	quotes_can_close(char *line)
{
	size_t		i;
	char	type;
	
	i = 0;
	while (line[i])
	{
		if (line[i] == '\'' || line[i] == '\"')
		{
			if (line[i] == '\'')
				type = '\'';
			else if (line[i] == '\"')
				type = '\"';
			i++;
			if (line[i] == '\0') //new quote at end of line
				return (false);
			if (line[i] != type)
			{
				while(line[i] && line[i] != type)
					i++;
				if (line[i] == '\0')
					return (false);
			}
		}
			i++;
	}
	return (true);
}

/*Checks if the cmd sent to this function is a builtin.*/
bool		is_builtin(char *cmd_content)
{
	int		i;
	char	*builtins[9];
	
	builtins[0] = "echo";
	builtins[1] = "cd";
	builtins[2] = "pwd";
	builtins[3] = "export";
	builtins[4] = "unset";
	builtins[5] = "env";
	builtins[6] = "exit";
	builtins[7] = "history";
	builtins[8] = "$?";
	i = 0;
	while(i < 9)
	{
		if (ft_strncmp(cmd_content, builtins[i], ft_strlen(cmd_content)) == 0 && \
		ft_strlen(cmd_content) == ft_strlen(builtins[i]))
			return (true);
		i++;
	}
	return (false);
}

bool	is_user_in_path(char *path, t_env *env_list)
{
	int		i;
	int		j;
	t_env	*user;

	i = 0;
	user = find_value_with_key_env(env_list, "USER");
	while(path[i])
	{
		j = 0;
		if (path[i] == user->value[j])
		{
			while(path[i + j] == user->value[j] && user->value[j])
				j++;
			if (user->value[j] == '\0')
				return (true);
		}
		i++;
	}
	return (false);
}