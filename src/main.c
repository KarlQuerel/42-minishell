/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: octonaute <octonaute@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 17:17:16 by carolina          #+#    #+#             */
/*   Updated: 2023/11/09 16:45:36 by octonaute        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/minishell.h"
#include "../libft/libft.h"

////////////////////////////////////////
/* void set_terminal_mode() {
    struct termios t;
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag &= ~ICANON;  // Disable canonical mode
    t.c_lflag &= ~ECHO;    // Disable echoing
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

void reset_terminal_mode() {
    struct termios t;
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag |= ICANON;  // Enable canonical mode
    t.c_lflag |= ECHO;    // Enable echoing
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

char get_key() {
    char key;
    read(STDIN_FILENO, &key, 1);
    return key;
} */

/* void arrow_key_callback(int key) {
    if (key == 0x401B5B41) {
        // Check for the up arrow key (0x401B5B41 represents the up arrow key)
        printf("Up arrow key pressed!\n");
    }
} */

/* void up_arrow_callback(char* unused, int key) {
    if (key == 0x101) {
        // Check for the up arrow key (0x101 represents the up arrow key)
        printf("Up arrow key pressed!\n");
    }
} */

////////////////////////////////////////


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

void	signal_reset_prompt(int signo)
{
	(void)signo;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

int main (int argc, char **argv, char **env)
{
	char                *line;
	char                *new_line;
	struct sigaction    signal;
	t_env				*env_list;
	t_element			*cmd_list;
	t_pipe				*exec;
	t_history			*entries;
	char				*prompt;



///////////////////////////////////////
    // char key;
	// set_terminal_mode();

	// int n;
	// char buffer[3]; // Buffer to store escape sequences

	// rl_event_hook = arrow_key_callback;
	// rl_bind_key(0x101, up_arrow_callback); // Bind the up arrow key to the callback



/* 	struct termios term_settings;
    if (tcgetattr(STDIN_FILENO, &term_settings) == -1) {
        perror("tcgetattr");
        exit(EXIT_FAILURE);
    }

    struct termios modified_term_settings = term_settings;
    modified_term_settings.c_lflag &= ~(ICANON | ECHO); // Disable canonical mode and echoing

    if (tcsetattr(STDIN_FILENO, TCSANOW, &modified_term_settings) == -1) {
        perror("tcsetattr");
        exit(EXIT_FAILURE);
    }

    char escape_sequence[3] = {0}; // Buffer to store escape sequences
    int escape_sequence_length = 0; */
///////////////////////////////////////



	exec = ft_calloc(1, sizeof(t_pipe));
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
	line = NULL;
	entries = NULL;
	while (1)
	{
		g_signals.location = IN_PROMPT;
		prompt = ft_strjoin(ft_prompt(env_list, NO_PRINT), "$ ");
		line = readline(prompt);

/////////////////////////////////////

		// printf("line = [%s]\n", line);

/* 		key = get_key();

        if (key == '\x1b') // Check if the key is the escape character
		{
            char sequence[3];
            if (read(STDIN_FILENO, sequence, 3) == 3) 
			{
                if (sequence[0] == '[' && sequence[2] == 'A') // Up arrow key is pressed
                    history(entries, 1); //show precedent line
            }
        } */

		/* memset(buffer, 0, sizeof(buffer)); // Clear buffer
        n = read(STDIN_FILENO, buffer, sizeof(buffer) - 1); // Read input

        if (n == 3 && buffer[0] == 27 && buffer[1] == 91 && buffer[2] == 65) // Up arrow key is pressed
			history(entries, 1); //show precedent line */
/* 
		if (line != NULL)
		{
			printf("line = [%s]\n", line);
			if (ft_strncmp(line, "\x1b[A", ft_strlen(line)) == 0)
				history(entries, 1);
		} */

/* 		char c;
        if (read(STDIN_FILENO, &c, 1) == 1) {
            // Append the character to the escape sequence buffer
            escape_sequence[escape_sequence_length++] = c;

            // Check if we have received a complete escape sequence for the up arrow key
            if (escape_sequence_length == 3 &&
                escape_sequence[0] == 27 && // ASCII code for escape character
                escape_sequence[1] == '[' && escape_sequence[2] == 'A') {
                printf("HEY\n");
                escape_sequence_length = 0; // Reset the buffer
            } else if (c == 'q') {
                // Exit when 'q' is pressed
                break;
            } else if (escape_sequence_length >= 3) {
                // Reset the buffer if the escape sequence is not recognized
                escape_sequence_length = 0;
            }
        }
 */
////////////////////////////////////////////
		if (sigaction(SIGQUIT, &signal, NULL) == 0 && g_signals.location == IN_PROMPT)
			signal.sa_handler = SIG_IGN;
		
 		/*if (commande en cours)
			ctrlD(line); */
		entries = ft_add_history(entries, line);
		// add_history(line);
		new_line = erase_spaces(line);
		if (new_line != NULL)
		{
			/*J'envoie new_line au lieu de line aux fonctions qui suivent
			car sur bash qd on fait flèche du haut on retrouve la commande
			telle qu'elle avait été écrite alors qu'ici on la modifiait*/
			if (line_errors_and_fix(&new_line) == true)
			{
				cmd_list = parsing(new_line, env_list);
				// printlist_test(cmd_list);
				ft_execute(cmd_list, env_list, exec, entries);
				free_cmd_list(cmd_list);
			}
		}
		free(new_line); //en commentaire pour tests avec dollar
//--------------------------------
		env_list = pwd_update_in_env(/* cmd_list,  */env_list);
		/*
		dans le cas ou karl unset pwd, le prompt doit juste afficher $
		*/
		env_list->env = env;
//--------------------------------
	}
	
	final_free(line, env_list, entries);

///////////////////////////////////////
	// reset_terminal_mode();


/* 	// Restore terminal settings
    if (tcsetattr(STDIN_FILENO, TCSANOW, &term_settings) == -1) {
        perror("tcsetattr");
        exit(EXIT_FAILURE);
    } */
///////////////////////////////////////
	
	return (EXIT_SUCCESS);
}
