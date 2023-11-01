/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: octonaute <octonaute@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 17:17:16 by carolina          #+#    #+#             */
/*   Updated: 2023/11/01 18:05:51 by octonaute        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/minishell.h"
#include "../libft/libft.h"


// int g_signals;
t_global g_signals;

//faire perror("Error") plutot que des printf pour toutes les fonctions qui utilisent errno
//utiliser ft_putstr_fd au lieu de printf

void	ft_welcome(void)
{
	printf("%s%s\n\n", BGRE, PC);
	printf("%s", WHT);
}

/*Deleted the /mnt/nfs/homes/casomarr/ part*/
char	*home_path_simplified(char *absolute_path, t_env *env_list)
{
	char	*path_from_home;
	char	*temp;
	t_env	*user;
	int		i;
	int		j;
	int		start;

	i = 0;
	start = 1;
	path_from_home = NULL;
	user = find_value_with_key_env(env_list, "USER");
	//je compares chaque element entre slashes au nom du user
	while(absolute_path[i])
	{
		if(absolute_path[i + 1] == '/')
		{
			temp = NULL;
			temp = strlcpy_middle(temp, absolute_path, start, i); // je copies absolute path jusqu a i dans temp
			start = i + 2; //begining of next word for next round
			if (ft_strncmp(temp, user->value, ft_strlen(user->value)) == 0 && ft_strlen(user->value) == ft_strlen(temp)) //je compares temp a user
			{
				//si sont les memes alors je mets dans path from home toutle reste de absolute path
				j = 0;
				i+=2;
				path_from_home = malloc(sizeof(char) * (ft_strlen(absolute_path) - i + 2));
				while (absolute_path[i])
					path_from_home[j++] = absolute_path[i++];
				path_from_home[j] = '\0';
				//printf("path = [%s]\n", path_from_home);
				free(temp);
				return (path_from_home);
			}
			free(temp);
		}
		i++;
	}
	return (NULL);
}

//PROTEGER TOUS MES MALLOCS!! --> avec perror

int main (int argc, char **argv, char **env)
{
	char                *line;
	char                *new_line;
	//struct sigaction    signal;
	t_env				*env_list;
	t_element			*cmd_list;
	t_pipe				*exec;
	t_history			*entries;


	exec = ft_calloc(1, sizeof(t_pipe));
	if (!exec)
	{
		perror("exec");
		exit(EXIT_FAILURE);
	}
/* 	sigemptyset(&signal.sa_mask);
	// signal.sa_flags = SA_SIGINFO;
	signal.sa_flags = SA_RESTART;
	signal.sa_handler = &signal_handler;
	if (sigaction(SIGINT, &signal, NULL) == -1 || \
	sigaction(SIGQUIT, &signal, NULL) == -1)
		return (EXIT_FAILURE); */

	/* signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
 */

	signal(SIGINT, signal_handler);
	signal(SIGQUIT, signal_handler);
	//signal(SIGQUIT, SIG_IGN);

	(void)argv;
	if (argc != 1)
	{
		printf("Error\nNo arguments accepted: run again with ./minishell\n"); //putsr
		return (EXIT_FAILURE);
	}
	ft_welcome();
	env_list = put_env_in_list(env);
	env_list->env = env;
	line = NULL;
	entries = NULL;

//--------------------------------
	prompt(env_list);
	printf("%sAVANT READLINE AVANT LA WHILE\n%s", YELLOW, RESET);
	line = readline("$ ");
	printf("%sAPRES READLINE AVANT LA WHILE\n%s", YELLOW, RESET);
/* 	printf("%sAVANT GNL AVANT LA WHILE\n%s", YELLOW, RESET);
	line = get_next_line(STDIN_FILENO); // fd 0 = stdin_fileno
	printf("%sAPRES GNL AVANT LA WHILE\n%s", YELLOW, RESET); */
//--------------------------------
	while (1)
	{
		printf("%sDEBUT DU MAIN\n%s", YELLOW, RESET);
		g_signals.location = IN_PROMPT;
/* 		if (commande en cours)
			ctrlD(line); */
		entries = ft_add_history(entries, line);
		printf("%sline = [%s]\n%s", YELLOW, line, RESET);
		if (line && ft_strncmp(line, "^C", ft_strlen(line)) != 0 )
		{
			/*J'envoie new_line au lieu de line aux fonctions qui suivent
			car sur bash qd on fait flèche du haut on retrouve la commande
			telle qu'elle avait été écrite alors qu'ici on la modifiait*/
			new_line = erase_spaces(line); //line est free ici
			if (line_errors_and_fix(&new_line) == true)
			{
				cmd_list = parsing(new_line, env_list);
				//ft_redirect(cmd_list); // a finir
				//printlist_test(cmd_list);
				ft_execute(cmd_list, env_list, exec, entries);
				free_cmd_list(cmd_list);
			}
			free(new_line); //en commentaire pour tests avec dollar
		}
		else
			free(line);
//--------------------------------
		env_list = pwd_update_in_env(/* cmd_list,  */env_list);
		/*
		dans le cas ou karl unset pwd, le prompt doit juste afficher $
		*/
		env_list->env = env;
		prompt(env_list);
		printf("%sAVANT READLINE DANS LA WHILE\n%s", YELLOW, RESET);
		line = readline("$ ");
		printf("%sAPRES READLINE DANS LA WHILE\n%s", YELLOW, RESET);
/* 		printf("%sAVANT GNL DANS LA WHILE\n%s", YELLOW, RESET);
		line = get_next_line(STDIN_FILENO); // fd 0 = stdin_fileno
		printf("%sAPRES GNL\n%s", YELLOW, RESET); */
//--------------------------------
		printf("%sFIN DU MAIN\n%s", YELLOW, RESET);
	}
	final_free(line, env_list, entries);
	return (EXIT_SUCCESS);
}

