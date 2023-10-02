/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 14:46:12 by kquerel           #+#    #+#             */
/*   Updated: 2023/10/02 17:53:18 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

void	ft_redirect(t_element *s)
{
	while (s)
	{
		printf("%s\n", s->content);
		if (s->type == 0)
			printf(" -> pipe\n");
		else if (s->type == 1)
			printf(" -> pipe - option\n");
		else if (s->type == 2)
			printf(" -> arg\n");
		else if (s->type == 3)
			printf(" -> fd_infile\n");
		else if (s->type == 4)
			printf(" -> fd_outfile\n");
		else if (s->type == 5)
			printf(" -> fd_infile HEREDOC\n");
		else if (s->type == 6)
			printf(" -> fd_outfile HEREDOC\n");
		else if (s->type == 7)
			printf(" -> pipec_bonus\n");
		s = s->next;
	}
}

/*
- pipe(pipe_end), returns -1 upon failure
- assigns fork return value to pid_t pid_child1;
	-> fork returns -1 upon failure
- in first child
	-> open assigned to fd_infile (av[1], )
	-> dup2 (fd_infile)
	-> dup2 (pipe_end[1])

	
	

- waitpid(pid_child1, NULL, 0)
- waitpid(pid_child2, NULL, 0)

*/

void	ft_print_path(t_env *env_list)
{
	
	t_env	*to_print;
	to_print = env_list;
	to_print = find_value_with_key_env(to_print, "PATH");
	
	printf("t_env = ::: %s :::\n", to_print->value);


	char **res_split;
	res_split = ft_split(to_print->value, ':');
	int	i = 0;
	while (res_split) // a gerer
	{
		printf("%s\n", res_split[i]);
		i++;
	}
	
}