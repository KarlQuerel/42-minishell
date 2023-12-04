/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 17:17:16 by carolina          #+#    #+#             */
/*   Updated: 2023/12/04 14:43:48 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_location;

bool	is_exit_status_in_line(char *big, char *little)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (!big || !little)
		return (false);
	while (big[i])
	{
		j = 0;
		while (big[i + j] == little[j] && little[j])
		{
			j++;
			if (little[j] == '\0')
				return (true);
		}
		i++;
	}
	return (false);
}

int	main(int argc, char **argv, char **env)
{
	char				*line;
	t_env				*env_list;
	t_element			*cmd_list;
	t_pipe				*exec;
	char				*path;

	(void)argv;
	if (argc != 1)
		return (msg_error(0, ""), EXIT_FAILURE);
	line = NULL;
	path = NULL;
	set_struct_null(&env_list, &cmd_list, &exec);
	exec = init_struct(exec, &env_list, env);
	while (1)
	{
		if (ft_mini_prompt(&env_list, &path, exec, &line))
			return (1);
		if (line == NULL)
			line_null(line, &env_list, exec);
		if (line != NULL)
			line_not_null(&line, cmd_list, &env_list, exec);
		free_and_update(line, cmd_list, exec, &env_list);
	}
	return (EXIT_SUCCESS);
}
