/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: casomarr <casomarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 17:45:28 by carolina          #+#    #+#             */
/*   Updated: 2023/09/30 17:20:47 by casomarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

void printlist_test(t_element *head) // A EFFACER A LA FIN
{
	int i = 0;

	while (head)
	{
		if (i != 0)
			printf("prev cmd = %s\n", head->prev->content);
		printf("content = %s\n", head->content);
		printf("type = %d\n", head->type);
		if (head->next != NULL)
			printf("next cmd = %s\n", head->next->content);
		head = head->next;
		i++;
	}
}

int determine_command_type(char *str, char *line, int end, int start)
{
	printf("str[0] = [%c]\n", str[0]);
	//  printf("str[1] = %c\n", str[1]);
	// printf("str[2] = %c\n", str[2]);
	// printf("str[ft_strlen(str) - 1] = %c\n", str[ft_strlen(str) - 1]);
	printf("length str = %d\n", ft_strlen(str));

	if ((str[0] == '-' && ft_isalpha(str[1]) == 1) ||
		(str[ft_strlen(str)] >= 4 && str[0] == '-' && (str[1] == '\'' || str[1] == '\"') && ft_isalpha(str[2]) == 1 &&
		 (str[ft_strlen(str) - 1] == '\'' || str[ft_strlen(str) - 1] == '\"')))
		return (OPTION);
	else if ((str[0] == '\'' || str[0] == '\"') &&
			 (str[ft_strlen(str) - 1] == '\'' || str[ft_strlen(str) - 1] == '\"'))
		return (ARGUMENT);
	else if (ft_strlen(line) > end + 2)
	{
		if (line[end + 1] == '<' && line[end + 2] == ' ')
			return (INFILE);
		else if (line[end + 1] == '<' && line[end + 2] == '<')
			return (INFILE_DELIMITER);
	}
	else if (start > 1)
	{
		if (line[start - 2] == '>' && line[start - 3] == ' ')
			return (OUTFILE);
		else if (line[start - 2] == '>' && line[start - 3] == '>')
			return (OUTFILE_APPEND);
	}
	else if (ft_strlen(str) == 1 && str[0] == '|') // ne marche pas qd pas au debut de la ligne
		return (PIPE);
	return (COMMAND);
}

t_element *parsing(char *line)
{
	int i;
	int start;
	int j;
	t_element *current_cmd;
	t_element *head; // pour printlist test

	i = 0;
	start = i;
	j = 0;
	current_cmd = NULL;
	current_cmd = lstnew(line, i);
	head = current_cmd; // pour printlist test
	while (line[i])
	{
		if (line[i] == ' ' && i != 0)
		{
			current_cmd->content[j] = '\0';
			current_cmd->type = determine_command_type(current_cmd->content, line, i, start);
			printf("content = %s\n", current_cmd->content);
			printf("type = %d\n", current_cmd->type);
			current_cmd->next = lstnew(line, i);
			current_cmd->next->prev = current_cmd; // TEST ICI
			current_cmd = current_cmd->next;
			j = 0;
			while (line[i] == ' ' || line[i] == '<' || line[i] == '>')
				i++;
			start = i;
		}
		current_cmd->content[j++] = line[i++];
	}
	current_cmd->content[j] = '\0';
	current_cmd->type = determine_command_type(current_cmd->content, line, i, start);
	printf("content = %s\n", current_cmd->content);
	printf("type = %d\n", current_cmd->type);
	current_cmd->next = NULL;

	// printlist_test(head); //pour printlist test
	return (head);
}
