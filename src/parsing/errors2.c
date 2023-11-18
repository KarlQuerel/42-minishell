/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: casomarr <casomarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 15:40:58 by octonaute         #+#    #+#             */
/*   Updated: 2023/11/18 13:11:37 by casomarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

void	and_error(char *line)
{
	int	i;

	i = 0;
	while (line[i] != ' ' && line[i])
		i++;
	if (i == 1)
		printf("bash: syntax error near unexpected token `&'\n");
	else
		printf("bash: syntax error near unexpected token `&&'\n");
}

/* void	str_error(char *line)
{
	int		i;
	int		j;
	char	*str;
	char	type;

	if (line[0] == '\'')
		type = '\'';
	if (line[0] == '\"')
		type = '\"';
	i = 1;
	while (line[i] != type)
		i++;
	str = ft_calloc(i + 1, sizeof(char));
	i = 1;
	j = 0;
	while (line[i] != type)
		str[j++] = line[i++];
	str[j] = '\0';
	printf("%s : command not found\n", str);
	free(str);
} */

bool	pipe_double_or_eof(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] == '|' && line[i + 1] == '|')
		{
			ft_putendl_fd("bash: syntax error near \
            unexpected token `||'", STDERR_FILENO);
			return (false);
		}
		i++;
	}
	if (line[ft_strlen(line) - 1] == '|')
	{
		ft_putendl_fd("bash: syntax error : | at end of line", STDERR_FILENO);
		return (false);
	}
	return (true);
}
