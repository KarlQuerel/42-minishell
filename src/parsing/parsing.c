/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: casomarr <casomarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 17:45:28 by carolina          #+#    #+#             */
/*   Updated: 2023/10/17 15:03:20 by casomarr         ###   ########.fr       */
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

/* int determine_command_type(char *str, char *line, size_t end, size_t start)
{
	if ((str[0] == '-' && ft_isalpha(str[1]) == 1) ||
		(str[ft_strlen(str)] >= 4 && str[0] == '-' && (str[1] == '\'' || str[1] == '\"') && ft_isalpha(str[2]) == 1 &&
		 (str[ft_strlen(str) - 1] == '\'' || str[ft_strlen(str) - 1] == '\"')))
		return (OPTION);
	if ((str[0] == '\'' || str[0] == '\"') &&
			 (str[ft_strlen(str) - 1] == '\'' || str[ft_strlen(str) - 1] == '\"'))
		return (ARGUMENT);
	if (ft_strlen(line) > end + 2)
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
	if (ft_strncmp(str, "|", 1) == 0)
		return (PIPE);
	return (COMMAND);
} */

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

t_element *parsing(char *line)
{
	int i;
	int start;
	int j;
	t_element *current_cmd;
	t_element *head;
	bool	inside_quotes;
	int	type;

	i = 0;
	start = i;
	current_cmd = NULL;
	inside_quotes = false;
	current_cmd = lstnew(line, start, CMD); //je pars du principe que tjrs cmd d abord
	head = current_cmd;
	while (line[i])
	{
		j = 0;
		if ((line[start] == '\'' || line[start] == '\"') && quotes_can_close(line) == true)
		{
			type = STR;
			while (line[i] && (line[i] != '\'' || line[i] != '\"')) //verifier sans le quotes_can_close pq je pense que j ai deja cette protection ailleurs
				current_cmd->content[j++] = line[i++];
			current_cmd->content[j] = '\0';
		}
		else
		{
			type = CMD;
			while (line[i] && line[i] != ' ')
				current_cmd->content[j++] = line[i++];
			current_cmd->content[j] = '\0';
		}
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
	head = parsing_fix(head);
	head = builtin_fix(head);
	return (head);
}
/*Pour les commandes type echo qui sont suivies d'arguments qui ne sont pas
forcements entre guillements et donc compris dans la fonction parsing comme 
des commandes.*/
t_element	*parsing_fix(t_element *current)
{
	t_element	*head;

	head = current;
	if (current->next == NULL || current->next->type == PIPE)
		return (head);
	while(current->next != NULL)
	{
		if (strncmp(current->content, "echo", ft_strlen("echo")) == 0 || strncmp(current->content, "cd", ft_strlen("cd")) == 0)
		{
			current = current->next;
			while (current->type != PIPE && current->next != NULL)
			{
				if (current->type != OPTION)
					current->type = ARGUMENT;
				current = current->next;
			}
		}
		if (current->next != NULL)
			current = current->next;
	}
	if (current->type != OPTION && current->type != PIPE)
		current->type = ARGUMENT;
	return (head);
}

t_element	*builtin_fix(t_element *cmd_list)
{
	t_element	*head;

	head = cmd_list;
	if (cmd_list->next == NULL || cmd_list->next->type == PIPE) //??
		return (head);
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