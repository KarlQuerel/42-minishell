/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: octonaute <octonaute@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 18:36:55 by octonaute         #+#    #+#             */
/*   Updated: 2023/10/19 19:37:58 by octonaute        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

void    prompt(t_env *env_list)
{
    char    *word;
    int     i;
    t_env	*user;
    t_env   *home;
    char	*path;
    
	word = NULL;
	user = find_value_with_key_env(env_list, "USER");
    home = find_value_with_key_env(env_list, "HOME");
	i = ft_strlen(pwd(NO_PRINT)) - 2; //pour sauter le dernier slash
	while(i > 0)
	{
		if (pwd(NO_PRINT)[i - 1] == '/')
			break;
		i--;
	}
	word = strlcpy_middle(word, pwd(NO_PRINT), i, ft_strlen(pwd(NO_PRINT)));
	if (ft_strncmp(word, user->value, ft_strlen(user->value)) != 0) //si le dernier mot du path n'est pas celui du user (donc si on n'est pas dans le dossier user)
    {
		path = home_path_simplified(pwd(NO_PRINT), env_list);
        printf("%s", path);
        free(path);
    }

//----------------------------------------------------------------------------------------------
/*C'est ici que je dois gérer d'écrire "home" quand on cd .. après avoir cd ou de ne rien écrire (comme je veux)
Avant je le gérais dans le cd mais du coup je ne permettais pas de monter aussi haut que voulu*/

/*-----------------> je crois que ça ne marche pas parce que ça rentre dans le premier if tjrs car si on est 
dans le home alors la fin du path est home et non le nom du user >------------------------------*/
    if (ft_strncmp(pwd(NO_PRINT), home->value, ft_strlen(pwd(NO_PRINT)) - i + 1) == 0)
    {
        free(word);
        return ;
    } 
//------------------------------------------------------------------------------------------------

    free(word);
}
