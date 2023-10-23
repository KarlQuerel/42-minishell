/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 17:43:38 by carolina          #+#    #+#             */
/*   Updated: 2023/10/23 11:50:29 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

void	history(int option)
{
	/*History : quand on fait cd et qu'on est deja tout en haut
	on ne doit pas remettre cd dans history. Pareil qd on fait deux fois
	de suite ls par exemple. Par contre quand on écrit une commande qui 
	n'existe pas ça s'affiche dans history.*/

	/* 
		history ls donne :
		error bash: history: ls: numeric argument required

	 */

	int i;
	printf("hello\n");
	i = 0;
	/* get the state of your history list (offset, length, size) */
	HISTORY_STATE *myhist = history_get_history_state ();
	/* retrieve the history list */
	HIST_ENTRY **mylist = history_list ();
	if (option == HISTORY)
	{
		while(i < myhist->length)
		{
			printf(" %d  %s\n", i + 1, mylist[i]->line);
			//free_history_entry(mylist[i]); // FREE AU MOMENT DE EXIT
			i++;
		}
		putchar ('\n');	
	}
	else if (option == FREE_HISTORY)
	{
		while(i < myhist->length)
		{
			free_history_entry(mylist[i]);
			i++;
		}
		free (myhist);  /* free HIST_ENTRY list */ 
		free (mylist);  /* free HISTORY_STATE   */ 
	}
}
