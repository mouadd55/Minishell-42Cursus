/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 18:22:34 by moudrib           #+#    #+#             */
/*   Updated: 2023/06/13 10:21:33 by moudrib          ###   ########.fr       */
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
				ft_printf("%s=%s\n", fd_out, env->key, env->value);
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
			ft_printf("env: illegal option -- %c\n", 2, cmd[v.i][1]);
			return ;
		}
		else if (v.count && cmd[v.i][0] != '-')
		{
			ft_printf("env: %s: No such file or directory\n", 2, cmd[v.i]);
			return ;
		}
	}
	print_env(v.i, v.count, env, fd_out);
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
