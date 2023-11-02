/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: octonaute <octonaute@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 18:36:55 by octonaute         #+#    #+#             */
/*   Updated: 2023/11/02 13:46:29 by octonaute        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

char	*prompt(t_env *env_list, int option)
{
	char    *word;
	int     i;
	t_env	*user;
	t_env   *home;
	t_env   *Gpath;
	char	*path;
	
	path = NULL;
	word = NULL;
	user = find_value_with_key_env(env_list, "USER");
	home = find_value_with_key_env(env_list, "HOME");
	Gpath = find_value_with_key_env(env_list, "PWD");
	
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
	
	if (ft_strncmp(word, user->value, ft_strlen(user->value)) == 0) //si user juste $
	{
		free(word);
		/* printf("$"); //vérifier
		return ("$"); //vérifier */
		// path = "$";
		return NULL;
	}
	else if (ft_strncmp(word, "homes", ft_strlen(word) - i + 1) == 0) //si home on print jusqu a home
	{
		path = strlcpy_middle(path, Gpath->value, 1, ft_strlen(Gpath->value) - 1);
		//printf("%s", path);
		// free(path);
	}
/*     else if (ft_strncmp(word, "", ft_strlen(word)) == 0) //NE MARCHE PAS
	{
		printf("/");
		// free(path);
	} */
	else if (ft_strncmp(word, user->value, ft_strlen(user->value)) != 0 && is_user_in_path(pwd(NO_PRINT), env_list) == true) //si pas dans home ni dans user et que plus loin que user
	{
		path = home_path_simplified(pwd(NO_PRINT), env_list);
		//printf("%s", path);
		// free(path);
	}
	else
	{
		path = pwd(NO_PRINT);
		// printf("%s", path);
		// free(path);
	}
	
	if (option == PRINT)
		printf("%s", path);
	free(word);
	return (path);
}
