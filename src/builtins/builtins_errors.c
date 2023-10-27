/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_errors.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: casomarr <casomarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 15:39:41 by kquerel           #+#    #+#             */
/*   Updated: 2023/10/27 15:34:28 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

/* Checks if builtins take an option or an argument (for env)
--- if option 0, checks if env has a next node
--- if option 1, checks if the built has a next node AND its an option
*/
bool	check_next_node_builtin(t_element *cmd, int option)
{
	t_element *head;

	head = cmd;
	
	if (option == NO_OPTIONS)
	{
		while (cmd)
		{
			if (cmd->type == OPTION)
			{
				ft_putstr_fd(head->content, STDERR_FILENO);
				ft_putendl_fd(" cannot take options", STDERR_FILENO);
				return (false);
			}
			cmd = cmd->next;
		}
	}
	else if (option == ENV)
	{
		if (cmd->next)
		{
			ft_putendl_fd("env cannot take arguments nor options", STDERR_FILENO);
			return (false);
		}
	}
	else if (option == HISTORY)
	{
		if (cmd->next && cmd->next->type != PIPE && (!ft_is_num(cmd->next->content) || ft_atoi_check(cmd->next->content) == false)) //si history pas tout seul
		{
			printf("bash: history: %s numeric agument required\n", cmd->next->content); // ft_putsrfd et envoyer au pipe suivant
			return (false);
		}
		if (cmd->next && ft_is_num(cmd->next->content) && cmd->next->next && cmd->next->next->type != PIPE)
		{
			printf("bash: history: too many arguments\n"); // ft_putsrfd et envoyer au pipe suivant
			return (false);
		}
	}
	else /* if (option == ECHO) */
	{
		if (cmd->next && cmd->next->type == OPTION && \
		(ft_strncmp(cmd->next->content, "-n", 2) !=0 || ft_strlen(cmd->next->content) != 2))
		{	
			ft_putendl_fd("echo only accepts option -n", STDERR_FILENO);
			return (false);
		}
		if (cmd->next && cmd->next->type == OPTION && cmd->next->next && cmd->next->next->type != ARGUMENT)
		{
			ft_putstr_fd("", STDOUT_FILENO); // envoyez au pipe suivant
			return (false);
		}
	}
	return (true);
}

/*Checks if s is only numeric*/
bool	ft_is_num(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (!ft_isdigit(s[i]))
			return (false);
		i++;
	}
	return (true);
}

/*Checks number validity for history*/
bool ft_atoi_check(char *str)
{
	int nb;
	unsigned int i;

	i = 0;
	nb = 0;
	while (str[i] == ' ' || (str[i] >= '\t' && str[i] <= '\r'))
		i++;
	if (str[i] == '+')
	{
		if (str[i + 1] == '+')
			return (false);
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (nb > INT_MAX / 10 || (nb == INT_MAX / 10 && str[i] - '0' > INT_MAX % 10))
		{
				return (false);
		}
		nb = nb * 10 + str[i] - '0';
		i++;
	}
	return (true);
}
