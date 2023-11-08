/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karl <karl@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 18:25:43 by karl              #+#    #+#             */
/*   Updated: 2023/11/08 12:46:42 by karl             ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

// -->unset seg fault avec plusieurs arguments car tmp est vide
// et n'est pas accessible
// regarder unset abc=caro a 42 (valid identifier)

/* Reproduces the unset command */
int	ft_unset(t_element *cmd_list, t_env *env)
{
	t_env *tmp;
	
	while (cmd_list && cmd_list->next && cmd_list->next->type != PIPE)
	{
		tmp = env;
		if (!ft_is_valid_key_var(cmd_list->next->content) || ft_strchr(cmd_list->next->content, '='))
		{
			ft_putstr_fd("unset: ", STDOUT_FILENO);
			ft_putstr_fd(cmd_list->next->content, STDOUT_FILENO);
			ft_putendl_fd(" not a valid identifier", STDOUT_FILENO);
			return (0);
		}
		else
		{
			if (is_key_in_env(tmp, cmd_list->next->content) == true)
			{
				tmp = find_value_with_key_env(tmp, cmd_list->next->content);
				if (tmp)
					ft_delete_node(&env, tmp); // tmp cannot access memory sur gdb
			}
		}
		cmd_list = cmd_list->next;
	}
	return (1);
}

/* A EFFACTER A LA FIN */
void printenv(t_env *head)
{
	while (head)
	{
		printf("--------------------------------\n");
		printf("key = %s\n", head->key);
		printf("value = %s\n", head->value);
		printf("--------------------------------\n");
		head = head->next;
	}
}

/* Delete the node passed in parameters */
void	ft_delete_node(t_env **head, t_env *to_delete)
{   
    if (!to_delete)
        return;

    if (to_delete->prev)
    {
        to_delete->prev->next = to_delete->next;
    }
    else
    {
        *head = to_delete->next;
    }

    if (to_delete->next)
    {
        to_delete->next->prev = to_delete->prev;
    }

    free(to_delete->key);
    free(to_delete->value);
   	free(to_delete);
}


// {	
// 	t_env	*tmp;

// 	tmp = to_delete;
// 	if (!to_delete)
// 		return ;
// 	if (to_delete->next && to_delete->prev == NULL)
// 	{
// 		(*head) = to_delete->next;
// 		(*head)->prev = NULL;
// 	}
// 	else if (to_delete->next && to_delete->prev)
// 	{
// 		tmp = to_delete->next;
// 		tmp->prev = to_delete->prev;
// 		to_delete->prev->next = to_delete->next;
// 	}
// 	else if (to_delete->next == NULL && to_delete->prev)
// 	{
// 		tmp = to_delete->prev;
// 		tmp->next = NULL;
// 	}
// 	free(to_delete->key);
// 	to_delete->key = NULL;
// 	free(to_delete->value);
// 	to_delete->value = NULL;
// 	free(to_delete);
// 	to_delete = NULL;
// 	return ;
// }

