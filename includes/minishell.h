/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 17:11:19 by carolina          #+#    #+#             */
/*   Updated: 2023/10/02 16:53:46 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*Libraries*/

# define _XOPEN_SOURCE 700 //sans ça, erreur de signaux

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <stdbool.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
# include <signal.h>
# include <sys/wait.h>
# include <fcntl.h>

/*Macros*/

# define COMMAND 0
# define OPTION 1
# define ARGUMENT 2 //argument (str or char inside single or double quotes)
# define INFILE 3 // < next commands based on infile's contents
# define OUTFILE 4 // > puts result inside the outfile
# define INFILE_DELIMITER 5 // << reads from infile until delimiter found
# define OUTFILE_APPEND 6 // >> redirects output in append mode
# define PIPE 7 //separator

# define KEY 0
# define VALUE 1
# define CMD 2
# define STR 3

# define HISTORY 0
# define FREE_HISTORY 1

/*Structures*/

/* Double linked list */
typedef struct s_element
{
	char	*content;
	int		type;
	struct s_element *prev;
	struct s_element *next;
}	t_element;

/* Environment */
typedef struct s_env
{
	char	*key;
	char	*value;
	struct s_env *next;
}	t_env;

/* To handles pipes */
typedef struct s_pipe
{
	pid_t	pid_child1;
	pid_t	pid_child2;
	int		pipe_end[2]; // int array of both ends of a pipe
	int		fd_infile;
	int		ft_outfile;
}	t_pipe;

/*-------------------MAIN FOLDER-------------------*/

/*Main*/

/*History*/
void		history(int option);

/*Checks*/
bool		check_commands_grammar(char *commands);

/*Signal*/
void		signal_handler(int signal);

/*Builtins*/
void	echo(char *line);
void    pwd();
void    cd();
char	*dollar(char *line, t_env *env_list);

/*------------------PARSING FOLDER------------------*/

/*Commands*/
char	*commands(char *line, t_env *env_list);
bool	is_this_command(char *buffer, char* command);
int		size_of_command(char *command, int len, int type);
bool    is_cmd_in_line(char *line, char *cmd);
int 	where_is_cmd_in_line(char *line, char *cmd);

/*Parsing*/
void	printlist_test(t_element   *head); //A EFFACER A LA FIN
int		determine_command_type(char *str, char *line, int i, int start);
t_element	*parsing(char *command);

/*Env_list*/
t_env   *put_env_in_list(char **env);
t_env   *find_value_with_key_env(t_env *env_list, char *key);
bool	is_key_in_env(t_env *env_list, char *key);

/*Lstnew*/
t_element	*lstnew(char *line, int i);
t_env		*lstnew_env(char *line, int i);

/*Free*/
void	final_free(char *line, t_env *env_list);
void	free_cmd_list(t_element *cmd_list);

/*-----------------EXECUTABLE FOLDER------------------*/
void	ft_redirect(t_element *cmd_list);
void	ft_print_path(t_env *env_list);