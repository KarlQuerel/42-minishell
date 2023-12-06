/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: octonaute <octonaute@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 17:45:28 by carolina          #+#    #+#             */
/*   Updated: 2023/12/06 13:07:02 by octonaute        ###   ########.fr       */
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
	int			ret;
	t_element	*current_cmd;
	t_element	*head;

	current_cmd = parsing_initialisation(line, &i, &start);
	head = current_cmd;
	while (line[i])
	{
		ret = parsing_loop(line, &i, &start, &current_cmd);
		if (ret == 1)
		{
			if (ft_heredoc(current_cmd, env_list) == false)
				return (NULL);
		}
		parsing_advance_to_next_word(line, &start, &i);
		parsing_initialize_next(&current_cmd, line, &i);
	}
	if (parsing_fix(&head, env_list) == 1)
		return (NULL);
	builtin_fix(&head);
	return (head);
}

int	parsing_loop(char *line, int *i, int *start, t_element **current_cmd)
{
	// char		*separator;

/* 	separator = type_of_separator(line, (*start), \
	parsing_str_type(line, (*start))); */
	// separator = " |<>";
/* 	if (parsing_str_type(line, (*start)) == STR)
	free(separator); */
	fill_content_loop(current_cmd, line, i/* , separator */);
	//printf("current->content = %s\n", (*current_cmd)->content);
/* 	if ((*current_cmd)->content[0] == '\'') //faudra modifier ça : singel quote dans content ou signel quote dans content[0]?
	{
		printf("no change : %s\n", (*current_cmd)->content);
		(*current_cmd)->change = false;
	} */
	(*current_cmd)->type = determine_command_type(line, (*i), (*start));
	if ((*current_cmd)->type == HEREDOC)
		return (1);
	return (0);
}

/*Sets all arguments encountered between a cmd that is a builtin and
a pipe to "builtin = true" so that the executor skips them.*/
void	builtin_fix(t_element **cmd_list)
{
	t_element	*current;

	current = (*cmd_list);
	while (current)
	{
		if (is_builtin(current->content) == true)
			current->builtin = true;
		current = current->next;
	}
}
