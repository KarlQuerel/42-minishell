/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karl <karl@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 17:17:16 by carolina          #+#    #+#             */
/*   Updated: 2023/11/14 19:59:01 by karl             ###   ########.fr       */
/*                                                                            */
/******************************************************************************/


#include "../includes/minishell.h"
#include "../libft/libft.h"

t_global g_signals;

//PROTEGER TOUS MES MALLOCS!! --> avec perror
//faire perror("Error") plutot que des printf pour toutes les fonctions qui utilisent errno
//utiliser ft_putstr_fd au lieu de printf

void	ft_welcome(void)
{
	printf("%s%s\n\n", BGRE, PC);
	printf("%s", WHT);
}


int main (int argc, char **argv, char **env)
{
	char				*line;
	struct sigaction	signal;
	t_env				*env_list;
	t_element			*cmd_list;
	t_pipe				*exec;
	t_history			*entries;
	char				*prompt;

	exec = ft_calloc(1, sizeof(t_pipe));
	if (!exec)
	{
		perror("exec");
		exit(EXIT_FAILURE);
	}

	memset(&signal, 0, sizeof(signal));
	signal.sa_handler = &signal_handler;
	if (sigaction(SIGINT, &signal, NULL) == -1 || \
	sigaction(SIGQUIT, &signal, NULL) == -1)
		return (EXIT_FAILURE);

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
	entries = NULL;
	while (1)
	{
		g_signals.location = IN_PROMPT;
		prompt = ft_strjoin(ft_prompt(env_list, NO_PRINT), "$ ");
		line = readline(prompt);
		add_history(line);
		
 		/*if (commande en cours)
			ctrlD(line); */
		
		if (line != NULL)
		{
			line = erase_spaces(line);
			if (line_errors_and_fix(&line) == true)
			{
				cmd_list = parsing(line, env_list);
				//printlist_test(cmd_list);
				ft_execute(cmd_list, env_list, exec);
				free_cmd_list(cmd_list);
			}
		}
		free(line);
		free(prompt);
		pwd_update_in_env(&env_list);
		env_list->env = env;
	}
	final_free(line, env_list);
	return (EXIT_SUCCESS);
}
