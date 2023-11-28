/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: casomarr <casomarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 17:45:28 by carolina          #+#    #+#             */
/*   Updated: 2023/11/28 13:17:42 by casomarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
		if (separator[0] == '\'')
			current_cmd->change=false;
		parsing_fill_content(&current_cmd, line, &i, separator);
		if (parsing_str_type(line, start) == STR)
			free(separator);
		current_cmd->type = determine_command_type(line, i, start);
		if (current_cmd->type == HEREDOC)
		{
			if (ft_heredoc(current_cmd, env_list) == false)
				return (NULL);
		}
		parsing_advance_to_next_word(line, &start, &i);
		parsing_initialize_next(&current_cmd, line, &i);
	}
	parsing_fix(&head, env_list);
	builtin_fix(&head);
	return (head);
}

/* bool	nothing_before(int start, int n, char *line)
{
	if (start - (n + 1) > -1)
	{
		if (line[start - (n + 1)] != ' ')
			return false;
	}
	return (true);
} */

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
	if ((start >= 3 && (line[start - 1] == ' ' && line[start - 2] == '<' && line[start - 3] == '<'/*  && line[start - 4] == ' ' */)/*  && nothing_before(start, 3, line) == true */) || \
	(start >= 2 && (line[start - 1] == '<' && line[start - 2] == '<'/*  && line[start - 3] == ' ' */)/*  && nothing_before(start, 4, line) == true */) || \
	(start >= 3 && ((line[start - 1] == '\"' || line[start - 1] == '\'') && line[start - 2] == '<' && line[start - 3] == '<'/*  && line[start - 4] == ' ' */)/*  && nothing_before(start, 3, line) == true */) || \
	(start >= 4 && ((line[start - 1] == '\"' || line[start - 1] == '\'') && line[start - 2] == ' ' && line[start - 3] == '<' && line[start - 4] == '<'/*  && line[start - 5] == ' ' */)/*  && nothing_before(start, 4, line) == true */))
		return (HEREDOC);
	if ((start >= 2 && (line[start - 1] == ' ' && line[start - 2] == '<'/*  && line[start - 3] == ' ' */)/*  && nothing_before(start, 2, line) == true */) || \
	(start >= 1 && line[start - 1] == '<'/*  && line[start - 2] == ' ' *//*  && nothing_before(start, 1, line) == true */) || \
	(start >= 2 && ((line[start - 1] == '\"' || line[start - 1] == '\'') && line[start - 2] == '<'/*  && line[start - 3] == ' ' */)/*  && nothing_before(start, 2, line) == true */) || \
	(start >= 3 && ((line[start - 1] == '\"' || line[start - 1] == '\'') && line[start - 2] == ' ' && line[start - 3] == '<'/*  && line[start - 4] == ' ' */)/*  && nothing_before(start, 3, line) == true */))
		return (INFILE);
	if ((start >= 3 && (line[start - 1] == ' ' && line[start - 2] == '>' && line[start - 3] == '>'/*  && line[start - 4] == ' ' */)/*  && nothing_before(start, 3, line) == true */) || \
	(start >= 2 && (line[start - 1] == '>' && line[start - 2] == '>'/*  && line[start - 3] == ' ' */)/*  && nothing_before(start, 2, line) == true */) || \
	(start >= 3 && ((line[start - 1] == '\"' || line[start - 1] == '\'') && line[start - 2] == '>' && line[start - 3] == '>'/*  && line[start - 4] == ' ' */)/*  && nothing_before(start, 3, line) == true */) || \
	(start >= 4 && ((line[start - 1] == '\"' || line[start - 1] == '\'') && line[start - 2] == ' ' && line[start - 3] == '>' && line[start - 4] == '>'/*  && line[start - 5] == ' ' */)/*  && nothing_before(start, 4, line) == true */))
		return (OUTFILE_APPEND);
	if ((start >= 2 && (line[start - 1] == ' ' && line[start - 2] == '>'/*  && line[start - 3] == ' ' */)/*  && nothing_before(start, 2, line) == true */) || \
	(start >= 1 && line[start - 1] == '>'/*  && line[start - 1] == ' ' *//*  && nothing_before(start, 1, line) == true */) || \
	(start >= 2 && ((line[start - 1] == '\"' || line[start - 1] == '\'') && line[start - 2] == '>'/*  && line[start - 3] == ' ' */)/*  && nothing_before(start, 2, line) == true */) || \
	(start >= 3 && ((line[start - 1] == '\"' || line[start - 1] == '\'') && line[start - 2] == ' ' && line[start - 3] == '>'/*  && line[start - 4] == ' ' */)/*  && nothing_before(start, 3, line) == true */))
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

void	type_arg_after_cmd(t_element **current)
{
	t_element	*temp;

	if ((*current)->type == COMMAND && (*current)->next)
	{
		temp = (*current)->next;
		while (temp->type != PIPE && temp != NULL)
		{
			if (temp->type != OPTION && temp->type < 3)
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
		if ((current->prev != NULL && current->prev->type >= 3 && current->type < 3 \
		&& no_cmd_before(current) == true) || (current->prev == NULL && current->type < 3))
			current->type = COMMAND;
		if (current->type == COMMAND && current->next)
			type_arg_after_cmd(&current);
		else if (current->content[0] == '$' && current->change == true)
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
		// if (is_builtin(current->content) == true)
		// {
		// 	while(current->prev && current->prev->type != PIPE)
		// 	{
		// 		current->builtin = true;
		// 		current = current->prev;
		// 	}
		// 	if (current->next->builtin == true && current->type != PIPE)
		// 		current->builtin = true;
		// 	while(current->next && current->next->type != PIPE)
		// 	{
		// 		current->builtin = true;
		// 		current = current->next;
		// 	}
		// 	if (current->prev->builtin == true && current->type != PIPE)
		// 		current->builtin = true;
		// }
		// current = current->next;
  
    // KARL --> JE NE SAIS PAS QUELLE VERSION EST LA BONNE DU COUP J'AI LAISSE LES DEUX
  
		/* if (is_builtin(current->content) == true)
		{
			while(current->prev && current->prev->type != PIPE)
			{
				current->builtin = true;
				current = current->prev;
			}
			if (current->next->builtin == true && current->type != PIPE)
				current->builtin = true;
			while(current->next && current->next->type != PIPE)
			{
				current->builtin = true;
				current = current->next;
			}
			if (current->prev->builtin == true && current->type != PIPE)
				current->builtin = true;
		}
		current = current->next; */

		if (is_builtin(current->content) == true)
			current->builtin = true;
		current = current->next;
	}
	return ;
}

bool	no_cmd_before(t_element *current)
{
	while (current->prev && current->prev->type != PIPE)
	{
		if (current->prev->type == COMMAND)
			return (false);
		current = current->prev;
	}
	return (true);
}

//void	put_all_to_type_builtin(t_element **cmd)

//ctrl Z?