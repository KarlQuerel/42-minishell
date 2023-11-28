/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: casomarr <casomarr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 17:02:19 by kquerel           #+#    #+#             */
/*   Updated: 2023/11/28 18:31:43 by casomarr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* Restore I/O's in case of builtins */
int	ft_is_builtin(t_element *cmd, t_env **env, t_pipe *exec)
{
	if (cmd && cmd->builtin == true && cmd->content)
	{
		exec->std_in = dup(STDIN_FILENO);
		exec->std_out = dup(STDOUT_FILENO);
		if (!ft_redirect(cmd, exec))
		{
			//free ?
			return (0);
		}
		ft_builtins(cmd, env, exec);
		dup2(exec->std_in, STDIN_FILENO);
		dup2(exec->std_out, STDOUT_FILENO);
		close(exec->std_in);
		close(exec->std_out);
		return (0);
	}
	return (1);
}

/* Being on the middle pipe(s), both fds's are sent to dup2 */
void	middle_dup(t_element *cmd, t_env **env, t_pipe *exec)
{
	if (dup2(exec->fd_temp, STDIN_FILENO) < 0)
	{
		perror("dup2");
		exit(0);
	}
	close(exec->fd_temp);
	if (dup2(exec->fd[1], STDOUT_FILENO) < 0)
	{
		perror("dup2");
		exit(0);
	}
	close(exec->fd[1]);
	handle_command(cmd, env, exec);
}

/* Being on the middle pipe(s), only stdin fd is sent to dup2 */
void last_dup(t_element *cmd, t_env **env, t_pipe *exec)
{
	if (dup2(exec->fd_temp, STDIN_FILENO) < 0)
		perror("dup last");
	close(exec->fd_temp);
	handle_command(cmd, env, exec);
}

/* Redirects command based on its input
--- if a redirection is detected, ft_redirect is called
--- if a builtin is detected, ft_builtins is called
---	if the cmd is not empty, exec_command is called
*/
void	handle_command(t_element *cmd, t_env **env, t_pipe *exec)
{
	t_env	*exit_status;
	int	exit_nb;

	exit_nb = 0;
	if (!ft_redirect(cmd, exec))
	{
		// free
		exit(1);
	}
	exit_status = find_value_with_key_env(*env, "EXIT_STATUS");
	if (cmd->builtin == true)
		return (ft_builtins(cmd, env, exec), close(exec->fd[0]), exit(ft_atoi(exit_status->value)));
	if (exec->cmd_tab[0] != NULL)
		exit_nb = add_exit_status_in_env(env, exec_command(cmd, *env, exec));
	exit(exit_nb);
}
