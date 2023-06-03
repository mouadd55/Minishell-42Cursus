/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/20 17:05:37 by yonadry           #+#    #+#             */
/*   Updated: 2023/06/03 15:58:35 by moudrib          ###   ########.fr       */
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

void	change_dir_2(t_env **envr, t_vars *v)
{
	t_env	*env;

	env = *envr;
	if (v->tmp_str && !chdir(v->tmp_str))
	{
		env = *envr;
		while (env)
		{
			if (!ft_strcmp(env->key, "PWD"))
			{
				v->tmp_value = getcwd(NULL, 0);
				free(env->value);
				env->value = ft_strdup(v->tmp_value);
			}
			if (!ft_strcmp(env->key, "OLDPWD"))
			{
				free(env->value);
				env->value = ft_strdup(v->val);
			}
			env = env->link;
		}
	}
	else
		perror(v->tmp_str);
}

void	change_dir(t_env **envr, t_command *f_list)
{
	t_env	*env;
	t_vars v;

	env = *envr;
	v.tmp_str = NULL;
	if (!f_list->cmd[1] || !ft_strcmp(f_list->cmd[1], "~"))
		v.tmp_str = ft_strdup(getenv("HOME"));
	else if (!ft_strcmp(f_list->cmd[1], "-"))
	{
		while (env)
		{
			if (!ft_strcmp(env->key, "OLDPWD"))
				v.tmp_str = ft_strdup(env->value);
			env = env->link;
		}
	}
	else
		v.tmp_str = ft_strdup(f_list->cmd[1]);
	v.val = getcwd(NULL, 0);
	change_dir_2(envr, &v);
	free(v.tmp_str);
	free(v.tmp_value);
	free(v.val);
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
		sort_env(env_copy, f_list->fd_out);
		ft_destroy_list_env(&env_copy);
	}
	if (export_parsing(list, envr))
		return ;
}
