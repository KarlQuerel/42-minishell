/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: casomarr <casomarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 17:43:38 by carolina          #+#    #+#             */
/*   Updated: 2023/10/20 20:19:34 by casomarr         ###   ########.fr       */
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
	int i;

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
		free (myhist);  //free HIST_ENTRY list
		free (mylist);  //free HISTORY_STATE 
	}
}


//FONCTIONS POUR EVITER FONCTIONS INTERDITES : IL FAUT QUE JE FASSE EN SORTE DE LES INTEGRER A MON CODE CI-DESSUS
/*

// -------------------------------------------DANS MON .H
	typedef struct history_entry {
		int timestamp;
		char *command;
		struct history_entry *next;
	} HIST_ENTRY;

	typedef struct history_state {
		int size;
		HIST_ENTRY *head;
	} HISTORY_STATE;
// -------------------------------------------

	void add_to_history(HISTORY_STATE *state, char *command, int timestamp) {
		HIST_ENTRY *new_entry = (HIST_ENTRY *) malloc(sizeof(HIST_ENTRY));
		new_entry->timestamp = timestamp;
		new_entry->command = (char *) malloc(strlen(command) + 1);
		strcpy(new_entry->command, command);
		new_entry->next = state->head;
		state->head = new_entry;
		state->size++;
	}

	void remove_history_entry(HISTORY_STATE *state, HIST_ENTRY *entry) {
		HIST_ENTRY *temp = state->head;
		HIST_ENTRY *prev = NULL;

		while (temp != NULL) {
			if (temp == entry) {
				if (prev == NULL) {
					state->head = temp->next;
				} else {
					prev->next = temp->next;
				}
				free(temp->command);
				free(temp);
				state->size--;
				return;
			}
			prev = temp;
			temp = temp->next;
		}
	}

	void free_history_entry(HIST_ENTRY *entry) {
		if (entry == NULL) {
			return;
		}
		free(entry->command);
		free(entry);
	}

	void free_history_state(HISTORY_STATE *state) {
		if (state == NULL) {
			return;
		}
		HIST_ENTRY *temp = state->head;
		HIST_ENTRY *next;

		while (temp != NULL) {
			next = temp->next;
			free_history_entry(temp);
			temp = next;
		}
		free(state);
	}
*/