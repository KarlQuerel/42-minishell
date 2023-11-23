/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 17:17:16 by carolina          #+#    #+#             */
/*   Updated: 2023/11/23 19:49:09 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../libft/libft.h"

t_global g_signals;

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

//PROTEGER TOUS MES MALLOCS!! --> avec perror
//faire perror("Error") plutot que des printf pour toutes les fonctions qui utilisent errno
//utiliser ft_putstr_fd au lieu de printf

/* Badass welcome message made with love */
void	ft_welcome(void)
{
	printf("%s%s\n\n", BGRE, PC);
	printf("%s", WHT);
}


int main (int argc, char **argv, char **env)
{
	char				*line;
	//struct sigaction	signal;
	t_env				*env_list;
	t_element			*cmd_list;
	t_pipe				*exec;
	char				*prompt;
	char				*path; //ai du le creer pour free ft_prompt correctement

	exec = ft_calloc(1, sizeof(t_pipe));
	if (!exec)
	{
		perror("exec");
		exit(EXIT_FAILURE);
	}

	/* memset(&signal, 0, sizeof(signal));
	signal.sa_flags = SA_SIGINFO | SA_RESTART;
	signal.sa_handler = &signal_handler;
	if (sigaction(SIGINT, &signal, NULL) == -1 || \
	sigaction(SIGQUIT, &signal, NULL) == -1)
		return (EXIT_FAILURE); */

	(void)argv;
	if (argc != 1)
	{
		printf("Error\nNo arguments accepted: run again with ./minishell\n"); //putsr
		return (EXIT_FAILURE);
	}
	ft_welcome();
	env_list = put_env_in_list(env);
	using_history(); // initialisation de l'historique
	env_list->env = env;
	line = NULL;
	g_signals.exit_status = 0;
	while (1)
	{
		g_signals.location = IN_PROMPT;
		if (set_signals() == EXIT_FAILURE)
		{
			//free
			return (1);
		}
		exitstatus_update_in_env(&env_list);
		path = ft_prompt(env_list, NO_PRINT);
		prompt = ft_strjoin(path, "$ ");
		if (ft_strncmp(path, "/", ft_strlen(path)) != 0 && \
		ft_strncmp(path, "", ft_strlen(path)) != 0) //if malloc'ed
			free(path);
		line = readline(prompt);
		add_history(line);
		
		if (line == NULL)
		{
			if (g_signals.location == IN_PROMPT)
				ctrld_free(line, prompt, env_list, exec);
			else
			{
				ft_putchar_fd('\n', STDERR_FILENO);
				rl_reset_line_state();
			}
		}
		if (line != NULL)
		{
			//printf("line AVANT : [%s]\n", line);
			line = erase_spaces(line);
			// printf("line APRES : [%s]\n", line);
			if (line_errors_and_fix(line) == true)
			{
				cmd_list = parsing(line, env_list);
				// printlist_test(cmd_list);
				exec->line = &line;
				exec->prompt = &prompt;
				ft_execute(cmd_list, &env_list, exec);
				free_cmd_list(cmd_list);
				//free_cmd_arr(exec);
			}
		}
		free(line);
		free(prompt);
		pwd_update_in_env(&env_list);
		env_list->env = env;
	}
	return (EXIT_SUCCESS);
}
