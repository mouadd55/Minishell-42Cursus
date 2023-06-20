/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yonadry <yonadry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/20 17:05:37 by yonadry           #+#    #+#             */
/*   Updated: 2023/06/19 22:41:54 by yonadry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*strlower(char *str)
{
	int		i;
	char	*tmp;

	i = 0;
	tmp = ft_strdup(str);
	while (tmp[i])
	{
		if (tmp[i] >= 'A' && tmp[i] <= 'Z')
			tmp[i] += 32;
		i++;
	}
	return (tmp);
}

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
		{
			v.flag = 1;
			v.i++;
		}
		while (f_list->cmd[v.i])
		{
			ft_putstr_fd(f_list->cmd[v.i], f_list->fd_out);
			if (f_list->cmd[v.i + 1])
				ft_putstr_fd(" ", f_list->fd_out);
			v.i++;
		}
		if (f_list && !v.flag)
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

	pwd = strlower(f_list->cmd[0]);
	if (!ft_strcmp("pwd", pwd))
		ft_printf("%s\n", f_list->fd_out, ft_getenv(env, "PWD"));
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
			free(value);
			return ;
		}
		env = env->link;
	}
	return ;
}

void	change_dir_2(t_env **envr, t_vars *v)
{
	if (v->tmp_str && !chdir(v->tmp_str))
	{
		v->command = getcwd(NULL, 0);
		if (!v->command)
		{
			ft_setenv(envr, "OLDPWD", ft_strdup(ft_getenv(*envr, "PWD")));
			v->tmp_value = ft_getenv(*envr, "PWD");
			ft_setenv(envr, "PWD", ft_strjoin(ft_strdup(v->tmp_value), "/.."));
			ft_putstr_fd("cd: error retrieving current", 2);
			ft_putstr_fd(" directory: getcwd: cannot access parent ", 2);
			ft_putstr_fd("directories: No such file or directory\n", 2);
		}
		else
		{
			ft_setenv(envr, "OLDPWD", ft_strdup(ft_getenv(*envr, "PWD")));
			ft_setenv(envr, "PWD", v->command);
		}
	}
	else
	{
		g_exit_status = 1;
		perror(v->tmp_str);
	}
}

void	change_dir(t_env **envr, t_cmd *f_list)
{
	t_vars	v;

	v.tmp_str = NULL;
	if (!f_list->cmd[1] || !ft_strcmp(f_list->cmd[1], "~"))
		v.tmp_str = ft_strdup(getenv("HOME"));
	else if (!ft_strcmp(f_list->cmd[1], "-"))
		v.tmp_str = ft_strdup(ft_getenv(*envr, "OLDPWD"));
	else
		v.tmp_str = ft_strdup(f_list->cmd[1]);
	v.val = getcwd(NULL, 0);
	change_dir_2(envr, &v);
	free(v.tmp_str);
	// free(v.val);
}

void	check_cmd(t_list **list, t_env **envr, t_cmd *f_list)
{
	t_list	*tmp;
	t_env	*env_copy;

	tmp = *list;
	if (f_list && !ft_strcmp(f_list->cmd[0], "cd"))
		change_dir(envr, f_list);
	else if (*list && !(*list)->prev && (*list)->link
			&& (*list)->link->type[0] == 's' && !strcmp("unset",
				(*list)->content))
		unset(list, envr);
	if (!ft_strcmp(f_list->cmd[0], "export") && f_list->cmd[1] == 0)
	{
		env_copy = ft_copy_env_list(*envr);
		sort_env(env_copy, f_list->fd_out);
		ft_destroy_list_env(&env_copy);
	}
	if (export_parsing(list, envr, 0))
		return ;
	if (f_list->cmd && f_list->cmd[0] && !ft_strcmp(f_list->cmd[0], "exit"))
		ft_exit(f_list->cmd, f_list);
	else if (lstsize_cmd(f_list) == 1 && f_list->cmd && f_list->cmd[0]
			&& !ft_strcmp(f_list->cmd[0], "env"))
		env_parsing(f_list->cmd, *envr, f_list->fd_out);
	pwd(f_list, *envr);
	echo(f_list);
}
