/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yonadry <yonadry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/20 17:05:37 by yonadry           #+#    #+#             */
/*   Updated: 2023/06/01 15:03:45 by yonadry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	echo(t_command *f_list)
{
	t_vars v;

	v.flag = 0;
	v.i  = 1;
	while (f_list->cmd[v.i] && (ft_strnstr(f_list->cmd[v.i], "-n", 2))
	&& ft_count_char(&f_list->cmd[v.i][2], 'n') == ft_strlen(&f_list->cmd[v.i][2]))
	{
		v.flag = 1;
		v.i++;
	}
	while (f_list->cmd[v.i])
	{
		ft_putstr_fd(f_list->cmd[v.i], f_list->fd_out);
		if (f_list->cmd[v.i + 1])
			ft_putstr_fd(" ", f_list->fd_out);
		if (!f_list->cmd[v.i + 1])
			break;
		v.i++;

	}
	if (f_list && !v.flag)
		ft_putstr_fd("\n", f_list->fd_out);
}

char	*strlower(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] >= 'A' && str[i] <= 'Z')
			str[i] += 32;
		i++;
	}
	return (str);
}

void	pwd(t_command *f_list)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (pwd)
		ft_printf_fd("%s\n", f_list->fd_out, pwd);
	free(pwd);
}

void	change_dir_2(t_env **envr, t_command *f_list)
{
	t_env	*env;
	char	*pwd;

	env = *envr;
	pwd = getcwd(NULL, 0);
	if (f_list->cmd[1] && chdir(f_list->cmd[1]))
	{
		env = *envr;
		while (env)
		{
			if (!ft_strcmp(env->key, "PWD"))
				env->value = ft_strdup(getcwd(NULL, 0));
			if (!ft_strcmp(env->key, "OLDPWD"))
				env->value = ft_strdup(pwd);
			env = env->link;
		}
	}
	else
		perror(f_list->cmd[1]);
}

void	change_dir(t_env **envr, t_command *f_list)
{
	t_env	*env;

	env = *envr;
	if (!f_list->cmd[1] || !ft_strcmp(f_list->cmd[1], "~"))
	{
		chdir(getenv("HOME"));
		return;
	}
	else if (!ft_strcmp(f_list->cmd[1], "-"))
	{
		while (env)
		{
			if (!ft_strcmp(env->key, "OLDPWD"))
				f_list->cmd[1] = ft_strdup(env->value);
			env = env->link;
		}
	}
	change_dir_2(envr, f_list);
}

void	check_cmd(t_list **list, t_env **envr, t_command *f_list)
{
	t_list	*tmp;
	t_env	*env_copy;

	tmp = *list;
	if (f_list && !ft_strcmp(f_list->cmd[0], "echo"))
		echo(f_list);
	else if (f_list && !ft_strcmp(f_list->cmd[0], "cd"))
		change_dir(envr, f_list);
	else if (f_list && !ft_strcmp("pwd", strlower(f_list->cmd[0])))
		pwd(f_list);
	else if (*list && !(*list)->prev && (*list)->link
		&& (*list)->link->type[0] == 's' && !strcmp("unset", (*list)->content))
		unset(list, envr);
	if (ft_lstsize(*list) == 1 && !ft_strcmp((*list)->content, "export"))
	{
		env_copy = ft_copy_env_list(*envr);
		sort_env(env_copy);
		ft_destroy_list_env(&env_copy);
	}
	if (export_parsing(list, envr))
		return ;
}
