/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 12:10:49 by casomarr          #+#    #+#             */
/*   Updated: 2023/12/07 13:49:05 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*Checks if s is only numeric*/
bool	ft_is_num(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (!ft_isdigit(s[i]))
			return (false);
		i++;
	}
	return (true);
}

/*Checks number validity for history*/
bool	ft_atoi_check(char *str)
{
	int				nb;
	unsigned int	i;

	i = 0;
	nb = 0;
	while (str[i] == ' ' || (str[i] >= '\t' && str[i] <= '\r'))
		i++;
	if (str[i] == '+')
	{
		if (str[i + 1] == '+')
			return (false);
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (nb > INT_MAX / 10 || (nb == INT_MAX / 10 \
			&& str[i] - '0' > INT_MAX % 10))
			return (false);
		nb = nb * 10 + str[i] - '0';
		i++;
	}
	return (true);
}

bool	compare(char *s1, char *s2)
{
	size_t	i;

	i = 0;
	if (!s1 || !s2)
		return (false);
	while ((s1[i] || s2[i]))
	{
		if (s1[i] != s2[i])
			return (false);
		i++;
	}
	if (ft_strlen(s1) == ft_strlen(s2))
		return (true);
	return (false);
}
