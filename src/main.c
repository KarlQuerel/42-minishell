/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 17:17:16 by carolina          #+#    #+#             */
/*   Updated: 2023/11/30 12:29:58 by kquerel          ###   ########.fr       */
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


int main (int argc, char **argv, char **env)
{
	char				*line;
	t_env				*env_list;
	t_element			*cmd_list;
	t_pipe				*exec;
	char				*prompt;
	char				*path; //ai du le creer pour free ft_prompt correctement

	if (argc != 1)
		return (msg_error(0, ""), EXIT_FAILURE);
	ft_welcome();
	(void)argv;
	env_list = put_env_in_list(env);
	using_history(); // initialisation de l'historique
	line = NULL;
	add_exit_status_in_env(&env_list, 0);

	exec = ft_calloc(1, sizeof(t_pipe));
	if (!exec)
	{
		msg_error(1, "");
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
		if (ft_strncmp(path, "/", ft_strlen(path)) != 0 && \
		ft_strncmp(path, "", ft_strlen(path)) != 0) //if malloc'ed
			free(path);
		line = readline(prompt);
		if (g_location == QUIT)
			add_exit_status_in_env(&env_list, 130);
		add_history(line);
		if (line == NULL)
		{
			if (g_location == IN_PROMPT)
			{
				printf("line = %s\n", line);
				//DANS LE CAS DES REDIRS, LINE EST NULLE
				printf("ca quitte le zigouigoui\n"); 
				//printf("cas line = NULL ou control D\n"); 
				//CARO --> dans le cas de juste des redirections ca rentre dans cette while
				// par exemple --> '> a > b > c'
				//PAREIL pour ligne avec juste heredoc --> '<< heredoc'
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
				//printlist(cmd_list);
				if (cmd_list != NULL)
				{
					exec->line = &line;
					exec->prompt = &prompt;
					ft_execute(cmd_list, &env_list, exec);
					free_cmd_arr(exec); //double free qd heredoc
					free(cmd_list->hd_filename);
					if (ft_strncmp(line, "$?", 2) != 0) //condition strlen
						free_cmd_list(cmd_list); //free A REGARDER AVEC KARL
					//ft_unlink(cmd_list);
				}
			}
		}
		free(line);
		line = NULL;
		free(prompt);
		pwd_update_in_env(&env_list);
	}
	return (EXIT_SUCCESS);
}
