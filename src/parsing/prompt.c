/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 18:36:55 by octonaute         #+#    #+#             */
/*   Updated: 2023/10/20 18:06:07 by kquerel          ###   ########.fr       */
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
    t_env   *Gpath;
    char	*path;
    
	word = NULL;
	user = find_value_with_key_env(env_list, "USER");
    home = find_value_with_key_env(env_list, "HOME");
    Gpath = find_value_with_key_env(env_list, "PWD"); /*JE NE PEUX
    PAS ME BASER LA DESSUS CAR **ENV DANS LE MAIN NE CHANGE PAS UNE FOIS DANS LE PROGRAMME*/
	i = ft_strlen(pwd(NO_PRINT)) - 2; //pour sauter le dernier slash
	while(i > 0)
	{
		if (pwd(NO_PRINT)[i - 1] == '/')
			break;
		i--;
	}
	word = strlcpy_middle(word, pwd(NO_PRINT), i, ft_strlen(pwd(NO_PRINT)));
    //printf("last word from path = [%s]\n", word);
    //printf("home->value = [%s]\n", home->value);
    //printf("Gpath->value = [%s]\n",Gpath->value);
    
/* 	if (ft_strncmp(word, user->value, ft_strlen(user->value)) != 0 && ft_strncmp(pwd(NO_PRINT), home->value, ft_strlen(pwd(NO_PRINT)) - i + 1) != 0) //si le dernier mot du path n'est pas celui du user (donc si on n'est pas dans le dossier user)
    {
        printf("path != home && != user\n");
		path = home_path_simplified(pwd(NO_PRINT), env_list);
        printf("%s", path);
        free(path);
    } */
    
    if (ft_strncmp(word, user->value, ft_strlen(user->value)) == 0)
    {
        //printf("path = user\n");
        free(word);
        return ;
    }
    else if (ft_strncmp(word, "homes", ft_strlen(word) - i + 1) == 0)
    {
        //printf("path = home\n");
        //printf("Gpath->value = [%s]\n",Gpath->value);
		path = strlcpy_middle(path, Gpath->value, 1, ft_strlen(Gpath->value) - 1);
        printf("%s", path);
        free(path);
    }
    else if (ft_strncmp(word, user->value, ft_strlen(user->value)) != 0)
    {
        //printf("path != home && != user\n");
		path = home_path_simplified(pwd(NO_PRINT), env_list);
        printf("%s", path);
        free(path);
    }
/*     else
    {
        //printf("path = user\n");
        free(word);
        return ;
    } */


//----------------------------------------------------------------------------------------------
/*C'est ici que je dois gérer d'écrire "home" quand on cd .. après avoir cd ou de ne rien écrire (comme je veux)
Avant je le gérais dans le cd mais du coup je ne permettais pas de monter aussi haut que voulu*/

/*-----------------> je crois que ça ne marche pas parce que ça rentre dans le premier if tjrs car si on est 
dans le home alors la fin du path est home et non le nom du user >------------------------------*/
/*     if (ft_strncmp(pwd(NO_PRINT), home->value, ft_strlen(pwd(NO_PRINT)) - i + 1) == 0)
    {
        free(word);
        return ;
    }  */
//------------------------------------------------------------------------------------------------ */

    free(word);
}
