/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/11 22:04:34 by moudrib           #+#    #+#             */
/*   Updated: 2023/06/12 16:26:33 by moudrib          ###   ########.fr       */
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

void	dup_file_descriptors(char *command, t_cmd *f_list, char **env_arr)
{
	if (f_list->fd_out != -1 && f_list->fd_in != -1)
	{	
		if (f_list->fd_out != STDOUT_FILENO)
		{
			dup2(f_list->fd_out, STDOUT_FILENO);
			close(f_list->fd_out);
		}
		if (f_list->fd_in != STDIN_FILENO)
		{
			dup2(f_list->fd_in, STDIN_FILENO);
			close(f_list->fd_in);
		}
		if (execve(command, f_list->cmd, env_arr) == -1)
		{
			printf("minishell: %s: command not found\n", f_list->cmd[0]);
			free(command);
			ft_free_arr(env_arr);
			exit(1);
		}
	}
}

void	simple_cmd(t_cmd *f_list, t_env *env, char *command
	, char **env_arr)
{
	if (f_list->cmd && f_list->cmd[0])
	{
		if (check_if_builtin(f_list))
			exit (0);
		command = get_paths(f_list->cmd[0], env);
		if (!command)
		{
			ft_free_arr(env_arr);
			printf("minishell: %s: No such file or directory\n", f_list->cmd[0]);
			exit(1);
		}
		dup_file_descriptors(command, f_list, env_arr);
	}
}
