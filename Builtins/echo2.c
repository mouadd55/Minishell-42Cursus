/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 16:53:34 by yonadry           #+#    #+#             */
/*   Updated: 2023/06/25 16:09:34 by moudrib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	echo(t_cmd *f_list)
{
	t_vars	v;

	v.str = strlower(f_list->cmd[0]);
	if (!ft_strcmp("echo", v.str))
	{
		g_exit_status = 0;
		v.flag = 0;
		v.i = 1;
		while (f_list->cmd[v.i] && (ft_strnstr(f_list->cmd[v.i], "-n", 2))
			&& ft_count_char(&f_list->cmd[v.i][2],
			'n') == ft_strlen(&f_list->cmd[v.i][2]))
			v.i++;
		if (v.i > 1)
			v.flag = 1;
		while (f_list->cmd[v.i])
		{
			ft_putstr_fd(f_list->cmd[v.i], f_list->fd_out);
			if (f_list->cmd[v.i + 1])
				ft_putstr_fd(" ", f_list->fd_out);
			v.i++;
		}
		if (!v.flag)
			ft_putstr_fd("\n", f_list->fd_out);
	}
	free(v.str);
}

char	*ft_getenv(t_env *env, char *key)
{
	while (env)
	{
		if (!ft_strcmp(env->key, key))
			return (env->value);
		env = env->link;
	}
	return (NULL);
}

void	pwd(t_cmd *f_list, t_env *env)
{
	char	*pwd;
	int		i;

	i = 0;
	pwd = strlower(f_list->cmd[0]);
	if (!ft_strcmp("pwd", pwd))
	{
		free(pwd);
		pwd = getcwd(NULL, 0);
		if (!pwd)
		{
			pwd = ft_getenv(env, "PWD");
			i = 1;
		}
		ft_printf("%s\n", f_list->fd_out, pwd);
	}
	if (!i)
		free(pwd);
}

void	ft_setenv(t_env **envr, char *key, char *value)
{
	t_env	*env;

	env = *envr;
	while (env)
	{
		if (!ft_strcmp(env->key, key))
		{
			free(env->value);
			env->value = ft_strdup(value);
			break ;
		}
		env = env->link;
	}
	free(value);
	return ;
}
