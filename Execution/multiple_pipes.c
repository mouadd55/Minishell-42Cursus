/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiple_pipes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/11 22:14:31 by moudrib           #+#    #+#             */
/*   Updated: 2023/06/12 19:27:43 by moudrib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	exec_st_cmd(t_vars *v, t_env **env, char **env_arr, int pipefd[2])
{
	if (v->final_list->cmd && v->final_list->cmd[0]
		&& v->final_list->fd_in != -1 && v->final_list->fd_out != -1)
	{
		v->command = get_paths(v->final_list->cmd[0], *env);
		if (!v->command)
		{
			ft_free_arr(env_arr);
			ft_printf("minishell: %s: No such file or directory\n", 2, v->final_list->cmd[0]);
			exit(1);
		}
		if (v->final_list->fd_in != STDIN_FILENO)
		{
			dup2(v->final_list->fd_in, STDIN_FILENO);
			close (v->final_list->fd_in);
		}
		if (v->final_list->fd_out != STDOUT_FILENO)
		{
			dup2(v->final_list->fd_out, STDOUT_FILENO);
			if (!check_if_builtin(v->final_list))
				close (v->final_list->fd_out);
			close(pipefd[0]);
			close(pipefd[1]);
		}
		else
		{
			close(pipefd[0]);
			dup2(pipefd[1], STDOUT_FILENO);
			close(pipefd[1]);
		}
		if (check_if_builtin(v->final_list))
		{
			ft_builtins(v->lst, *env, v->final_list, v->count);
			exit (0);
		}
		else if (execve(v->command, v->final_list->cmd, env_arr) == -1)
		{
			ft_printf("minishell: %s: command not found\n", 2, v->final_list->cmd[0]);
			free(v->command);
			ft_free_arr(env_arr);
			exit(1);
		}
	}
}

void	exec_mid_cmd(t_vars *v, t_env **env, char **env_arr, int pipefd[2])
{
	if (v->final_list->cmd && v->final_list->cmd[0]
		&& v->final_list->fd_in != -1 &&  v->final_list->fd_out != -1)
	{
		v->command = get_paths(v->final_list->cmd[0], *env);
		if (!v->command)
		{
			ft_free_arr(env_arr);
			ft_printf("minishell: %s: No such file or directory\n", 2, v->final_list->cmd[0]);
			exit(1);
		}
		if (v->final_list->fd_in != STDIN_FILENO)
		{
			dup2(v->final_list->fd_in, STDIN_FILENO);
			close(v->final_list->fd_in);
		}
		if (v->final_list->fd_out == STDOUT_FILENO)
		{
			close(pipefd[0]);
			dup2(pipefd[1], STDOUT_FILENO);
			close(pipefd[1]);
		}
		else
		{
			dup2(v->final_list->fd_out, STDOUT_FILENO);
			if (!check_if_builtin(v->final_list))
				close(v->final_list->fd_out);
			close(pipefd[0]);
			close(pipefd[1]);
		}
		if (check_if_builtin(v->final_list))
		{
			ft_builtins(v->lst, *env, v->final_list, v->count);
			exit (0);
		}
		else if (execve(v->command, v->final_list->cmd, env_arr) == -1)
		{
			ft_printf("minishell: %s: command not found\n", 2, v->final_list->cmd[0]);
			free(v->command);
			ft_free_arr(env_arr);
			exit(1);
		}
	}
}

void	exec_last_cmd(t_vars *v, t_env **env, char **env_arr, int pipefd[2])
{
	if (v->final_list->cmd && v->final_list->cmd[0]
		&& v->final_list->fd_in != -1 &&  v->final_list->fd_out != -1)
	{
		v->command = get_paths(v->final_list->cmd[0], *env);
		if (!v->command)
		{
			ft_free_arr(env_arr);
			ft_printf("minishell: %s: No such file or directory\n", 2, v->final_list->cmd[0]);
			exit(1);
		}
		if (v->final_list->fd_out != STDOUT_FILENO)
		{
			dup2(v->final_list->fd_out, STDOUT_FILENO);
			ft_printf("out :%d %d\n",2,v->final_list->fd_out,STDOUT_FILENO);
			if (!check_if_builtin(v->final_list))
				close(v->final_list->fd_out);
		}
		if (v->final_list->fd_in != STDIN_FILENO)
		{
			dup2(v->final_list->fd_in, STDIN_FILENO);
			close(v->final_list->fd_in);
			close(pipefd[0]);
			close(pipefd[1]);
		}
		else
		{
			close(pipefd[1]);
			// dup2(pipefd[0], STDIN_FILENO);
			close(pipefd[0]);
		}
		if (check_if_builtin(v->final_list))
		{
			ft_builtins(v->lst, *env, v->final_list, v->count);
			exit (0);
		}
		else if (execve(v->command, v->final_list->cmd, env_arr) == -1)
		{
			ft_printf("minishell: %s: command not found\n", 2, v->final_list->cmd[0]);
			free(v->command);
			ft_free_arr(env_arr);
			exit(1);
		}
	}
}
