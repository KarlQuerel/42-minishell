/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carolina <carolina@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 17:11:19 by carolina          #+#    #+#             */
/*   Updated: 2023/10/11 15:58:03 by carolina         ###   ########.fr       */
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
#include <sys/uio.h>
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

/* # define BEGINING 0
# define END 1
# define MIDDLE 2 */

# define YELLOW	"\033[33m"
# define GREEN	"\033[32m"
# define RESET	"\033[0m"

/* Colors */
# define BYEL "\e[1;33m"
# define BRED "\e[1;31m"
# define BWHT "\e[1;37m"
# define BCYN "\e[1;36m"
# define WHT "\e[0;37m"
# define BGRE "\033[1m\033[32m"

#define PC "             ________________________________________________\n            /                                                \\\n           |    _________________________________________     |\n           |   |                                         |    |\n           |   |  Welcome                                |    |\n           |   |    to                                   |    |\n           |   |      Minishell                          |    |\n           |   |        by                               |    |\n           |   |          casomarr                       |    |\n           |   |            and                          |    |\n           |   |              kquerel                    |    |\n           |   |                                         |    |\n           |   |                                         |    |\n           |   |                                         |    |\n           |   |                                         |    |\n           |   |                                         |    |\n           |   |_________________________________________|    |\n           |                                                  |\n            \\_________________________________________________/\n                   \\___________________________________/\n                ___________________________________________\n             _-'    .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.  --- `-_\n          _-'.-.-. .---.-.-.-.-.-.-.-.-.-.-.-.-.-.-.--.  .-.-.`-_\n       _-'.-.-.-. .---.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-`__`. .-.-.-.`-_\n    _-'.-.-.-.-. .-----.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-----. .-.-.-.-.`-_\n _-'.-.-.-.-.-. .---.-. .-------------------------. .-.---. .---.-.-.-.`-_\n:-------------------------------------------------------------------------:\n`---._.-------------------------------------------------------------._.---'"


/*Structures*/

/* Command list
--> content represents the command
--> type is the command type
--> cmd_tab is an array of all commands (type 0)
*/
typedef struct s_element
{
	char	*content;
	char	*cmd;
	int		type;
	bool	builtin;
	struct s_element *prev;
	struct s_element *next;
}	t_element;

/* Environment
--> key is a substring to find in value
--> value is the env as a whole
--> env is the env (for execve) // apparemment il me faut l'env de bash et non l'env actuel
*/
typedef struct s_env
{
	char	*key;
	char	*value;
	char	**env;
	struct s_env *next;
}	t_env;

/* To handles pipes
--> pid is the id process
--> pipe_end is an array of both ends of a pipe
--> av_nb is the arg number
-->
-->
*/
typedef struct s_pipe
{
	pid_t	pid;
	
	int		here_doc;
	int		pipe_nb;
	char	**cmd_tab;
	char	**cmd_path;
	int		*pipe_end;
	int		fd_infile;
	int		fd_outfile;
	int		av_nb;
}	t_pipe;

/*-------------------MAIN FOLDER-------------------*/

/*Main*/

/*History*/
void		history(int option);

/*Checks*/
bool		check_commands_grammar(char *commands);
bool		quotes_can_close(char *line);
bool		is_builtin(char *cmd_content);

/*Signal*/
void		signal_handler(int signal);

/*Builtins*/
void	echo(char *line);
void    pwd();
char	*cd(char *line, t_env *env_list, char *home_path);
char	*cd_home_path(char *line, char *home_path);
char	*dollar(char *line, t_env *env_list);

/*Errors*/
void	first_character_error(char *line);
void    redirecters_error(char *line);
void	slash_error(char *line);
void	pipe_error(char *line);
void	and_error(char *line);
void	str_error(char *line);

/*Utils*/
char	*ft_joinstr_minishell(char *line, int len, char *str, char type);
char	*ft_join_pour_cd(char *line_begining, char *path);
bool	only_spaces_after_cmd(char *line, size_t i);
char	*erase_spaces(char *line);
//char	*erase_spaces(char *line, int option, int start);

/*------------------PARSING FOLDER------------------*/

/*Commands*/
char	*commands(char *line, t_env *env_list, char *home_path);
bool	is_this_command(char *buffer, char* command);
int		size_of_command(char *command, int len, int type);
bool    is_cmd_in_line(char *line, char *cmd);
int 	where_is_cmd_in_line(char *line, char *cmd);

/*Parsing*/
void	printlist_test(t_element   *head); //A EFFACER A LA FIN
int		determine_command_type(char *str, char *line, size_t i, size_t start);
t_element	*parsing(char *command);
t_element	*parsing_fix(t_element *cmd_list);
t_element	*builtin_fix(t_element *cmd_list);

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

/* Exec*/
void	ft_redirect(t_element *cmd_list);
void	ft_execute(t_element *cmd, t_env *env);
char	**split_path(t_env *env_list);
void	ft_welcome(void);
void	ft_close_pipe(t_pipe *exec);
void	ft_create_pipe(t_pipe *exec);
char	*ft_strcpy(char *dst, char *src);


char	*ft_get_command(char **path, char *argument);
