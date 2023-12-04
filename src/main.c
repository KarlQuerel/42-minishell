/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: casomarr <casomarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 17:17:16 by carolina          #+#    #+#             */
/*   Updated: 2023/12/04 13:46:04 by casomarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_location;
//PROTEGER TOUS MES MALLOCS!! --> avec perror
//faire perror("Error") plutot que des printf pour toutes les fonctions qui utilisent errno
//utiliser ft_putstr_fd au lieu de printf

void	printlist_test(t_element *head) // A EFFACER A LA FIN
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

bool	is_exit_status_in_line(char *big, char *little)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (!big || !little)
		return (false); //?
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


int main (int argc, char **argv, char **env)
{
	char				*line;
	t_env				*env_list;
	t_element			*cmd_list;
	t_pipe				*exec;
	char				*prompt;
	char				*path;

	if (argc != 1)
		return (msg_error(0, ""), EXIT_FAILURE);
	(void)argv;
	env_list = put_env_in_list(env);
	using_history();
	line = NULL;
	exec = ft_calloc(1, sizeof(t_pipe));
	if (!exec)
	{
		msg_error(1, "");
		//ft_lst_clear(env_list); // pensez a free ici ;)
		exit(EXIT_FAILURE);
	}
	while (1)
	{
		g_location = IN_PROMPT;
		if (set_signals() == EXIT_FAILURE)
		{
			//free
			ft_putendl_fd("FAILURE", STDERR_FILENO);
			return (1);
		}
		if (is_key_in_env(env_list, "EXIT_STATUS") == false)
			add_exit_status_in_env(&env_list, 0);
		path = ft_prompt(env_list, NO_PRINT);
		prompt = ft_strjoin(path, "$ ");
		if (compare(path, "/") == false && compare(path, "") == false) //if malloc'ed
			free(path);
		line = readline(prompt);
		if (g_location == QUIT)
			add_exit_status_in_env(&env_list, 130);
		add_history(line);
		if (line == NULL)
		{
			if (g_location == IN_PROMPT)
			{
				ft_putendl_fd("exit", STDERR_FILENO);
				ctrld_free(line, prompt, env_list, exec);
			}
			else if (g_location == IN_COMMAND)
			{
				ft_putchar_fd('\n', STDERR_FILENO);
				rl_reset_line_state();
			}
		}
		if (line != NULL)
		{
			line = erase_spaces(line);
			if (line_errors_and_fix(line) == true)
			{
				cmd_list = parsing(line, env_list);
				//printlist_test(cmd_list);
				if (cmd_list != NULL)
				{
					exec->line = &line;
					exec->prompt = &prompt;
					ft_execute(cmd_list, &env_list, exec);
					free_cmd_arr(exec); //double free qd heredoc
					ft_unlink(cmd_list);
				
					if (compare(line, "$") == false)
					{
						if (free_cmd_list_parent(cmd_list) == 1)
							cmd_list = NULL;
					}
				}
			}
		}
		if (compare(line, "$") == true)
		{
			free(cmd_list);
			cmd_list = NULL;
		}
		free(line);
		line = NULL;
		free(prompt);
		pwd_update_in_env(&env_list);
	}
	return (EXIT_SUCCESS);
}
