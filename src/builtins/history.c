/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 17:43:38 by carolina          #+#    #+#             */
/*   Updated: 2023/11/17 14:40:34 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

/*History : quand on fait cd et qu'on est deja tout en haut
on ne doit pas remettre cd dans history. Pareil qd on fait deux fois
de suite ls par exemple. Par contre quand on écrit une commande qui 
n'existe pas ça s'affiche dans history.*/

void	history(int option, int len)
{
	int i;
	int reverse;

	i = 0;

	/* get the state of your history list (offset, length, size) */
	HISTORY_STATE *myhist = history_get_history_state ();
	/* retrieve the history list */
	HIST_ENTRY **mylist = history_list ();

	if (len > myhist->length) /*c'est moi qui l'ai décidé ainsi car sur bash on n epeut pas en donner
	trop, donc je dis de tout print dans ce cas ci*/
		len = myhist->length;
	
	if (option == FT_HISTORY && len == -1)
	{
		while(i < myhist->length)
		{
			ft_putstr_fd(ft_itoa(i + 1), STDOUT_FILENO);
			ft_putstr_fd(" ", STDOUT_FILENO);
			ft_putendl_fd(mylist[i]->line, STDOUT_FILENO);
			
			//printf(" %d  %s\n", i + 1, mylist[i]->line);
			//free_history_entry(mylist[i]); // FREE AU MOMENT DE EXIT
			i++;
		}
		// putchar ('\n');	
	}
	else if (option == FT_HISTORY && len != -1)
	{
		reverse = myhist->length - len;
		while(i < len)
		{
			ft_putstr_fd(ft_itoa(reverse), STDOUT_FILENO);
			ft_putstr_fd(" ", STDOUT_FILENO);
			ft_putendl_fd(mylist[reverse]->line, STDOUT_FILENO);
			
			// printf(" %d  %s\n", reverse, mylist[reverse]->line);
			//free_history_entry(mylist[i]); // FREE AU MOMENT DE EXIT
			i++;
			reverse++;
		}
		// putchar ('\n');	
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
