/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_errors.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: casomarr <casomarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 15:39:41 by kquerel           #+#    #+#             */
/*   Updated: 2023/11/17 12:13:11 by casomarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

bool	no_option(t_element **cmd, t_element *head)
{
	while ((*cmd) && (*cmd)->type != PIPE)
	{
		if ((*cmd)->type == OPTION)
		{
			ft_putstr_fd(head->content, STDERR_FILENO);
			ft_putendl_fd(" cannot take options", STDERR_FILENO);
			return (false);
		}
		(*cmd) = (*cmd)->next;
	}
	return (true);
}

bool	env_option(t_element **cmd)
{
	if ((*cmd)->next && (*cmd)->next->type != PIPE)
	{
		ft_putendl_fd("env cannot take arguments nor options", \
		STDERR_FILENO);
		return (false);
	}
	return (true);
}

bool	history_option(t_element **cmd)
{
	if ((*cmd)->next && (*cmd)->next->type != PIPE && \
	(!ft_is_num((*cmd)->next->content) || \
	ft_atoi_check((*cmd)->next->content) == false)) //si history pas tout seul
	{
		printf("bash: history: %s numeric agument required\n", \
		(*cmd)->next->content); // ft_putsrfd et envoyer au pipe suivant
		return (false);
	}
	if ((*cmd)->next && ft_is_num((*cmd)->next->content) && (*cmd)->next->next && \
	(*cmd)->next->next->type != PIPE)
	{
		printf("bash: history: too many arguments\n"); // ft_putsrfd et envoyer au pipe suivant
		return (false);
	}
	return (true);
}

bool	echo_option(t_element **cmd)
{
	if ((*cmd)->next && (*cmd)->next->type == OPTION && \
	(ft_strncmp((*cmd)->next->content, "-n", 2) != 0 || \
	ft_strlen((*cmd)->next->content) != 2))
	{	
		ft_putendl_fd("echo only accepts option -n", STDERR_FILENO);
		return (false);
	}
	if (((*cmd)->next && (*cmd)->next->type == OPTION && (((*cmd)->next->next && \
	(*cmd)->next->next->type != ARGUMENT) || (*cmd)->next->next == NULL)))
	{
		ft_putstr_fd("", STDOUT_FILENO); // envoyez au pipe suivant
		return (false);
	}
	return (true);
}

/* Checks if builtins take an option or an argument (for env)
The variable "cmd" sent to this function is the node following
the builtin command.
--- if option 0, checks if env has a next node
--- if option 1, checks if the built has a next node AND its an option
*/
bool	check_next(t_element *cmd, int option)
{
	t_element	*head;

	head = cmd;
	if (option == NONE)
		return (no_option(&cmd, head));
	else if (option == ENV)
		return (env_option(&cmd));
	else if (option == HISTORY)
		return (history_option(&cmd));
	else
		return (echo_option(&cmd));
}
