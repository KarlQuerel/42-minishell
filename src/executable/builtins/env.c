/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karl <karl@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 18:21:09 by karl              #+#    #+#             */
/*   Updated: 2023/10/22 20:59:32 by karl             ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../../../includes/minishell.h"
#include "../../../libft/libft.h"

/* Reproduces the env command 
if option == 1, 
reproduces export behaviour when used without an argument
*/
void	ft_env(t_env *env, t_element *cmd, int option)
{
	int	i;

	/*
	dans le cas ou "env ls" doit executer ls
	je dois faire appel a l'exec de nouveau mais du coup changer ou mettre execve,
	ce qui complique le code
	a voir avec caro
	*/

	// if (cmd->next->content)
	// 	printf(" cmd->next->content == %s\n", cmd->next->content);

	//-> a tester demain avec Caro
	// if (cmd->next->content && cmd->next->type == COMMAND)
	// 	printf("gerer l'execution de %s\n", cmd->next->content); // execute_command(cmd, env, exec) // du coup rajouter t_pipe a commands.c
	// if (cmd->next && cmd->next->type == ARGUMENT)
	// {
	// 	ft_putstr_fd("env: ", STDERR_FILENO);
	// 	ft_putstr_fd("'", STDERR_FILENO);
	// 	ft_putstr_fd(cmd->next->content, STDERR_FILENO);
	// 	ft_putendl_fd("': No such file or directory", STDERR_FILENO);
	// }
	i = 0;
	while (env)
	{
		if (option == 1)
			ft_putstr_fd("export ", STDOUT_FILENO);
		ft_putstr_fd(env->key, STDOUT_FILENO);
		ft_putstr_fd("=", STDOUT_FILENO);
		ft_putendl_fd(env->value, STDOUT_FILENO);
		env = env->next;
	}
}
