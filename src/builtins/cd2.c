/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd2.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: casomarr <casomarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 11:47:20 by casomarr          #+#    #+#             */
/*   Updated: 2023/11/17 17:01:10 by casomarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

size_t	size_of_word(char *path, int i)
{
	if (path[i] == '/')
		i += 1;
	while (path[i] != '/' && path[i])
		i++;
	return (i + 1);
}

void	go_forward_until_user(char *current_path, char *home_value)
{
	//on retrouve ou est current_path compare a home_path
	// on fait cd() de chaque word entre slashes jusqu a ce que 
	// current_path = home_value

	// marche bien sauf qd je fais cd en etant tout en haut (sur /)
	
	int 	end;
	int		start;
	char	*word;

	end = 0;
	while(current_path[end] == home_value[end])
		end++;
	if (ft_strncmp(current_path, "/", ft_strlen(current_path)) != 0)
		end+=1;
	start = end;
	word = ft_calloc(size_of_word(home_value, start) + 1, sizeof(char));
	while(is_cmd(current_path, home_value) == false)
	{
		while(home_value[end] != '/' && home_value[end])
			end++;
		free(word);
		word = strlcpy_middle(word, home_value, start, end - 1);
		if (chdir(word) != 0)
		{
			free(word);
			free(current_path);
			return ;
		}
		if (home_value[end + 1] == '\0')
			break;
		end+=1;
		start = end;
		free(word);
		word = ft_calloc(size_of_word(home_value, start) + 1, sizeof(char));
	}
	free(word);
	free(current_path);
}

void	go_backwards_until_user(char *current_path, char *home_value)
{
	while (is_cmd(current_path, home_value) == false)
	{
		// -----> cd qd plus haut que user devrait nous faire avancer jusqu'à après le user!!!!
		if (chdir("..") != 0)
		{
			free(current_path);
			//printf errno
			return ;
		}
		free(current_path);
		current_path = pwd(NO_PRINT);
	}
	free(current_path);
}
