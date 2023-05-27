/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yonadry <yonadry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/20 17:05:37 by yonadry           #+#    #+#             */
/*   Updated: 2023/05/27 14:45:00 by yonadry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_list	*skip_n_echo(t_list *tmp, int *flag)
{
	while (tmp)
	{
		if (tmp && tmp->link && ((!ft_strcmp(tmp->content, "-n")
					&& !ft_strcmp(tmp->link->content, "-n"))
				|| ((ft_strnstr(tmp->content, "-n", 2)
						&& ft_count_char(&tmp->content[2],
							'n') == ft_strlen(&tmp->content[2]))
					&& (ft_strnstr(tmp->link->content, "-n", 2)
						&& ft_count_char(&tmp->link->content[2],
							'n') == ft_strlen(&tmp->link->content[2])))))
			break ;
		if (tmp && (!ft_strcmp(tmp->content, "-n") || (ft_strnstr(tmp->content,
						"-n", 2) && ft_count_char(&tmp->content[2],
						'n') == ft_strlen(&tmp->content[2]))))
		{
			*flag = 1;
			if (tmp && tmp->link && tmp->link->link)
				tmp = tmp->link->link;
			else
				return (tmp->link);
		}
		else
			return (tmp);
	}
	return (tmp);
}

void	echo(t_list *list, int fd)
{
	t_list	*tmp;
	int		flag;

	tmp = list;
	flag = 0;
	if (list && list->link && !ft_strcmp(list->content, "echo"))
	{
		tmp = tmp->link->link;
		tmp = skip_n_echo(tmp, &flag);
		if (tmp && tmp->content[0] == 32 && tmp->link)
			tmp = tmp->link;
		while (tmp)
		{
			if (ft_strnstr("PIPE,HEREDOC,APPEND,OUTFILE,INFILE", tmp->type, 32)
				|| (!ft_strcmp(" ", tmp->content)
					&& ft_strnstr("PIPE,HEREDOC,APPEND,OUTFILE,INFILE", tmp->link->type, 32)))
				break ;
			if (tmp)
				ft_putstr_fd(tmp->content, fd);
			tmp = tmp->link;
		}
	}
	if (!flag && list && !ft_strcmp(list->content, "echo"))
		ft_putstr_fd("\n", fd);
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

void	pwd(int fd)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (pwd)
		ft_printf_fd("%s\n", fd, pwd);
	free(pwd);
}

void	change_dir_2(t_list *list, t_env **envr, int fd)
{
	t_env	*env;
	char	*pwd;

	env = *envr;
	pwd = getcwd(NULL, 0);
	if (fd == -1)
		fd = 2;
	if (list && !chdir(list->content))
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
	{
		ft_printf_fd("minishel: cd: %s: No such file or directory\n", fd, list->content);
		return ;
	}
}

void	change_dir(t_list *list, t_env **envr, int fd)
{
	t_env	*env;

	env = *envr;
	if (list && (!list->link || (list->link->link
				&& (!ft_strcmp(list->link->link->content, "~")))))
		list->content = ft_strdup(getenv("HOME"));
	else if (list && !ft_strcmp(list->link->link->content, "-"))
	{
		while (env)
		{
			if (!ft_strcmp(env->key, "OLDPWD"))
				list->content = ft_strdup(env->value);
			env = env->link;
		}
	}
	else if (list && list->link && list->link->link)
		list = list->link->link;
	change_dir_2(list, envr, fd);
}

void	check_cmd(t_list **list, t_env **envr, char *input, int fd)
{
	t_list	*tmp;
	t_env	*env_copy;

	tmp = *list;
	if (*list && !ft_strcmp((*list)->content, "echo"))
		echo(*list, fd);
	else if (*list && !ft_strcmp((*list)->content, "cd"))
		change_dir(*list, envr, fd);
	else if (ft_strnstr(input, "exit", ft_strlen(input)))
	{
		while (tmp)
		{
			if (!ft_strcmp(tmp->content, "exit"))
				ft_exit(list);
			tmp = tmp->link;
		}
	}
	else if (list && !strcmp("pwd", strlower((*list)->content)))
		pwd(fd);
	else if (*list && !(*list)->prev && (*list)->link
		&& (*list)->link->type[0] == 's' && !strcmp("unset", (*list)->content))
		unset(list, envr);
	if (ft_lstsize(*list) == 1 && !ft_strcmp((*list)->content, "export"))
	{
		env_copy = ft_copy_env_list(*envr);
		sort_env(env_copy);
		ft_destroy_list_env(&env_copy);
	}
	if (ft_strlen(input) && (export_parsing(list, envr)))
		return ;
}
