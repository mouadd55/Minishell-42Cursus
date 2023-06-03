/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 18:22:34 by moudrib           #+#    #+#             */
/*   Updated: 2023/06/03 15:56:23 by moudrib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_env(int i, int count, t_env *env, int fd_out)
{
	if (i && i == count)
	{
		while (env)
		{
			if (env->value)
				ft_printf_fd("%s=%s\n", fd_out, env->key, env->value);
			env = env->link;
		}
	}	
}

void	env_parsing(char **cmd, t_env *env, int fd_out)
{
	t_vars	v;

	v.i = -1;
	v.count = 0;
	while (cmd[++v.i])
	{
		if (ft_strcmp(cmd[v.i], "env") == 0)
			v.count++;
		else if (v.count && cmd[v.i][0] == '-' && ft_strlen(cmd[v.i]) > 1)
		{
			printf("env: illegal option -- %c\n", cmd[v.i][1]);
			return ;
		}
		else if (v.count && cmd[v.i][0] != '-')
		{
			printf("env: %s: No such file or directory\n", cmd[v.i]);
			return ;
		}
	}
	print_env(v.i, v.count, env, fd_out);
}

void	ft_lstadd_back_env(t_env **head, t_env *new)
{
	t_env	*tmp;

	if (!*head || !head)
		*head = new;
	else
	{
		tmp = ft_lstlast_env(*head);
		tmp->link = new;
		new->prev = tmp;
	}
}

void	*ft_destroy_list_env(t_env **head)
{
	t_env	*tmp;

	if (!head || !*head)
		return (0);
	tmp = *head;
	while (tmp)
	{
		tmp = (*head)->link;
		free((*head)->key);
		free((*head)->value);
		free(*head);
		(*head) = tmp;
	}
	return (0);
}

t_env	*ft_split_environment(char **env)
{
	int		i;
	int		start;
	int		length;
	t_env	*envr;

	i = -1;
	envr = NULL;
	while (env[++i])
	{
		start = 0;
		length = ft_strlen(env[i]);
		while (env[i][start] != '=')
			start++;
		ft_lstadd_back_env(&envr, ft_lstnew_env(ft_substr(env[i], 0, start),
				ft_substr(env[i], start + 1, length)));
	}
	return (envr);
}
