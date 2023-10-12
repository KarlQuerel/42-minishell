/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: casomarr <casomarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 17:17:16 by carolina          #+#    #+#             */
/*   Updated: 2023/10/12 18:19:21 by casomarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/minishell.h"
#include "../libft/libft.h"

//printf("%sHELLO%s\n", GREEN, RESET); //green et reset defined dans le .h
//faire perror("Error") plutot que des printf pour toutes les fonctions qui utilisent errno
//utiliser ft_putstr_fd au lieu de printf

void	ft_welcome(void)
{
	printf("%s%s\n\n", BGRE, PC);
	printf("%s", WHT);
}

//PROTEGER TOUS MES MALLOCS!! --> avec perror

int main (int argc, char **argv, char **env)
{
	char                *line;
	char                *home_path;
	struct sigaction    signal;
	t_env				*env_list;
	t_element			*cmd_list;
	t_pipe				*exec;
	t_env				*path;

	exec = malloc(sizeof(t_pipe));
	if (!exec)
	{
		perror("exec");
		exit(EXIT_FAILURE);
	}
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
		printf("Error\nNo arguments accepted: run again with ./minishell\n"); //putsr
		return (EXIT_FAILURE);
	}
	ft_welcome();
	env_list = put_env_in_list(env);
	env_list->env = env;
	path = find_value_with_key_env(env_list, "PWD");
	home_path = path->value;
	using_history(); // initialisation de l'historique
	line = NULL;
	line = readline("Minishell $ ");
	check_commands_grammar(line); //fonction pas encore terminee
	while (is_this_command(line, "exit") == false)
	{
		if (feof(stdin)) // pour ctrl + D?? // ne le comprend pas
		{
			printf("CTRL + D detected\n");
			final_free(line, env_list);
			return (EXIT_SUCCESS);
		}
		//printf("line before : [%s]\n", line);
		line = erase_spaces(line);
		//printf("line after : [%s]\n", line);
		add_history(line);
		line = commands(line, env_list, home_path);
		cmd_list = parsing(line);
		ft_redirect(cmd_list); // a finir
		ft_execute(cmd_list, env_list, exec);
		//printf("APRES PARSING FIX\n");
		//printlist_test(cmd_list);
		free(line);
		free_cmd_list(cmd_list);
		line = readline("Minishell $ ");
	}
	final_free(line, env_list);
	return (EXIT_SUCCESS);
}
