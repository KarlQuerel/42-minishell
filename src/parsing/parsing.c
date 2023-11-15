/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karl <karl@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 17:45:28 by carolina          #+#    #+#             */
/*   Updated: 2023/11/15 11:52:05 by karl             ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

void printlist_test(t_element *head) // A EFFACER A LA FIN
{
	int	i = 0;

	while (head)
	{
		// if (i != 0)
		// 	printf("prev cmd = %s\n", head->prev->content);
		printf("--------------------------------\n");
		printf("content = ---%s---\n", head->content);
		printf("type = ---%d---\n", head->type);
		printf("builtin = ---%d---\n", head->builtin);
		printf("--------------------------------\n");
		// if (head->next != NULL)
		// 	printf("next cmd = %s\n", head->next->content);
		head = head->next;
		i++;
	}
}

/*Returns the delimiter to look for depending
on the type of string.*/
char	*type_of_separator(char *line, int i, int str_type)
{
    char	*type;
    
	if (str_type == STR)
	{
		type = calloc(2, sizeof(char));
		type[0] = line[i];
		type[1] = '\0';
	}
    else // if CMD (donc si rellement cmd ou si quotes can´t close)
	{
/*         type[0] = ' '; //on force l'espace au cas où quotes can´t close
		type[1] = '<';
		type[1] = '>';
		type[1] = '|';
		type[1] = '<'; */
		type = " |<>";
	}
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
	if (line[end + 1] && line[end + 2] && line[end + 1] == '<' && line[end + 2] == ' ')
		return (INFILE);
	if (line[end + 1] && line[end + 2] && line[end + 1] == '<' && line[end + 2] == '<')
		return (INFILE_DELIMITER);
	if (start >= 2 && line[start - 2] == '>' && line[start - 3] == '>')
		return (OUTFILE_APPEND);
	if (start >= 2 && line[start - 2] == '>'/* && line[start - 3] == ' '*/)
		return (OUTFILE);
	if (ft_strncmp(&line[start], "|", 1) == 0)
		return (PIPE);
	return (COMMAND);
}

/*Initializes the element_list by returning the first node.*/
t_element	*parsing_initialisation(char *line, int *i, int *start)
{
	int	typestr;
	
	(*i) = 0;
	while ((line[(*i)] == '<' || line[(*i)] == '>') && line[(*i)])
		(*i)++;
	if ((*i) != 0)
		(*i)++; //pour passer l'espace après le redirecteur
	(*start) = (*i);
	typestr = parsing_str_type(line, (*i));
	return(lstnew(line, (*start), typestr));
}

/*Returns the type of string to know if quotes have to be skipped*/
int	parsing_str_type(char *line, int i)
{
	if ((line[i] == '\'' || line[i] == '\"') && quotes_can_close(line) == true)
		return(STR);
	else
		return(CMD);
}

/*Advances the i and start variables until the beginning of the next word*/
void	parsing_advance_to_next_word(char *line, int *start, int *i)
{
	while ((line[(*i)] == ' ' || line[(*i)] == '<' || line[(*i)] == '>') && line[(*i)])
		(*i)++;
	if (line[(*i)] == ' ' || line[(*i)] == '<' || line[(*i)] == '>')
		(*start) = (*i) + 1;
	else
		(*start) = (*i);
}

void	parsing_fill_content(t_element **current_cmd, char *line, int *i, char *separator)
{
	int	j;
	int	x;

	j = 0;
	if (separator[0] == '\'' || separator[0] == '\"')
		(*i)++;
	while (line[(*i)])
	{
		x = 0;
		while(separator[x])
		{
			if (line[(*i)] == separator[x])
			{
				if (separator[x] == '|' && j == 0) //si pipe tout seul
					(*current_cmd)->content[j++] = line[(*i)++];
				(*current_cmd)->content[j] = '\0';
				if (separator == '\'' || separator == '\"')
					(*i)++;
				return ;
			}
			x++;
		}
		if (line[(*i)] == '\\') //pour le test echo hola\ncaro -> doit donner holancaro
			(*i)++;
		(*current_cmd)->content[j++] = line[(*i)++];
	}
	(*current_cmd)->content[j] = '\0';
	if (separator[0] == '\'' || separator[0] == '\"')
		(*i)++;
}

/*Sets next to NULL if line is over, otherwise initialises the next command.*/
void	parsing_initialize_next(t_element **current_cmd, char *line, int *i, int *start)
{
	if (line[(*i)] == '\0')
			(*current_cmd)->next = NULL;
	else
	{
		(*current_cmd)->next = lstnew(line, (*i), parsing_str_type(line, (*i)));
		(*current_cmd)->next->prev = (*current_cmd);
		(*current_cmd) = (*current_cmd)->next;
	}
}

/*Separates each argument in the command line in a t_element list.
Only the redirectors and spaces that separate each command are not
kept in the list. This list is then sent to the executor.*/
t_element *parsing(char *line, t_env *env_list)
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
		separator = type_of_separator(line, start, parsing_str_type(line, start));
		parsing_fill_content(&current_cmd, line, &i, separator);
		current_cmd->type = determine_command_type(line, i, start);
		parsing_advance_to_next_word(line, &start, &i);
		parsing_initialize_next(&current_cmd, line, &i, &start);
		// free(separator); //pas tjrs malloc'ed
	}
	parsing_fix(&head, env_list);
	builtin_fix(&head);
	return (head);
}

void	type_arg_after_cmd(t_element *current)
{
	t_element	*temp;
	
	if (current->type == COMMAND && current->next)
	{	
		//KARL pour proteger current->next
		//if (current->next)
		//FIN
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
				break;
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

/* To fix the type of the arguments that are not in between quotes and are
therefore considered as a COMMAND instead of an ARGUMENT in the parsing function.
This functions sets all arguments that are not of type OPTION or redirecter after 
a cmd to ARGUMENT until a type PIPE is found.*/
void	parsing_fix(t_element **cmd_list, t_env *env_list)
{
	t_element	*current;

	current = (*cmd_list);
	if (current->next == NULL || current->next->type == PIPE)
		return ;
	while(current != NULL)
	{
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
	while(current != NULL)
	{
		if (is_builtin(current->content) == true)
			current->builtin = true;
		current = current->next;
	}
	return ;
}
