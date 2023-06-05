/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 20:20:23 by moudrib           #+#    #+#             */
/*   Updated: 2023/06/05 20:10:17 by moudrib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_paths(char *cmd)
{
	int		i;
	char	*path_value;
	char	**paths;
	char	*path;

	i = -1;
	path_value = getenv("PATH");
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

void	execution(t_command *final_list)
{
	int		i;
	int		childPid;
	char	*command;
	char	**arr;

	childPid = fork();
	while (final_list)
	{
		if (childPid == 0)
		{
			i = -1;
			arr = ft_split("echo pwd cd export env exit unset", " ");
			while (arr[++i])
			{
				if (!ft_strcmp(final_list->cmd[0], arr[i]))
				{
					ft_free_arr(arr);
					exit (0);
				}
			}
			command = get_paths(final_list->cmd[0]);
			if (execv(command, final_list->cmd) == -1)
			{
				free(command);
				printf("minishell: %s: command not found\n", final_list->cmd[0]);
				exit(1);
			}
			free(command);
			exit(0);
		}
		else if (childPid > 0)
			wait(NULL);
		else
			printf("\nFork failed. Unable to execute command: %s", final_list->cmd[0]);
		final_list = final_list->link;
	}
}
