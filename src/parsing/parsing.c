/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 17:45:28 by carolina          #+#    #+#             */
/*   Updated: 2023/10/20 14:28:01 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

void printlist_test(t_element *head) // A EFFACER A LA FIN
{
	int i = 0;

	while (head)
	{
		// if (i != 0)
		// 	printf("prev cmd = %s\n", head->prev->content);
		printf("content = %s\n", head->content);
		printf("type = %d\n", head->type);
		printf("builtin = %d\n", head->builtin);
		// if (head->next != NULL)
		// 	printf("next cmd = %s\n", head->next->content);
		head = head->next;
		i++;
	}
}

/*Returns the delimiter to look for depending
on the type of string.*/
char	type_of_separator(char *line, int i)
{
    char	type;
    
    if (line[i] == '\'')
        type = '\'';
    else if (line[i] == '\"')
        type = '\"';
    else
        type = ' ';
    return (type);
}

/*Determines the type of a given cmd for the parsing function.*/
int determine_command_type(char *line, size_t end, size_t start)
{
	if ((line[start] == '-' && ft_isalpha(line[start + 1]) == 1) ||
		(line[end] >= 4 && line[start] == '-' && (line[start + 1] == '\'' || line[start + 1] == '\"') && ft_isalpha(line[start + 2]) == 1 &&
		 (line[end - 1] == '\'' || line[end - 1] == '\"')))
		return (OPTION);
	if ((line[start] == '\'' || line[start] == '\"') &&
			 (line[end - 1] == '\'' || line[end - 1] == '\"'))
		return (ARGUMENT);
	if (end - start > end + 2)
	{
		if (line[end + 1] == '<' && line[end + 2] == ' ')
			return (INFILE);
		else if (line[end + 1] == '<' && line[end + 2] == '<')
			return (INFILE_DELIMITER);
	}
	if (start > 1)
	{
		if (line[start - 2] == '>' && line[start - 3] == ' ')
			return (OUTFILE);
		else if (line[start - 2] == '>' && line[start - 3] == '>')
			return (OUTFILE_APPEND);
	}
	if (ft_strncmp(&line[start], "|", 1) == 0)
		return (PIPE);
	return (COMMAND);
}

/*Separates each argument in the command line in a t_element list.
Only the redirectors and spaces that separate each command are not
kept in the list. This list is then sent to the executor.*/
t_element *parsing(char *line, t_env *env_list)
{
	int i;
	int start;
	int j;
	t_element *current_cmd;
	t_element *head;
	int	type;
	char quote_type;

	i = 0;
	start = i;
	current_cmd = NULL;
	current_cmd = lstnew(line, start, CMD); //je pars du principe que tjrs cmd d abord
	head = current_cmd;
	
	// KARL -> j'ai ajoute ca pour regler une seg fault
	if (!line)
		return (NULL);
	// fin

	while (line[i])
	{
		j = 0;
		if ((line[start] == '\'' || line[start] == '\"') && quotes_can_close(line) == true)
		{
			type = STR;
			i++;
		}
		else
			type = CMD;
		quote_type = type_of_separator(line, start);
		while (line[i] && line[i] != quote_type)
		{
			if (line[i] == '\\') //pour le test echo hola\ncaro -> doit donner holancaro
    		    i++;
			current_cmd->content[j++] = line[i++];
		}
		current_cmd->content[j] = '\0';
		if (quote_type != ' ')
			i++;
		current_cmd->type = determine_command_type(line, i, start);
		while ((line[i] == ' ' || line[i] == '<' || line[i] == '>') && line[i])
			i++;
		if (line[i] == ' ')
			start = i + 1;
		else
			start = i;
		if (line[i] == '\0')
			current_cmd->next = NULL;
		else
		{
			current_cmd->next = lstnew(line, i, type);
			current_cmd->next->prev = current_cmd;
			current_cmd = current_cmd->next;
		}
	}
	head = parsing_fix(head, env_list);
	head = builtin_fix(head);
	return (head);
}

/* To fix the type of the arguments that are not in between quotes and are
therefore considered as a COMMAND instead of an ARGUMENT in the parsing function.
This functions sets all arguments that are not of type OPTION after a cmd
"echo" or "cd" to ARGUMENT until a type PIPE is found.*/
t_element	*parsing_fix(t_element *current, t_env *env_list)
{
	t_element	*head;
	t_element	*temp;

	head = current;
	if (current->next == NULL || current->next->type == PIPE)
		return (head);
	while(current != NULL)
	{
		if (strncmp(current->content, "echo", ft_strlen(current->content)) == 0 || strncmp(current->content, "cd", ft_strlen("cd")) == 0)
		{
			temp = current->next;
			while (temp->type != PIPE && temp->next != NULL)
			{
				if (temp->type != OPTION)
					temp->type = ARGUMENT;
				temp = temp->next;
			}
		}
		else if (current->content[0] == '$')
		{
			if (ft_isalpha(current->content[1]) == 0)
			{
				temp = current->prev;
				while (temp->type != COMMAND && temp != NULL)
					temp = temp->prev;
				if (strncmp(temp->content, "echo", ft_strlen(temp->content)) == 0)
				{
					free(current->content);
					current->content = ""; //et non \n car deja un \n a la fin de la fonction echo
				}
			}
			else
				current->content = dollar(current->content, env_list);
		}
		current = current->next;
	}
	return (head);
}

/*Sets all arguments encountered between a cmd that is a builtin and
a pipe to "builtin = true" so that the executor skips them.*/
t_element	*builtin_fix(t_element *cmd_list)
{
	t_element	*head;

	head = cmd_list;
	if (cmd_list->next == NULL || cmd_list->next->type == PIPE) //??
	{
		if (is_builtin(cmd_list->content) == true)
			cmd_list->builtin = true;
		return (head);
	}
	while(cmd_list->next != NULL)
	{
		if (is_builtin(cmd_list->content) == true)
		{
			while (cmd_list->type != PIPE && cmd_list->next != NULL)
			{
				cmd_list->builtin = true;
				cmd_list = cmd_list->next;
			}
		}
		else
			cmd_list = cmd_list->next;
	}
	if (cmd_list->prev->builtin == true && cmd_list->type != PIPE)
		cmd_list->builtin = true;
	return (head);
}
