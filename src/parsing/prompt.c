/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karl <karl@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 18:36:55 by octonaute         #+#    #+#             */
/*   Updated: 2023/11/15 15:31:15 by karl             ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

void	home_path_simplified_loop(char *absolute_path, t_env *user, int *i, int *start, char **path_from_home)
{
	int		j;
	char	*temp;
	
	if(absolute_path[(*i) + 1] == '/')
	{
		temp = NULL;
		temp = strlcpy_middle(temp, absolute_path, (*start), (*i));
		(*start) = (*i) + 2;
		if (ft_strncmp(temp, user->value, ft_strlen(user->value)) == 0 && ft_strlen(user->value) == ft_strlen(temp))
		{
			j = 0;
			(*i)+=2;
			(*path_from_home) = malloc(sizeof(char) * (ft_strlen(absolute_path) - (*i) + 2));
			while (absolute_path[(*i)])
				(*path_from_home)[j++] = absolute_path[(*i)++];
			(*path_from_home)[j] = '\0';
		}
		free(temp);
	}
}

/*Deletes the /mnt/nfs/homes/casomarr/ part by comparing each word
in between slashes to the username (user->value). If the word between
slashes (stored in temp) == username, we store in path_from_home the
rest of the path. This will be used in the prompt function, since the
prompt only prints the path from home (unless we are located previous 
the home folder)*/
char	*home_path_simplified(char *absolute_path, t_env *env_list)
{
	char	*path_from_home;
	t_env	*user;
	int		i;
	int		start;
	char	*result;

	i = 0;
	start = 1;
	path_from_home = NULL;
	if (!is_key_in_env(env_list, "USER"))
		return (NULL);
	user = find_value_with_key_env(env_list, "USER");
	while(absolute_path[i])
	{
		home_path_simplified_loop(absolute_path, user, &i, &start, &path_from_home);
		if (path_from_home != NULL)
			return (path_from_home);
		i++;
	}
	return (NULL);
}

/*Starts at the length of pwd and goes backwards until a
slash is found : it represents the beggining of the
last word of the path. We start at ft_strlen(PWD)-2 to skip
the slash at the end of pwd.*/
int	get_beggining_of_last_word()
{
	int		i;
	char	*temp;

	temp = pwd(NO_PRINT);
	i = ft_strlen(temp) - 2;
	while(i > 0)
	{
		if (temp[i - 1] == '/')
			break;
		i--;
	}
	free(temp);
	return (i);
}

char	*ft_prompt(t_env *env_list, int option)
{
	char	*word;
	t_env	*user;
	t_env   *home;
	t_env   *Gpath;
	char	*path;
	char	*temp;

	if (!is_key_in_env(env_list, "USER") || !is_key_in_env(env_list, "HOME") || !is_key_in_env(env_list, "PWD")) // voir ce que ca rend sur bash a 42
		return ("\0");
	temp = pwd(NO_PRINT); // changement pour les free
	user = find_value_with_key_env(env_list, "USER");
	home = find_value_with_key_env(env_list, "HOME");
	Gpath = find_value_with_key_env(env_list, "PWD");
	word = strlcpy_middle(word, temp, get_beggining_of_last_word(), ft_strlen(temp) - 1); //sans le -1 ne change rien mais fait plus de sens!
	if (ft_strncmp(word, user->value, ft_strlen(user->value)) == 0) //si user juste $
		path = "";
	else if (ft_strncmp(word, "homes", ft_strlen(word) - get_beggining_of_last_word() + 1) == 0) //si home on print jusqu a home
		path = strlcpy_middle(path, Gpath->value, 1, ft_strlen(Gpath->value) - 1);
	else if (ft_strncmp(word, user->value, ft_strlen(user->value)) != 0 && \
	is_user_in_path(temp, env_list) == true) //si pas dans home ni dans user et que plus loin que user
		path = home_path_simplified(temp, env_list);
	else
		path = temp;
	if (option == PRINT)
		printf("%s", path);
	free(temp); 
	free(word);
	return (path); //il faudra le free
}
