/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kquerel <kquerel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 17:02:19 by kquerel           #+#    #+#             */
/*   Updated: 2023/12/02 17:36:21 by kquerel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* Restore I/O's in case of builtins */
int	ft_is_builtin(t_element *cmd, t_env **env, t_pipe *exec, int option)
{
	if (cmd && cmd->builtin == true && cmd->content)
	{
		exec->std_in = dup(STDIN_FILENO);
		exec->std_out = dup(STDOUT_FILENO);
		if (!ft_redirect(cmd, exec))
		{
			//free ? free child
			close(exec->std_in);
			close(exec->std_out);
			return (0);
		}
		ft_builtins(cmd, env, exec);
		
		dup2(exec->std_in, STDIN_FILENO);
		dup2(exec->std_out, STDOUT_FILENO);
		close(exec->std_in);
		close(exec->std_out);
		
		if (option == 1)
		{
			free_cmd_list(cmd);
			free_env_list(*env);
			free (*exec->line);
			free (*exec->prompt);
/* 			int i = 0;
			while (exec->cmd_tab[i]) //que si strdup
				free(exec->cmd_tab[i]); */
			free_cmd_arr(exec);
			free(exec);
		}
		if (option == 0)
			return (0);
		exit (0);
	}
	return (1);
}

/* Redirects command based on its input
--- if a redirection is detected, ft_redirect is called
--- if a builtin is detected, ft_builtins is called
---	if the cmd is not empty, exec_command is called
*/
// void	handle_command(t_element *cmd, t_env **env, t_pipe *exec)
void	handle_command(t_element *cmd, t_env **env, t_pipe *exec, int option)
{
	t_env	*exit_status;
	int		exit_nb;
	(void)option;

	exit_nb = 0;
	if (!ft_redirect(cmd, exec))
	{
		free_child(cmd, env, exec);
		exit(1);
	}
	exit_status = find_value_with_key_env(*env, "EXIT_STATUS");
	if (cmd->builtin == true)
	{
		ft_builtins(cmd, env, exec);
		close(exec->fd[0]);
		exit(ft_atoi(exit_status->value));
		return ;
	}
	if (exec->cmd_tab[0] != NULL)
	{
		exit_nb = exec_command(cmd, *env, exec);
		//add_exit_status_in_env(env, exit_nb);
	}
	// if (option == 0 && exec->cmd_tab[0] && (ft_strncmp(*exec->line, "$?", 2) != 0 || \
	// (ft_strncmp(*exec->line, "$?", 2) == 0 && ft_strlen(*exec->line) != 2)) != 0)
	// if (is_in_line(*exec->line, "$?") == true)
	// {
	// 	free(exec->cmd_tab[0]);
	// 	exec->cmd_tab[0] = NULL;
	// }
	free_child(cmd, env, exec);
	
	// if (cmd)
	// 	printf("---->cmd\n");
	// if (env)
	// 	printf("---->env\n");
	
	exit(exit_nb);
}



/* 
==1677443== For lists of detected and suppressed errors, rerun with: -s
==1677443== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
==1677373== Invalid free() / delete / delete[] / realloc()
==1677373==    at 0x484B27F: free (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==1677373==    by 0x40841E: ft_delete_node_cmd_parent (free_2.c:77)
==1677373==    by 0x408540: free_cmd_list_parent (free_2.c:111)
==1677373==    by 0x40177D: main (main.c:161)
==1677373==  Address 0x4b705a0 is 0 bytes inside a block of size 4 free'd
==1677373==    at 0x484B27F: free (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==1677373==    by 0x402E86: add_exit_status_in_env (exit.c:116)
==1677373==    by 0x404234: ft_exit_status_single (exec.c:82)
==1677373==    by 0x4040B8: single_command (exec.c:70)
==1677373==    by 0x403FA6: ft_execute (exec.c:38)
==1677373==    by 0x401762: main (main.c:135)
==1677373==  Block was alloc'd at
==1677373==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==1677373==    by 0x40B437: ft_itoa (in /mnt/nfs/homes/kquerel/Minishell/minishell)
==1677373==    by 0x402E8E: add_exit_status_in_env (exit.c:117)
==1677373==    by 0x4042F1: ft_exit_status_multiple (exec.c:132)
==1677373==    by 0x4041B8: multiple_commands (exec.c:114)
==1677373==    by 0x403FBC: ft_execute (exec.c:41)
==1677373==    by 0x401762: main (main.c:135)
 */