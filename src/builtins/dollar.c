/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: casomarr <casomarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 12:42:47 by octonaute         #+#    #+#             */
/*   Updated: 2023/11/23 20:55:51 by casomarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"


// A GERER
/*

	echo $? est pareil que echo $USER, (les expands)
	bash:
	--> si $? utilise deux fois (ou plus) de suite -> exit code de 127

*/

/*
1. Checks if the $ is followed by alphanumeric
2. Puts what follows the $ into the variable key_to_find.
3. Cheks that the key exists in env_list (and key_in_env "takes its rank" if true).
4. Replaces the content by the key value.
*/
char	*dollar(char *content, t_env *env_list)
{
	/*Il manque à gérer l'option $?*/
	char	*key_to_find;
	t_env	*key_in_env;

	if (ft_strncmp(content, "$?", 2) == 0 && ft_strlen(content) == 2)
	{
		free(content);
		key_to_find = "EXIT_STATUS";
	}
	else
	{
		key_to_find = ft_calloc(ft_strlen(content), sizeof(char));
		key_to_find = strlcpy_middle(key_to_find, content, 1, ft_strlen(content));
	}
	if (is_key_in_env(env_list, key_to_find) == true)
	{
		key_in_env = find_value_with_key_env(env_list, key_to_find);
		// if (ft_strncmp(content, "EXIT_STATUS", ft_strlen(content)) != 0)
		// 	free(content);
		content = ft_calloc(ft_strlen(key_in_env->value), sizeof(char));
		content = strlcpy_middle(content, key_in_env->value, 0, ft_strlen(key_in_env->value));
	}
	if (ft_strncmp(key_to_find, "EXIT_STATUS", ft_strlen(key_to_find)) && \
	ft_strlen(key_to_find) == ft_strlen("EXIT_STATUS"))
		free(key_to_find);
	printf("content = [%s]\n", content);
	return (content);
}

void	ft_dollar_question_mark(void)
{
	char	*temp;

	temp = ft_itoa(g_signals.exit_status);
	// printf("---->Exit : %d\n", g_signals.exit_status);	
	ft_putstr_fd("bash: ", STDOUT_FILENO);
	ft_putstr_fd(temp, STDOUT_FILENO);
	ft_putendl_fd(": command not found", STDOUT_FILENO);
	free(temp);
	// FREE ITOA

	// 	si c'est $?, cas special
	// les signaux prennent 125 + l'int que rend le signal
	// CTRL + C = 5 + 125 = 130
	// 127 command not found (+2)
}
