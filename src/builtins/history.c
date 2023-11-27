/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: casomarr <casomarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 17:43:38 by carolina          #+#    #+#             */
/*   Updated: 2023/11/27 14:03:23 by casomarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*History : quand on fait cd et qu'on est deja tout en haut
on ne doit pas remettre cd dans history. Pareil qd on fait deux fois
de suite ls par exemple. Par contre quand on écrit une commande qui 
n'existe pas ça s'affiche dans history.

HISTORY_STATE and HISTORY_ENTRY are structures in history.h in
the readline library.

First if : if we ask to print more entries than there exist,
print all existing entries.*/
void	history(int option, int len)
{
	//Demander a Alban s'il considere que ce sont pas des fonctions interdites
	// history >2 du coup marche pas mais c est pas mes fonctions et history etait
	// pas a faire donc on s'en fout non?
	HISTORY_STATE	*info;
	HIST_ENTRY		**list;

	info = history_get_history_state();
	list = history_list();
	if (len > info->length)
		len = info->length;
	if (option == FT_HISTORY && len == -1)
		print_all_hist(info, list);
	else if (option == FT_HISTORY && len != -1)
		print_hist_until_len(info, list, len);
	else if (option == FREE_HISTORY)
		free_history(info, list);
}

void	print_all_hist(HISTORY_STATE *info, HIST_ENTRY **list)
{
	int	i;

	i = 0;
	while (i < info->length)
	{
		ft_putstr_fd(ft_itoa(i + 1), STDOUT_FILENO);
		ft_putstr_fd(" ", STDOUT_FILENO);
		ft_putendl_fd(list[i]->line, STDOUT_FILENO);
		i++;
	}
}

void	print_hist_until_len(HISTORY_STATE *info, HIST_ENTRY **list, int len)
{
	int	reverse;
	int	i;

	i = 0;
	reverse = info->length - len;
	while (i < len)
	{
		ft_putstr_fd(ft_itoa(reverse), STDOUT_FILENO);
		ft_putstr_fd(" ", STDOUT_FILENO);
		ft_putendl_fd(list[reverse]->line, STDOUT_FILENO);
		i++;
		reverse++;
	}
}

void	free_history(HISTORY_STATE *info, HIST_ENTRY **list)
{
	int	i;

	i = 0;
	while (i < info->length)
	{
		free_history_entry(list[i]);
		i++;
	}
	free (info);
	free (list);
}
