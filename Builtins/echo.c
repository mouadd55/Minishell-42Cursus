/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/20 17:05:37 by yonadry           #+#    #+#             */
/*   Updated: 2023/06/21 17:27:41 by moudrib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
	free(v.val);
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
