/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 17:11:19 by carolina          #+#    #+#             */
/*   Updated: 2023/11/30 13:21:56 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*Libraries*/


#ifndef MINISHELL_H
# define MINISHELL_H

# define _XOPEN_SOURCE 700 //sans ça, erreur de signaux

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 5
# endif

#include "../libft/libft.h"

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <stdbool.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
# include <signal.h>
# include <sys/wait.h>
# include <sys/uio.h>
# include <fcntl.h>
# include <errno.h>
# include <limits.h>
#include <sys/ioctl.h>

/*Macros pour signaux*/
# define IN_PROMPT 0
# define IN_COMMAND 1
# define IN_HEREDOC 2
# define QUIT_HEREDOC 3
# define QUIT 4


/*Macros*/
# define COMMAND 0 // ls
# define OPTION 1 // -a
# define ARGUMENT 2 //argument (str or char inside single or double quotes)
# define INFILE 3 // < next commands based on infile's contents
# define OUTFILE 4 // > puts result inside the outfile
# define HEREDOC 5 // << reads from infile until delimiter found
# define OUTFILE_APPEND 6 // >> redirects output in append mode
# define PIPE 7 //separator

# define KEY 0
# define VALUE 1
# define CMD 2
# define STR 3

# define NONE 0 //no options
# define ENV 1
# define ECHO 2
# define HISTORY 3
# define CD 4

# define FT_HISTORY 0
# define FREE_HISTORY 1

# define PRINT 0
# define NO_PRINT 1

# define SPACES 0
# define QUOTES 1

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

#define PC "             ________________________________________________\n            /                                                \\\n           |    _________________________________________     |\n           |   |                                         |    |\n           |   |  Welcome                                |    |\n           |   |    to                                   |    |\n           |   |      MinisHELL                          |    |\n           |   |        by                               |    |\n           |   |          casomarr                       |    |\n           |   |            and                          |    |\n           |   |              kquerel                    |    |\n           |   |                                         |    |\n           |   |                                         |    |\n           |   |                                         |    |\n           |   |                                         |    |\n           |   |                                         |    |\n           |   |_________________________________________|    |\n           |                                                  |\n            \\_________________________________________________/\n                   \\___________________________________/\n                ___________________________________________\n             _-'    .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.  --- `-_\n          _-'.-.-. .---.-.-.-.-.-.-.-.-.-.-.-.-.-.-.--.  .-.-.`-_\n       _-'.-.-.-. .---.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-`__`. .-.-.-.`-_\n    _-'.-.-.-.-. .-----.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-----. .-.-.-.-.`-_\n _-'.-.-.-.-.-. .---.-. .-------------------------. .-.---. .---.-.-.-.`-_\n:-------------------------------------------------------------------------:\n`---._.-------------------------------------------------------------._.---'"


/*Structures*/
/* Global structure
--> exit_status gets the exit code of the last process
--> location gives info to signals about the program location (IN HEREDOC, IN PROMPT, IN COMMAND)
*/
// typedef struct	s_global
// {
// 	int	exit_status;
// 	int				location;
// }	t_global;

// extern t_global	g_signals;

extern int	g_location;

/* Command list
--> content represents the command
--> type is the command type
--> cmd_tab is an array of all commands (type 0)
*/

typedef struct s_element
{
	char	*content;
	char	*hd_filename;
	int		type;
	bool	builtin;
	bool	change;
	struct s_element *prev;
	struct s_element *next;
	struct s_pipe *exec;
	struct s_env *env;
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
	int		fd_heredoc;
	struct s_env *prev;
	struct s_env *next;
}	t_env;

/* To handles pipes */
typedef struct s_pipe
{
	int		fd_here_doc;
	int		last_pid;
	int		pipe_nb;
	pid_t		*pid;
	char	**cmd_tab;
	char	**cmd_path;
	int		std_in;
	int		std_out;
	int		fd_temp;
	int		fd[2];
	char 	**env;
	char	**line;
	char	**prompt;
	char	**env_execve;
	struct s_element *cmd;
	struct s_env *env_s;
}	t_pipe;

/*------------------PARSING FOLDER------------------*/

/*Checks*/
bool	quotes_can_close(char *line, int i);
bool	is_builtin(char *cmd_content);
bool	is_user_in_path(char *path, t_env *env_list);

/*Cmd_types*/
int		cmd_type(char *command, int len);
int		key_and_value_type(char *command, int len, int type);
int		str_type(char *command, int len);

/*Commands*/
bool	is_cmd(char *buffer, char* command);
int		size_of_command(char *command, int len, int type);
bool	is_cmd_in_line(char *line, char *cmd);
void	ft_builtins_history(t_element *cmd);
void	ft_builtins(t_element *cmd, t_env **env_list, t_pipe *exec);

/*Env_list*/
int		put_env_in_list_loop(char **env, t_env **current, int line);
t_env	*put_env_in_list(char **env);
t_env	*find_value_with_key_env(t_env *env_list, char *key);
bool	is_key_in_env(t_env *env_list, char *key);

/*Erase_spaces*/
void	erase_spaces_loop(char *line, char **new_line, int *i, int *j);
char	*erase_spaces(char *line);
char	*erase_empty_strings(char *line);

/*Errors*/
bool	line_errors_and_fix(char *line);
bool	first_character_error(char *line);
bool	redirecters_error(char *line);
void	pipe_error(char *line);

/*Errors2*/
void	and_error(char *line);
bool	pipe_double_or_eof(char *line);

/*Free*/
void	exit_free(t_element *cmd_list, t_env **env_list, t_pipe *exec);
void	free_cmd_tab(t_pipe *exec);
void	free_cmd_list(t_element *cmd_list);
void	free_env_list(t_env *env_list);
void	ctrld_free(char *line, char *prompt, t_env *env, t_pipe *exec);

/*Lstnew*/
t_element	*lstnew(char *line, int i, int type);
t_env		*lstnew_env(char *line, int i);

/*Malloc*/
int		count_spaces(char *line);
char	*erase_spaces_malloc(char *line);
char	*joinstr_minishell_malloc(char *line, int len, char *str, char type);
char	*empty_strings_malloc(char *line);

/*Determine_cmd*/
int 	determine_command_type(char *line, size_t end, size_t start);

/*Parsing*/
t_element 	*parsing(char *line, t_env *env_list);
void		type_arg_after_cmd(t_element **current);
void		parsing_fix(t_element **cmd_list, t_env *env_list);
void		builtin_fix(t_element **cmd_list);
bool	no_cmd_before(t_element *current);

/*Parsing2*/
t_element	*parsing_initialisation(char *line, int *i, int *start);
void		parsing_fill_content(t_element **current_cmd, char *line, int *i, \
			char *separator);
void		parsing_advance_to_next_word(char *line, int *start, int *i);
void		parsing_initialize_next(t_element **current_cmd, char *line, int *i);

/*Prompt*/
void	home_path_simplified_loop(char *absolute_path, t_env *user, int *i, \
char **path_from_home);
char	*home_path_simplified(char *absolute_path, t_env *env_list);
int		get_beggining_of_last_word(void);
char	*ft_prompt(t_env *env_list, int option);
void	ft_prompt2(char **prompt, char *word, t_env *env_list, char *path);

/*Signal*/
void	sigint_handler(int signal);
void	sigquit_handler(int signal);
int		set_signals(void);

/*Utils*/
char	*ft_joinstr_minishell(char *line, int len, char *str, char type);
char	*ft_join_pour_cd(char *line_begining, char *path);
char 	*strlcpy_middle(char *dst, const char *src, size_t start, size_t end);
char	*ft_strjoin_free(char const *s1, char *s2);

/*Utils2*/
char	*type_of_separator(char *line, int i, int str_type);
int		parsing_str_type(char *line, int i);
bool	ft_is_num(char *s);
bool	ft_atoi_check(char *str);

/*------------------BUILT-INS FOLDER------------------*/

/*Builtins_errors*/
bool	no_option(t_element *cmd, t_element *head);
bool	cd_option(t_element *cmd, t_element *head);
bool	env_option(t_element *cmd);
bool	history_option(t_element *cmd);
bool	echo_option(t_element *cmd);
bool	check_next(t_element *cmd, int option);

/*Cd*/
char	*fix_path_if_spaces(char *path);
void	cd_directory(char *path, t_env *env_list);
void	cd_home(t_env *env_list);
void	cd(t_element *current, t_env *env_list);

/*Cd2*/
size_t	size_of_word(char *path, int i);
int		forward_loop(char *current_path, char *home_value, int end);
void	go_forward_until_user(char *current_path, char *home_value);
void	go_backwards_until_user(char *current_path, char *home_value);

/*Dollar*/
char	*dollar(char *content, t_env *env_list);
void	ft_dollar_question_mark(t_env *env);

/*Echo*/
bool	no_further_args(t_element *cmd);
void	echo(t_element *current);

/*Env*/
void	ft_env(t_env *env, t_element *cmd, int option);

/*Exit*/
void	ft_exit(t_element *cmd, t_env **env, t_pipe *exec);
void	ft_num_arg(t_element *cmd, t_env **env, t_pipe *exec, t_element *head);
void	ft_too_many_arg(t_element *cmd, t_env **env);
void	exit_cont(t_element *cmd, t_env **env, t_pipe *exec, t_element *head);
int		add_exit_status_in_env(t_env **env, int n);

/*Exit_2*/
bool	no_args_or_options(t_element *cmd);
bool	no_pipes_before(t_element *cmd);
void	exit_check_all(t_element *cmd, t_env **env, t_pipe *exec);

/*Export*/
int		ft_export(t_element *cmd_list, t_env **env);
bool	ft_is_valid_key_var(char *s);
char 	**split_var(char *s);
void	join_new_var(t_env **env, char *key, char *value);
void	replace_var(t_env **env, char *key, char *value);
void	put_var_in_env(t_env **env, char* key, char *value);

/*History*/
void	history(int option, int len);
void	print_all_hist(HISTORY_STATE *info, HIST_ENTRY **list);
void	print_hist_until_len(HISTORY_STATE *info, HIST_ENTRY **list, int len);
void	free_history(HISTORY_STATE *info, HIST_ENTRY **list);

/*Pwd*/
char	*pwd(int option);
void	pwd_update_in_env(t_env **env_list);

/*Unset*/
int		ft_unset(t_element *cmd, t_env **env);
void	ft_delete_node_env(t_env **head, t_env *to_delete);
int		ft_delete_node_cmd(t_element **head, t_element *to_delete);

/*-----------------EXECUTABLE FOLDER ------------------*/

/*Exec*/
void	ft_execute(t_element *cmd, t_env **env, t_pipe *exec);
void	single_command(t_element *cmd, t_env **env, t_pipe *exec);
void	multiple_commands(t_element *cmd, t_env **env, t_pipe *exec);


void	middle_pipes(t_element *cmd, t_env **env, t_pipe *exec);
void	last_pipe(t_element *cmd, t_env **env, t_pipe *exec);

/*Exec_continued*/
void	middle_dup(t_element *cmd, t_env **env, t_pipe *exec);
void	last_dup(t_element *cmd, t_env **env, t_pipe *exec);
void	handle_command(t_element *cmd, t_env **env, t_pipe *exec);
int		exec_command(t_element *cmd, t_env *env, t_pipe *exec);
char	*ft_get_command(char **path, char *argument);

/*Exec_errors*/
void	msg_error(int err, char *s);
void	msg_error_bash(int err, char *s);

/*Exec_utils*/
char	**split_path(t_env *env_list);
void	fill_array(t_element *cmd, t_pipe *exec);
int		get_size_cmd(t_element *cmd);
int		ft_count_pipes(t_element *cmd);
char	*ft_strcpy(char *dst, char *src);

/*Redirect*/
int		ft_redirect(t_element *cmd, t_pipe *exec);
int		ft_infile(char *filename);
int		ft_outfile(t_element *cmd);


void	handle_sigint(int sig);
void	free_cmd_arr(t_pipe *exec);
void	ft_exit_continued(t_element *cmd, t_env **env, t_pipe *exec, t_element *head, int option);
char	*create_heredoc(char *safeword, int i, int *fd);
int	ft_is_builtin(t_element *cmd, t_env **env, t_pipe *exec, int option);
void	write_heredoc(char *safe_word, t_pipe *exec, int fd);



bool	ft_heredoc(t_element *cmd, t_env *env);
int	ft_open_hd(t_pipe *exec, int iteration_nb);
char	*ft_strjoin_env(char const *s1, char const *s2);
char **ft_transform_env(t_env *env);
bool	ft_all_redir(t_element *cmd);
int	ft_exec_slash(t_element *cmd, t_pipe *exec, t_env *env);
int	command_not_found(t_element *cmd, t_env *env, t_pipe *exec);
void	ft_exit_continued_2(t_element *cmd, t_env **env, t_pipe *exec, t_element *head);
void	ft_export_continued(t_element *cmd, t_env **env);
int	ft_strchr_int(char *s, char c);
void	free_child(t_element *cmd, t_env **env, t_pipe *exec);
int	ft_exit_status_single(t_env **env, int pid);
int	ft_exit_status_multiple(t_env **env, t_pipe *exec);




char	*ft_alban(t_element *cmd);
int	only_create_outfile_or_append(t_element *cmd, int fd);
void	ft_only_create(t_element *cmd);
void	ft_unlink(t_element *cmd);
void	ft_top_of_list(t_element *cmd);




void	ft_welcome(void);

t_pipe	*init_exec();

void	exit_check_all(t_element *cmd, t_env **env, t_pipe *exec);





#endif