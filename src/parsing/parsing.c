/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 17:45:28 by carolina          #+#    #+#             */
/*   Updated: 2023/11/20 11:12:22 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

/*Separates each argument in the command line in a t_element list.
Only the redirectors and spaces that separate each command are not
kept in the list. This list is then sent to the executor.
Separator is only freed qhen str_type == str because when
str_type == cmd it is not malloc'ed (in function type_of_separator).*/
t_element	*parsing(char *line, t_env *env_list)
{
	int			i;
	int			start;
	t_element	*current_cmd;
	t_element	*head;
	char		*separator;

	current_cmd = parsing_initialisation(line, &i, &start);
	head = current_cmd;
	while (line[i])
	{
		separator = type_of_separator(line, start, \
		parsing_str_type(line, start));
		parsing_fill_content(&current_cmd, line, &i, separator);
		if (parsing_str_type(line, start) == STR)
			free(separator);
		current_cmd->type = determine_command_type(line, i, start);
		parsing_advance_to_next_word(line, &start, &i);
		parsing_initialize_next(&current_cmd, line, &i);
	}
	parsing_fix(&head, env_list);
	builtin_fix(&head);
	return (head);
}

/*Determines the type of a given cmd for the parsing function.*/
int	determine_command_type(char *line, size_t end, size_t start)
{
	if(line[start] == '\'' || line[start] == '\"')
		start = start + 1;
	
	if ((line[start] == '-' && ft_isalpha(line[start + 1]) == 1) || \
	(line[end] >= 4 && line[start] == '-' && (line[start + 1] == '\'' || \
	line[start + 1] == '\"') && ft_isalpha(line[start + 2]) == 1 && \
	(line[end - 1] == '\'' || line[end - 1] == '\"')))
		return (OPTION);
	if (start >= 2 && line[start - 2] == '<' && line[start - 3] == '<')
		return (INFILE_DELIMITER);
	if (start >= 2 && line[start - 2] == '<')
		return (INFILE);
	if (start >= 2 && line[start - 2] == '>' && line[start - 3] == '>')
		return (OUTFILE_APPEND);
	if (start >= 2 && line[start - 2] == '>')
		return (OUTFILE);
	if (ft_strncmp(&line[start], "|", 1) == 0)
		return (PIPE);
	while(start < end)
	{
		if (line[start] == ' ')
			return (ARGUMENT);
		start++;
	}
	return (COMMAND);
}

void	type_arg_after_cmd(t_element *current)
{
	t_element	*temp;

	if (current->type == COMMAND && current->next)
	{
		temp = current->next;
		while (temp->type != PIPE && temp != NULL)
		{
			if (temp->type != OPTION && temp->type != INFILE && \
			temp->type != INFILE_DELIMITER && \
			temp->type != OUTFILE && temp->type != OUTFILE_APPEND)
				temp->type = ARGUMENT;
			if (temp->next != NULL)
				temp = temp->next;
			else
				break ;
		}
	}
}

/*J'AI MIS CETTE FONCTION EN COMMENTAIRE CAR JE NE ME SOUVIENS PAS DE POURQUOI
J'AVAIS LA PREMIÈRE IF ET QUE ÇA A L'AIR DE MARCHER PAREIL SANS*/
/* void	dollar_fix(t_element *current, t_env *env_list)
{
	t_element	*temp;

	// manque a gerer le cas "$?"
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
} */

/* To fix the type of the arguments that are not in between quotes
and are therefore considered as a COMMAND instead of an ARGUMENT
in the parsing function. This functions sets all arguments that are
not of type OPTION or redirecter after a cmd to ARGUMENT until a 
type PIPE is found.*/
void	parsing_fix(t_element **cmd_list, t_env *env_list)
{
	t_element	*current;

	current = (*cmd_list);
	if (current->next == NULL || current->next->type == PIPE)
		return ;
	while (current != NULL)
	{
		if (current->prev == NULL || (current->prev->type >= 3 && current->type < 3))
			current->type = COMMAND;
		if (current->type == COMMAND && current->next)
			type_arg_after_cmd(current);
		else if (current->content[0] == '$')
		{
			current->content = dollar(current->content, env_list);
			if (current->content == NULL)
			{
				ft_delete_node_cmd(cmd_list, current);
			}
		}
			// dollar_fix(current, env_list);
		current = current->next;
	}
	return ;
}

/*Sets all arguments encountered between a cmd that is a builtin and
a pipe to "builtin = true" so that the executor skips them.*/
void	builtin_fix(t_element **cmd_list)
{
	t_element	*current;

	current = (*cmd_list);
	while (current != NULL)
	{
		if (is_builtin(current->content) == true)
			current->builtin = true;
		current = current->next;
	}
	return ;
}
