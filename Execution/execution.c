/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 20:20:23 by moudrib           #+#    #+#             */
/*   Updated: 2023/06/04 20:32:30 by moudrib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_paths(char *cmd)
{
	int		i;
	char	*path_value;
	char	**paths;

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
			return (paths[i]);
	}
	return (cmd);
}

void	execution(t_command *final_list)
{
	int		childPid;
	int		pipefd[2];
	char	*command;

	pipe(pipefd);
	childPid = fork();
	while (final_list)
	{
		if (childPid == 0)
		{
			if (ft_strnstr("echo, pwd, cd, export, env, exit, unset", final_list->cmd[0], 40))
				exit (0);
			command = get_paths(final_list->cmd[0]);
			// close(pipefd[0]);
			// dup2(pipefd[1], 1);
			// close(pipefd[1]);
			if (execv(command, final_list->cmd) == -1)
				printf("\nminishell: %s: command not found", final_list->cmd[0]);
			exit(0);
		}
		final_list = final_list->link;
	}
}
