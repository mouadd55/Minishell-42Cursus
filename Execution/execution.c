/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yonadry <yonadry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 20:20:23 by moudrib           #+#    #+#             */
/*   Updated: 2023/06/16 12:47:38 by yonadry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_if_builtin(t_cmd *final_list)
{
	int		i;
	char	**arr;

	i = -1;
	arr = ft_split("echo pwd cd export env exit unset", " ");
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

void	execute_commands(t_vars *v, t_env **env, int size)
{
	while (v->final_list)
	{
		if (pipe(v->pipefd) == -1)
		{
        	perror("minishell: pipe");
			return ;
    	}
		v->env_arr = create_2d_array_from_env_list(*env);
		v->child = fork();
		if (v->child < 0)
		{
			perror("minishell: fork");
			return ;
		}
		v->command = NULL;
		if (v->child == 0)
		{
			if (size == 1)
				simple_cmd(v->final_list, *env, v->command, v->env_arr);
			if (!v->final_list->prev && v->final_list->link)
				exec_st_cmd(v, env, v->env_arr, v->pipefd);
			else if (v->final_list->prev && v->final_list->link)
				exec_mid_cmd(v, env, v->env_arr, v->pipefd);
			else
				exec_last_cmd(v, env, v->env_arr, v->pipefd);
		}
		close(v->pipefd[1]);
		dup2(v->pipefd[0], STDIN_FILENO);
		close(v->pipefd[0]);
		ft_free_arr(v->env_arr);
		v->final_list = v->final_list->link;
	}
}

void	execution(t_cmd *final_list, t_env **env, t_list **lst)
{
	t_vars	v;
	int		std_in;

	v.lst = lst;
	v.final_list = final_list;
	std_in = dup(STDIN_FILENO);
	v.count = lstsize_cmd(final_list);
	if (final_list->cmd && final_list->cmd[0]
		&& final_list->fd_out != -1 && final_list->fd_in != -1)
		execute_commands(&v, env, v.count);
	while (wait (NULL) != -1)
	{
	}
	close(v.pipefd[0]);
	close(v.pipefd[1]);
	dup2(std_in, STDIN_FILENO);
}