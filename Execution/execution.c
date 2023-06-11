/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 20:20:23 by moudrib           #+#    #+#             */
/*   Updated: 2023/06/11 19:39:06 by moudrib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_path_value(t_env *env)
{
	while (env)
	{
		if (!ft_strcmp("PATH", env->key))
			return (env->value);
		env = env->link;
	}
	return (0);
}

char	**create_2d_array_from_env_list(t_env *env)
{
	int		i;
	char	*str;
	char	**env_arr;
	int		env_length;
	
	i = 0;
	env_length = ft_lstsize_env(env);
	env_arr = malloc((env_length + 1) * sizeof(char *));
	if (!env_arr)
		return (0);
	while (env)
	{
		str = NULL;
		str = ft_strjoin(str, env->key);
		str = ft_strjoin(str, "=");
		str = ft_strjoin(str, env->value);
		env_arr[i] = ft_strdup(str);
		free(str);
		i++;
		env = env->link;
	}
	env_arr[i] = 0;
	return (env_arr);
}


char	*get_paths(char *cmd, t_env *env)
{
	int		i;
	char	*path_value;
	char	**paths;
	char	*path;

	i = -1;
	path_value = get_path_value(env);
	if (!path_value)
		return (0);
	paths = ft_split(path_value, ":");
	if (!paths)
		return (0);
	while (paths[++i])
	{
		paths[i] = ft_strjoin(paths[i], "/");
		paths[i] = ft_strjoin(paths[i], cmd);
		if (!access(paths[i], X_OK))
		{
			path = ft_strdup(paths[i]);
			ft_free_arr(paths);
			return (path);
		}
	}
	ft_free_arr(paths);
	return (cmd);
}

int	check_if_builtin(t_command *final_list)
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

void	simple_command(t_command *final_list, t_env *env, char *command, char **env_arr)
{
	if (final_list->cmd && final_list->cmd[0])
	{
		if (check_if_builtin(final_list))
			exit (0);
		command = get_paths(final_list->cmd[0], env);
		if (!command)
		{
			ft_free_arr(env_arr);
			printf("minishell: %s: No such file or directory\n", final_list->cmd[0]);
			exit(1);
		}
		if (final_list->fd_out != -1 && final_list->fd_in != -1)
		{	
			if (final_list->fd_out != STDOUT_FILENO)
			{
				dup2(final_list->fd_out, STDOUT_FILENO);
				close(final_list->fd_out);
			}
			if (final_list->fd_in != STDIN_FILENO)
			{
				dup2(final_list->fd_in, STDIN_FILENO);
				close(final_list->fd_in);
			}
			if (execve(command, final_list->cmd, env_arr) == -1)
			{
				printf("minishell: %s: command not found\n", final_list->cmd[0]);
				free(command);
				ft_free_arr(env_arr);
				exit(1);
			}
		}
	}
	free(command);
	ft_free_arr(env_arr);
	exit(0);
}

void	execute_first_command(t_vars *v, t_env *env, char **env_arr, int pipefd[2])
{
	v->command = get_paths(v->final_list->cmd[0], env);
	if (!v->command)
	{
		ft_free_arr(env_arr);
		printf("minishell: %s: No such file or directory\n", v->final_list->cmd[0]);
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
	if (execve(v->command, v->final_list->cmd, env_arr) == -1)
	{
		free(v->command);
		ft_free_arr(env_arr);
		printf("minishell: %s: command not found\n", v->final_list->cmd[0]);
		exit(1);
	}
	free(v->command);
	ft_free_arr(env_arr);
	exit(0);
}

void	execute_middle_commands(t_vars *v, t_env *env, char **env_arr, int pipefd[2])
{
	v->command = get_paths(v->final_list->cmd[0], env);
	if (!v->command)
	{
		ft_free_arr(env_arr);
		printf("minishell: %s: No such file or directory\n", v->final_list->cmd[0]);
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
		close(v->final_list->fd_out);
		close(pipefd[0]);
		close(pipefd[1]);
	}
	if (execve(v->command, v->final_list->cmd, env_arr) == -1)
	{
		free(v->command);
		ft_free_arr(env_arr);
		printf("minishell: %s: command not found\n", v->final_list->cmd[0]);
		exit(1);
	}
	free(v->command);
	ft_free_arr(env_arr);
	exit(0);
}

void	execute_last_command(t_vars *v, t_env *env, char **env_arr, int pipefd[2])
{
	v->command = get_paths(v->final_list->cmd[0], env);
	if (!v->command)
	{
		ft_free_arr(env_arr);
		printf("minishell: %s: No such file or directory\n", v->final_list->cmd[0]);
		exit(1);
	}
	if (v->final_list->fd_out != STDOUT_FILENO)
	{
		dup2(v->final_list->fd_out, STDOUT_FILENO);
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
		dup2(pipefd[0], v->final_list->fd_in);
		close(pipefd[0]);
	}
	if (execve(v->command, v->final_list->cmd, env_arr) == -1)
	{
		free(v->command);
		ft_free_arr(env_arr);
		printf("minishell: %s: command not found\n", v->final_list->cmd[0]);
		exit(1);
	}
	free(v->command);
	ft_free_arr(env_arr);
	exit(0);
}

void	execution(t_command *final_list, t_env *env)
{
	t_vars	v;
	pid_t	child1;
	int		pipefd[2];
	char	**env_arr;
	int	stdin = dup(STDIN_FILENO);
	int	stdout = dup(STDOUT_FILENO);
	v.final_list = final_list;
	// if (v.final_list->fd_in != -1 && v.final_list->fd_out != -1)
	// {
		while (v.final_list)
		{
			pipe(pipefd);
			env_arr = create_2d_array_from_env_list(env);
			child1 = fork();
			v.command = NULL;
			if (child1 < 0)
				printf("minishell: fork: Resource temporarily unavailable\n");
			else if (child1 == 0)
			{
				if (lstsize(v.final_list) == 1)
					simple_command(v.final_list, env, v.command, env_arr);
				if (!v.final_list->prev && v.final_list->link)
					execute_first_command(&v, env, env_arr, pipefd);
				else if (v.final_list->prev && v.final_list->link)
					execute_middle_commands(&v, env, env_arr, pipefd);
				else
					execute_last_command(&v, env, env_arr, pipefd);
			}
			close(pipefd[1]);
			dup2(pipefd[0], STDIN_FILENO);
			close(pipefd[0]);
			ft_free_arr(env_arr);
			v.final_list = v.final_list->link;
		}
		while(wait(NULL) != -1);
		close(pipefd[0]);
		close(pipefd[1]);
		dup2(stdin, STDIN_FILENO);
		dup2(stdout, STDOUT_FILENO);
	// }
}
