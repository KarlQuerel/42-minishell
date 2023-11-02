/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: octonaute <octonaute@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 17:43:38 by carolina          #+#    #+#             */
/*   Updated: 2023/11/02 17:13:43 by octonaute        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

/*History : quand on fait cd et qu'on est deja tout en haut
on ne doit pas remettre cd dans history. Pareil qd on fait deux fois
de suite ls par exemple. Par contre quand on écrit une commande qui 
n'existe pas ça s'affiche dans history.*/
	
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


/*HISTORY QUE J'AVAI AVANT EN UTILISANT ADD_HISTORY POUR QUE LA FLÈCHE DU HAUT MARCHE. 
SI JE VEUX REPRENDRE LE CODE DU HAUT IL FAUT UTILISER LIBRAIRIE TERMIOS)
Il faudrait aussi changer la commande history dans l'exec du coup car on n'a plus entries*/

// void	history(int option)
// {
// 	int i;

// 	i = 0;
// 	/* get the state of your history list (offset, length, size) */
// 	HISTORY_STATE *myhist = history_get_history_state ();
// 	/* retrieve the history list */
// 	HIST_ENTRY **mylist = history_list ();
// 	if (option == HISTORY)
// 	{
// 		while(i < myhist->length)
// 		{
// 			printf(" %d  %s\n", i + 1, mylist[i]->line);
// 			//free_history_entry(mylist[i]); // FREE AU MOMENT DE EXIT
// 			i++;
// 		}
// 		putchar ('\n');	
// 	}
// 	else if (option == FREE_HISTORY)
// 	{
// 		while(i < myhist->length)
// 		{
// 			free_history_entry(mylist[i]);
// 			i++;
// 		}
// 		free (myhist);  /* free HIST_ENTRY list */ 
// 		free (mylist);  /* free HISTORY_STATE   */ 
// 	}
// }