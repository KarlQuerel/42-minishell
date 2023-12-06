/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd2.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karl <karl@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 11:47:20 by casomarr          #+#    #+#             */
/*   Updated: 2023/12/06 19:19:19 by karl             ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../../includes/minishell.h"

//SUPPRIMER LE .C ET DANS LE .H DEMAIN
// size_t	size_of_word(char *path, int i)
// {
// 	if (path[i] == '/')
// 		i += 1;
// 	while (path[i] != '/' && path[i])
// 		i++;
// 	return (i + 1);
// }

// int	forward_loop(char *current_path, char *home_value, int end)
// {
// 	int		start;
// 	char	*word;

// 	start = end;
// 	while (is_cmd(current_path, home_value) == false)
// 	{
// 		while (home_value[end] != '/' && home_value[end])
// 			end++;
// 		word = strlcpy_middle(word, home_value, start, end - 1);
// 		if (chdir(word) != 0)
// 		{
// 			free(word);
// 			free(current_path);
// 			return (1);
// 		}
// 		if (home_value[end + 1] == '\0')
// 		{
// 			free(word);
// 			break ;
// 		}
// 		end += 1;
// 		start = end;
// 		free(word);
// 	}
// 	return (0);
// }

// /*Advances by doing cd(word) (word being what is found in between
// each slashes of the path) until current_path = home_value.*/
// void	go_forward_until_user(char *current_path, char *home_value)
// {
// 	int		end;
// 	int		ret;

// 	end = 0;
// 	while (current_path[end] == home_value[end])
// 		end++;
// 	if (compare(current_path, "/") == false)
// 		end += 1;
// 	ret = forward_loop(current_path, home_value, end);
// 	if (ret == 0)
// 		free(current_path);
// }

// void	go_backwards_until_user(char *current_path, char *home_value)
// {
// 	while (is_cmd(current_path, home_value) == false)
// 	{
// 		if (chdir("..") != 0)
// 		{
// 			free(current_path);
// 			perror("bash: ");
// 			return ;
// 		}
// 		free(current_path);
// 		current_path = pwd(NO_PRINT);
// 	}
// 	free(current_path);
// }
