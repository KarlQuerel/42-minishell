/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 13:55:33 by casomarr          #+#    #+#             */
/*   Updated: 2023/10/23 13:45:15 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

/* Reproduces the export command */
int	ft_export(t_element *cmd_list, t_env *env)
{
	char	**new_var;
	
	if (cmd_list->next == NULL) // "export sans rien"
		ft_env(env, cmd_list, 1);
	printf("cmd_list->content %s\n", cmd_list->next->content);
	while (cmd_list && cmd_list->next)
	{
		if (!ft_is_valid(cmd_list->next->content)) // si on envoie un mauvais argument a export
		{
			ft_putstr_fd("export: ", STDOUT_FILENO);
			ft_putstr_fd(cmd_list->next->content, STDOUT_FILENO);
			ft_putendl_fd(": not a valid identifier", STDOUT_FILENO);
			return (0);
		}
		else if (ft_strchr(cmd_list->next->content, '=')) // si on trouve un = dans la string
		{
			new_var = new_env_var(cmd_list->next->content);
			put_new_var(env, new_var[0], new_var[1]);
		}
		cmd_list = cmd_list->next;
	}
	return (1);
}

/* Checks if the argument is a valid name for a variable*/
int	ft_is_valid(char *s)
{
	int	i;

	i = 0;
	if (!ft_isalpha(s[i]) && s[i] != '_')
		return (0);
	while (s[i] && s[i] != '=')
	{
		if (!ft_isalnum(s[i]) && s[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

/*
	a regarder sur bash --posix a 42 lundi
	--> example : variable=hello
	
	variable =hello -> ne doit pas marcher
	varable = hello -> ne doit pas marcher
	variable= hello -> donne "variable="
*/
/* Returns an array of strings with the new variable */
char	**new_env_var(char *s)
{
	char **ret;
	char *position_equal;

	ret = malloc(sizeof(ret) * 3); // 3 car ca sera toujours 3 string, variable, hello et la string ide
	if (!ret)
	{
		ft_putendl_fd("malloc failed", STDERR_FILENO);
		return (NULL);
	}
	position_equal = ft_strchr(s, '='); // on prend la position du egal
	ret[0] = ft_substr(s, 0, position_equal - s); // on ecrit le premier argument avant le egal "variable"
	ret[1] = ft_substr(position_equal, 1, ft_strlen(position_equal)); // on passe le egal et on ecrit le mot apres "hello"
	ret[3] = NULL;
	return (ret);
}

/* Puts the new variable in the environment */
int	put_new_var(t_env *env, char *key, char *content)
{
	char	*to_free;

	if (content == NULL)
		content = ""; // pour quand meme ecrire le new env (variable=)
	to_free = ft_strjoin("=", content); // on join = avec hello (=hello)
	if (!to_free)
	{
		ft_putendl_fd("Split failed", STDOUT_FILENO);
		return (0);
	}
	// t_env	*env;
	// env = malloc(sizeof(t_env)); // a free apres
	// if (!env)
	// {
	// 	ft_putendl_fd("Malloc failed", STDOUT_FILENO);
	// 	return (0);
	// }
	env = find_value_with_key_env(env, key); // si variable existe deja dans env //seg fault ici
	if (env == NULL)
	{
		free(env);
		return (0);
	}
	env->key = ft_strjoin(key, to_free); // on join variable avec =hello
	free(to_free);
	return (1);
}


