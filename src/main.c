/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: casomarr <casomarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 17:17:16 by carolina          #+#    #+#             */
/*   Updated: 2023/09/30 14:14:47 by casomarr         ###   ########.fr       */
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
	check_commands_grammar(line);
	while (is_this_command(line, "exit") == false)
	{
		add_history(line);
		line = commands(line, env_list);
		cmd_list = parsing(line);
		free(line);
		free_cmd_list(cmd_list);
		line = readline("Karlinashell $ ");
		// if (feof(stdin)) // pour ctrl + D??
		// 	return (EXIT_SUCCESS);
	}
	final_free(line, env_list);
	return (EXIT_SUCCESS);
}