/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: octonaute <octonaute@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 18:36:55 by octonaute         #+#    #+#             */
/*   Updated: 2023/10/19 19:02:12 by octonaute        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

void    prompt(t_env *env_list)
{
    char    *word;
    int     i;
    t_env	*user;
    char	*path;
    
	user = find_value_with_key_env(env_list, "USER");
	word = NULL;
	i = ft_strlen(pwd(NO_PRINT)) - 2; //pour sauter le dernier slash
	while(i > 0)
	{
		if (pwd(NO_PRINT)[i - 1] == '/')
			break;
		i--;
	}
	word = strlcpy_middle(word, pwd(NO_PRINT), i, ft_strlen(pwd(NO_PRINT)));

    /*C'est ici que je dois gérer d'écrire "home" quand on cd .. après avoir cd ou de ne rien écrire (comme je veux)
    Avant je le gérais dans le cd mais du coup je ne permettais pas de monter aussi haut que ovulu*/
    
    /*if (ft_strncmp(word, "home", ft_strlen(word)) != 0) //si on est dans le home
    {
        printf("home");
    } */

	if (ft_strncmp(word, user->value, ft_strlen(user->value)) != 0) //si le dernier mot du path n'est pas celui du user (donc si on n'est pas dans le dossier user)
    {
		path = home_path_simplified(pwd(NO_PRINT), env_list);
        printf("%s", path);
        free(path);
    }
    free(word);
}
