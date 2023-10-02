/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 17:17:16 by carolina          #+#    #+#             */
/*   Updated: 2023/10/02 17:29:33 by casomarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../libft/libft.h"


//PROTEGER TOUS MES MALLOCS!!

int main (int argc, char **argv, char **env)
{
	char                *line;
	struct sigaction    signal;
	t_env				*env_list;
	t_element			*cmd_list;

	sigemptyset(&signal.sa_mask);
	// signal.sa_flags = SA_SIGINFO;
	signal.sa_flags = SA_RESTART;
	signal.sa_handler = &signal_handler;
	if (sigaction(SIGINT, &signal, NULL) == -1 || \
	sigaction(SIGQUIT, &signal, NULL) == -1)
		return (EXIT_FAILURE);

	(void)argv;
	if (argc != 1)
	{
		printf("Error \nNo arguments accepted : run again with ./minishell\n"); //putsr
		return (EXIT_FAILURE);
	}
	env_list = put_env_in_list(env);
	using_history(); // initialisation de l'historique
	line = NULL;
	line = readline("Karlinashell $ ");
	//check_commands_grammar(line); //fonction pas encore terminee
	while (is_this_command(line, "exit") == false)
	{
		if (feof(stdin)) // pour ctrl + D?? // ne le comprend pas
		{
			printf("CTRL + D detected\n");
			final_free(line, env_list);
			return (EXIT_SUCCESS);
		}
		add_history(line);
		line = commands(line, env_list);
		cmd_list = parsing(line); //KARL : cmd_line est la liste doublement chainee avec toutes les commandes
		cmd_list = parsing_fix(cmd_list);
    ft_redirect(cmd_list); // fonction test
		ft_print_path(env_list); // fonction test
		//printlist_test(cmd_list);
		free(line);
		free_cmd_list(cmd_list);
		line = readline("Karlinashell $ ");
	}
	final_free(line, env_list);
	return (EXIT_SUCCESS);
}