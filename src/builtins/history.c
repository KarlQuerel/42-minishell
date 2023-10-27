/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 17:43:38 by carolina          #+#    #+#             */
/*   Updated: 2023/10/27 15:34:19 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

	/*History : quand on fait cd et qu'on est deja tout en haut
	on ne doit pas remettre cd dans history. Pareil qd on fait deux fois
	de suite ls par exemple. Par contre quand on écrit une commande qui 
	n'existe pas ça s'affiche dans history.*/

	/* 
		history ls donne :
		error bash: history: ls: numeric argument required
	 */
	
int	ft_lstsize_history(t_history *lst)
{
	int		len;
	t_history	*content;

	len = 0;
	content = lst;
	while (content != NULL)
	{
		content = content -> next;
		len++;
	}
	return (len);
}

t_history	*ft_lstlast_history(t_history *lst)
{
	if (lst == NULL)
		return (NULL);
	while (lst->next)
	{
		lst = lst->next;
	}
	return (lst);
}

void	lstadd_back_history(t_history *entries, char *line)
{
	t_history	*temp;

	temp = ft_lstlast_history(entries);
	temp->next = lstnew_history(line, ft_lstsize_history(entries) + 1);
}


t_history	*ft_add_history(t_history *entries, char *line)
{
	t_history	*head;

	if (entries == NULL)
	{
		entries = lstnew_history(line, 1);
		return (entries);
	}
	head = entries;
	lstadd_back_history(entries, line);
	return (head);
}

void	history(t_history *current_entry, int len)
{
	int	i;

	i = 0;
	if (len >= 0)
	{
		while(current_entry != NULL && i < len)
		{
			printf("%d %s\n", current_entry->nb, current_entry->cmd);
			current_entry = current_entry->next;
			i++;
		}
	}
	else
	{
		while(current_entry != NULL)
		{
			printf("%d %s\n", current_entry->nb, current_entry->cmd);
			current_entry = current_entry->next;
			i++;
		}
	}
}

void	free_history(t_history *current_entry)
{
	while(current_entry != NULL)
	{
		free(current_entry->cmd);
		current_entry = current_entry->next;
	}
	free(current_entry);
}