/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 17:42:36 by carolina          #+#    #+#             */
/*   Updated: 2023/10/23 11:52:41 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

/*The first two "if" check if there are errors in the grammar of the command
line and print the associated error. The last function replaces the $ by its
associated value so that the executable receives directly the line completed.*/
char	*line_errors_and_fix(char *line, t_env *env_list)
{
	// KARL -> j'ai ajoute ca pour regler une seg fault
	if (!line)
		return (NULL);
	// fin
	(void)env_list;
	
	if (is_cmd_in_line(line, ">") == true || is_cmd_in_line(line, "<") == true)
		redirecters_error(line);
	else if (line[0] == '<' || line[0] == '>' || \
	line[0] == '/' || line[0] == '|' || line[0] == '&' || \
	line[0] == '\'' || line[0] == '\"')
		first_character_error(line);
	return (line);
}

/*Checks if what is written in the command line corresponds to a command.*/
bool	is_this_command(char *buffer, char* command)
{
	/*Je devrais gerer ici le citer dans la fonction du dessus cas pour 
	eviter que "heycdhola" return true car "cd" trouve.
	SOIT
	Gerer dans where_is_cmd_in_line ceci. En fonction de ou est ce que je le gere
	je pourrai enlever la securite (if i == 0) suite a l'appel de 
	where_is_cmd_in_line dans bcp de fonctions, ce qui me permettra de gagner 2 lignes
	(ex : dans echo). */
	size_t	i;

	i = 0;
	if (!buffer)
		return (false);
	while (buffer[i])
	{
		if (buffer[i] != command[i])
			return (false);
		i++;
	}
	if (ft_strlen(buffer) == ft_strlen(command) && i == ft_strlen(buffer))
		return (true);
	else
		return (false);
}

/*Returns the size of a cmd, key, value or str.*/
int	size_of_command(char *command, int len, int type)
{
	int	size;

	size = 0;
	if (type == CMD)
		size = cmd_type(command, len);
	else if (type == KEY || type == VALUE)
		size = key_and_value_type(command, len, type);
	else if (type == STR)
		size = str_type(command, len);
	return(size + 1); //car pour mallocs besoin d'un +1 pour \0
}

/*Checks if a given commands is in the command line.*/
bool	is_cmd_in_line(char *line, char *cmd)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (!cmd || !line)
		return (false);
	while (line[i] != '\0')
	{
		j = 0;
		while (line[i + j] == cmd[j] && cmd[j] != '\0' && line[i + j] != '\0')
		{
			j++;
			if (cmd[j] == '\0')
				return (true);
		}
		i++;
	}
	return (false);
}

/*Returns the position of the end of the cmd found in the cmd line
Returns 0 in case of error since the end of a cmd can't be at the
beggining of the cmd line.*/
int	where_is_cmd_in_line(char *line, char *cmd)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (!cmd || !line)
		return (false);
	while (line[i] != '\0')
	{
		j = 0;
		while (line[i + j] == cmd[j] && cmd[j] != '\0' && line[i + j] != '\0')
		{
			j++;
			if (cmd[j] == '\0')
				return (i + j); // i + j = fin de cmd
		}
		i++;
	}
	return (0);
}

void	commands(t_element *current_cmd, t_env *env_list)
{
	// a la base j avais fait " cd " etc donc avec espace apres le 
	// nom du builtin pour eviter les erreur du type "holacdhey" et 
	// que ca compte comme cd mais pb : cd tout seul sans espace avant
	// car premiere cmd de la ligne et sans espace apres car c est a 
	// moi de remplacer par le home_path alors ca ne marche pas. Même
	// pb pour le $ dans la fonction line_errors_and_fix car on peut
	// avoir $ suivi de '?'.
	if (ft_strncmp(current_cmd->content, "history", ft_strlen(current_cmd->content)) == 0 && ft_strlen(current_cmd->content) == ft_strlen("history"))
	{
	printf("hello1111\n");
		history(HISTORY);
	}
	else if (ft_strncmp(current_cmd->content, "pwd", ft_strlen(current_cmd->content)) == 0 && ft_strlen(current_cmd->content) == ft_strlen("pwd"))
		pwd(PRINT);
	else if (ft_strncmp(current_cmd->content, "cd", ft_strlen(current_cmd->content)) == 0 && ft_strlen(current_cmd->content) == ft_strlen("cd"))
		cd(current_cmd, env_list);
	else if (ft_strncmp(current_cmd->content, "echo", ft_strlen(current_cmd->content)) == 0 && ft_strlen(current_cmd->content) == ft_strlen("echo"))
		echo(current_cmd);
	else if (ft_strncmp(current_cmd->content, "env", ft_strlen(current_cmd->content)) == 0 && ft_strlen(current_cmd->content) == ft_strlen("env"))
		ft_env(env_list, current_cmd, 0);
	else if (ft_strncmp(current_cmd->content, "export", ft_strlen(current_cmd->content)) == 0 && ft_strlen(current_cmd->content) == ft_strlen("export"))
		ft_export(current_cmd, env_list);
	else if (ft_strncmp(current_cmd->content, "unset", ft_strlen(current_cmd->content)) == 0 && ft_strlen(current_cmd->content) == ft_strlen("unset"))
		ft_unset(current_cmd, env_list);
	else if (ft_strncmp(current_cmd->content, "exit", ft_strlen(current_cmd->content)) == 0 && ft_strlen(current_cmd->content) == ft_strlen("exit"))
	{
		// free
		ft_putendl_fd("exit", STDOUT_FILENO);
		exit (1);
	}
}
