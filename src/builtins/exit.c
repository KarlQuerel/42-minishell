/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: casomarr <casomarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 19:36:13 by kquerel           #+#    #+#             */
/*   Updated: 2023/11/24 14:19:01 by casomarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

/* exit:
si le premier argument de exit n'est pas valide:
- soit pas numerique
- soit superieur a INT_MAX 
meme si il y en a deux
"exit abc def" OR "exit 999999999999"
bash: exit: abc: numeric argument required
--> ON EXIT

si le premier arg marche( >= 0 && < INT_MAX) mais que le 2eme ne marche pas
"exit 57 df"
bash: exit: too many arguments
--> ON EXIT PAS

exit (int n)

return (n % 255)
*/

bool	no_args_or_options(t_element *cmd)
{
	while(cmd)
	{
		if (cmd->type == ARGUMENT || cmd->type == OPTION)
			return (false);
		cmd = cmd->next;
	}
	return (true);
}

bool	no_pipes_before(t_element *cmd)
{
	while(cmd)
	{
		if (cmd->type == PIPE)
			return (false);
		cmd = cmd->prev;
	}
	return (true);
}

/* Reproduces the exit function
If the first argument is not valid (not numeric or > INT_MAX) the programs ends
else if the second argument is not valid, the program keeps running */
int	ft_exit(t_element *cmd, t_env **env, t_pipe *exec)
{
	t_element	*head;
	int			exit_code;
	int	arg_count;
	
	if (no_args_or_options(cmd) == true)
	{
		if (no_pipes_before(cmd) == true)
				ft_putendl_fd("exit", STDERR_FILENO);
		exit_free(cmd, env, exec);
		exit(g_signals.exit_status);
	}
	arg_count = 0;
	head = cmd;
	cmd = cmd->next;
	while (cmd && cmd->type != PIPE)
	{
		if (cmd->type == ARGUMENT)
		{
			arg_count++;
			if (arg_count > 1)
				return (ft_exit_continued(cmd, env, exec, head, 1), 0);
			if (!ft_is_num(cmd->content) || \
		!ft_atoi_check(cmd->content))
				ft_exit_continued(cmd, env, exec, head, 0);
		}
		cmd = cmd->next;
	}
	if (arg_count > 1)
		return (ft_exit_continued(cmd, env, exec, head, 1), 0);
	cmd = head;
	while (cmd && cmd->type != PIPE)
	{
		if (cmd->type == ARGUMENT)
		{
			exit_code = ft_atoi(cmd->content);
			if (no_pipes_before(cmd) == true)
				ft_putendl_fd("exit", STDERR_FILENO);
			exit_free(head, env, exec);
			exit(exit_code % 256);
		}
		cmd = cmd->next;
	}
	return (0);
}

/* exit_continued */
void	ft_exit_continued(t_element *cmd, t_env **env, t_pipe *exec, \
t_element *head, int option)
{
		if (option == 0)
	{
		if (no_pipes_before(cmd) == true)
				ft_putendl_fd("exit", STDERR_FILENO);
		ft_putstr_fd("bash: ", STDERR_FILENO);
		ft_putstr_fd(cmd->content, STDERR_FILENO);
		ft_putendl_fd(": numeric argument required", STDERR_FILENO);
		exit_free(head, env, exec);
		g_signals.exit_status = 2;
		exit(g_signals.exit_status);
	}
	else
	{
		if (no_pipes_before(cmd) == true)
				ft_putendl_fd("exit", STDERR_FILENO);
		ft_putendl_fd("bash: exit: too many arguments", STDERR_FILENO);
		g_signals.exit_status = 127;
	}
}

void	exitstatus_update_in_env(t_env **env)
{
	t_element	*tmp;

	tmp = ft_calloc(1, sizeof(t_element));
	tmp->content = "export"; //pq ft_export commence par la commande
	tmp->next = ft_calloc(1, sizeof(t_element));
	tmp->next->content = ft_calloc(13, sizeof(char));
	tmp->next->content = ft_strjoin("EXIT_STATUS=", ft_itoa(g_signals.exit_status));
	tmp->next->type = ARGUMENT;
	//printf("exit status : [%d]\n", g_signals.exit_status); //%d et non %c
	//tmp->next->next->content = ft_itoa(g_signals.exit_status);
	//printf("content : [%s]\n",tmp->next->content);
	tmp->next->next = NULL;
	ft_export(tmp, env);
 }