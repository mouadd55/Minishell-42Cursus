/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 20:20:23 by moudrib           #+#    #+#             */
/*   Updated: 2023/06/12 17:00:54 by moudrib          ###   ########.fr       */
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

void	execution(t_cmd *final_list, t_env **env, t_list **lst)
{
	t_vars	v;
	int		std_in;
	int		std_out;
	int		pipefd[2];
	char	**env_arr;

	v.lst = lst;
	v.final_list = final_list;
	std_in = dup(STDIN_FILENO);
	std_out = dup(STDOUT_FILENO);
	v.count = lstsize(final_list);
	while (v.final_list)
	{
		pipe(pipefd);
		env_arr = create_2d_array_from_env_list(*env);
		v.child = fork();
		v.command = NULL;
		if (v.child < 0)
			ft_printf_fd("minishell: fork: Resource temporarily unavailable\n",
				2);
		else if (v.child == 0)
		{
			if (lstsize(final_list) == 1)
				simple_cmd(v.final_list, *env, v.command, env_arr);
			if (!v.final_list->prev && v.final_list->link)
				exec_st_cmd(&v, env, env_arr, pipefd);
			else if (v.final_list->prev && v.final_list->link)
				exec_mid_cmd(&v, env, env_arr, pipefd);
			else
				exec_last_cmd(&v, env, env_arr, pipefd);
		}
		close(pipefd[1]);
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
		ft_free_arr(env_arr);
		v.final_list = v.final_list->link;
	}
	while (wait (NULL) != -1)
	{
	}
	close(pipefd[0]);
	close(pipefd[1]);
	dup2(std_in, STDIN_FILENO);
	dup2(std_out, STDOUT_FILENO);
}
