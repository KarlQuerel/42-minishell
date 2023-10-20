/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
<<<<<<< HEAD
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 17:17:16 by carolina          #+#    #+#             */
/*   Updated: 2023/10/18 13:17:50 by kquerel          ###   ########.fr       */
=======
/*   By: octonaute <octonaute@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 17:17:16 by carolina          #+#    #+#             */
/*   Updated: 2023/10/19 18:48:47 by octonaute        ###   ########.fr       */
>>>>>>> main
/*                                                                            */
/* ************************************************************************** */


#include "../includes/minishell.h"
#include "../libft/libft.h"

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
	struct sigaction    signal;
	t_env				*env_list;
	t_element			*cmd_list;
	t_pipe				*exec;
	
	
	
	exec = ft_calloc(1, sizeof(t_pipe));
	if (!exec)
	{
		perror("exec");
		exit(EXIT_FAILURE);
	}


	//utiliser ft_calloc a la place de malloc, dans les cas ou malloc ft_init pour tout mettre a zero
	

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
	using_history();
	line = NULL;

	// printf("pwd : %s\n", pwd(NO_PRINT));
	// printf("simplified : %s\n", home_path_simplified(pwd(NO_PRINT), env_list));
	
///////////////////////
/* 	//Gpath = find_value_with_key_env(env_list, "PATH");
	user = find_value_with_key_env(env_list, "USER");
	word = NULL;
	int i = ft_strlen(pwd(NO_PRINT)) - 2; //pour sauter le dernier slash
	while(i > 0)
	{
		if (pwd(NO_PRINT)[i - 1] == '/')
			break;
		i--;
	}
	word = strlcpy_middle(word, pwd(NO_PRINT), i, ft_strlen(pwd(NO_PRINT)));
	// printf("word = %s\n", word);
	// printf("user = %s\n", user->value);
	if (ft_strncmp(word, user->value, ft_strlen(user->value)) != 0)
	{
		path = home_path_simplified(pwd(NO_PRINT), env_list);
		printf("%s", path);
	}
	line = readline("$ "); */
////////////////////


//--------------------------------
	prompt(env_list);
	line = readline("$ ");
//--------------------------------

	while (is_this_command(line, "exit") == false) // while (1) et mettre line = readline("$ ") tout en haut de la while
	//et mettre en commentaire le line = readline("$ ") du bas de la while
	{
		if (feof(stdin)) // pour ctrl + D?? // ne le comprend pas
		{
			printf("CTRL + D detected\n");
			final_free(line, env_list);
			return (EXIT_SUCCESS);
		}
		add_history(line);


	/*J'envoie new_line au lieu de line aux fonctions qui suivent
	car sur bash qd on fait flèche du haut on retrouve la commande
	telle qu'elle avait été écrite alors qu'ici on la modifiait*/
		new_line = erase_spaces(line);
		new_line = line_errors_and_fix(new_line, env_list);
	/*SI line_errors_and_fix TROUVE DES ERREURS IL NE FAUDRAIT PAS ENTRER DANS PARSING*/
		cmd_list = parsing(new_line, env_list);
		//ft_env(env_list, 0);
		// ft_export(cmd_list, env_list);
		//ft_redirect(cmd_list); // a finir
		//ft_execute(cmd_list, env_list, exec);
		//printlist_test(cmd_list);

<<<<<<< HEAD
		//commands(new_line, env_list, home_path); // À effacer : c'est juste pour test mes builtins tant que ton exec est en commentaire
=======
		//commands(cmd_list, env_list, home_path); // À effacer : c'est juste pour test mes builtins tant que ton exec est en commentaire
		//echo(cmd_list);
		cd(cmd_list, env_list);
>>>>>>> main

		//free(new_line); //en commentaire pour tests avec dollar
		free_cmd_list(cmd_list);
/* 		path = home_path_simplified(pwd(NO_PRINT), env_list);
		printf("%s", path);
		line = readline("$ "); */

///////////////////////
/* 		//Gpath = find_value_with_key_env(env_list, "PATH");
		user = find_value_with_key_env(env_list, "USER");
		word = NULL;
		int i = ft_strlen(pwd(NO_PRINT)) - 2; //pour sauter le dernier slash
		while(i > 0)
		{
			if (pwd(NO_PRINT)[i - 1] == '/')
				break;
			i--;
		}
		word = strlcpy_middle(word, pwd(NO_PRINT), i, ft_strlen(pwd(NO_PRINT)));
		// printf("word = %s\n", word);
		// printf("user = %s\n", user->value);
		if (ft_strncmp(word, user->value, ft_strlen(user->value)) != 0)
		{
			path = home_path_simplified(pwd(NO_PRINT), env_list);
			printf("%s", path);
		}
		line = readline("$ "); */
////////////////////
//--------------------------------
		prompt(env_list);
		line = readline("$ ");
//--------------------------------

	}
	final_free(line, env_list);
	return (EXIT_SUCCESS);
}
