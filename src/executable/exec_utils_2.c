/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: casomarr <casomarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 18:54:48 by kquerel           #+#    #+#             */
/*   Updated: 2023/11/28 19:40:32 by casomarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* Checks if only redirections are present in the linked list */
bool	ft_all_redir(t_element *cmd)
{
	t_element *head;
	
	head = cmd;
	while (cmd)
	{
		if (cmd->type < 3 && cmd->type != PIPE)
			return (false);
		cmd = cmd->next;
	}
	cmd = head;
	ft_only_create(cmd);
	return (true);
}

/* Only creates files in the case of all_redir */
bool	ft_only_create(t_element *cmd)
{
	int	fd;

	while (cmd)
	{
		if (cmd->type == OUTFILE || cmd->type == OUTFILE_APPEND)
		{
			if (cmd->type == OUTFILE)
				fd = open(cmd->content, O_CREAT | O_RDWR | O_TRUNC, 0644);
			else
				fd = open(cmd->content, O_CREAT | O_RDWR | O_APPEND, 0644);
			if (fd < 0)
				return (perror("bash"), false);
		}
		else if (cmd->type == INFILE)
		{
			fd = open (cmd->content, O_RDONLY, 0644);
			if (fd < 0)
			{
				ft_putstr_fd("bash: ", 2), ft_putstr_fd(cmd->content, 2);
				return (perror(" "), false);
			}
		}
		cmd = cmd->next, close (fd);
	}
	return (true);
}

/* Transforms a t_env list in an array of strings */
char **ft_transform_env(t_env *env)
{
	int	i;
	char **new;
	t_env *head;

	head = env;
	i = 0;
	while (head)
	{
		head = head->next;
		i++;
	}
	new = ft_calloc(i + 1, sizeof(char *));
	i = 0;
	while (env)
	{
		new[i] = ft_strjoin_env(env->key, env->value);
		env = env->next;
		i++;
	}
	new[i] = NULL;
	return (new);
}

/* Modified strjoin adding "=" between s1 and s2 */
char	*ft_strjoin_env(char const *s1, char const *s2)
{
	int		i;
	int		j;
	int		ft_strlen_total;
	char	*new_str;

	if (s1 == NULL || s2 == NULL)
		return (NULL);
	ft_strlen_total = ft_strlen(s1) + ft_strlen(s2) + 1 + 1;
	new_str = ft_calloc(ft_strlen_total, sizeof(char));
	if (new_str == NULL)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		new_str[i] = s1[i];
		i++;
	}
	new_str[i++] = '=';
	j = 0;
	while (s2[j])
		new_str[i++] = s2[j++];
	new_str[i] = '\0';
	return (new_str);
}

/* Joins every splitted string from the PATH= env variable with the command
	then tests its validity with access */
char	*ft_get_command(char **path, char *argument)
{
	char	*to_free;
	char	*to_return;
	int		i;

	i = 0;
	if (argument && path)
	{
		while (path[i])
		{
			to_free = ft_strjoin(path[i], "/");
			to_return = ft_strjoin(to_free, argument);
			free(to_free);
			if (access(to_return, F_OK) == 0)
				return (to_return);
			free(to_return);
			i++;
		}
	}
	return (NULL);
}
