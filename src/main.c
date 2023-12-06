/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: octonaute <octonaute@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 17:17:16 by carolina          #+#    #+#             */
/*   Updated: 2023/12/06 20:04:00 by octonaute        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_location;



void	printlist(t_element *head) // A EFFACER A LA FIN
{
	int	i;

	i = 0;
	while (head)
	{
		// if (i != 0)
		// 	printf("prev cmd = %s\n", head->prev->content);
		printf("--------------------------------\n");
		printf("content = ---%s---\n", head->content);
		printf("type = ---%d---\n", head->type);
		printf("builtin = ---%d---\n", head->builtin);
		printf("--------------------------------\n");
		// if (head->next != NULL)
		// 	printf("next cmd = %s\n", head->next->content);
		head = head->next;
		i++;
	}
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
		else if (line != NULL)
			line_not_null(&line, cmd_list, &env_list, exec);
		free_and_update(line, cmd_list, exec, &env_list);
	}
	return (EXIT_SUCCESS);
}
