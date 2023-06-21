/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 20:20:23 by moudrib           #+#    #+#             */
/*   Updated: 2023/06/21 09:40:26 by moudrib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_if_builtin(t_cmd *final_list)
{
	int		i;
	char	**arr;
	char	*tmp;

	i = -1;
	tmp = strlower(final_list->cmd[0]);
	if (!ft_strcmp(tmp, "pwd") || !ft_strcmp(tmp, "echo"))
	{
		free(tmp);
		return (1);
	}
	free(tmp);
	arr = ft_split("cd export env exit unset", " ");
	while (arr[++i])
	{
		if (final_list->cmd && final_list->cmd[0]
			&& !ft_strcmp(final_list->cmd[0], arr[i]))
		{
			ft_free_arr(arr);
			return (1);
		}
	}
	ft_free_arr(arr);
	return (0);
}

int	pipe_and_fork_protection(t_vars *v, t_env **env)
{
	if (pipe(v->pipefd) == -1)
	{
		perror("minishell: pipe");
		return (1);
	}
	v->env_arr = create_2d_array_from_env_list(*env);
	v->child = fork();
	if (v->child < 0)
	{
		ft_free_arr(v->env_arr);
		perror("minishell: fork");
		return (1);
	}
	v->command = NULL;
	return (0);
}

void	inside_child_process(t_vars *v, t_env **env, int size)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (v->final_list->cmd && !v->final_list->cmd[0])
		exit(0);
	if (size == 1)
		simple_cmd(v->final_list, *env, v->command, v->env_arr);
	if (!v->final_list->prev && v->final_list->link)
		exec_st_cmd(v, env, v->env_arr, v->pipefd);
	else if (v->final_list->prev && v->final_list->link)
		exec_mid_cmd(v, env, v->env_arr, v->pipefd);
	else
		exec_last_cmd(v, env, v->env_arr, v->pipefd);
}

void	execute_commands(t_vars *v, t_env **env, int size)
{
	while (v->final_list)
	{
		if (pipe_and_fork_protection(v, env))
			return ;
		if (v->child == 0)
			inside_child_process(v, env, size);
		close(v->pipefd[1]);
		dup2(v->pipefd[0], STDIN_FILENO);
		close(v->pipefd[0]);
		ft_free_arr(v->env_arr);
		while ((v->lst) && ft_strcmp((v->lst)->type, "PIPE"))
		{
			(v->lst) = (v->lst)->link;
			if ((v->lst) && !ft_strcmp((v->lst)->type, "PIPE"))
			{
				(v->lst) = (v->lst)->link;
				break ;
			}
		}
		v->final_list = v->final_list->link;
	}
}

void	execution(t_cmd *final_list, t_env **env, t_list **lst)
{
	t_vars	v;
	int		std_in;
	int		std_out;

	v.lst = *lst;
	v.final_list = final_list;
	std_in = dup(STDIN_FILENO);
	std_out = dup(STDOUT_FILENO);
	v.count = lstsize_cmd(final_list);
	signal(SIGINT, SIG_IGN);
	if (final_list->cmd && final_list->fd_out != -1 
		&& final_list->fd_in != -1)
		execute_commands(&v, env, v.count);
	// signal(SIGQUIT, SIG_IGN);
	ft_destroy_list(lst);
	dup2(std_in, STDIN_FILENO);
	dup2(std_out, STDOUT_FILENO);
	close(std_in);
	close(std_out);
	exit_by_signal();
	signal(SIGINT, &catching_signals);
}
