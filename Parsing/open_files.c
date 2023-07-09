/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_files.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/29 11:57:03 by yonadry           #+#    #+#             */
/*   Updated: 2023/07/07 13:43:43 by moudrib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	close_fd(t_cmd *tmp, t_vars *v)
{
	if (!ft_strcmp(v->tmp_value, ">") || !ft_strcmp(v->tmp_value, ">>"))
	{
		if (tmp->fd_out >= 3)
			close(tmp->fd_out);
		tmp->fd_out = v->fd;
	}
	else if (!ft_strcmp(v->tmp_value, "<") || !ft_strcmp(v->tmp_value,
			"<<"))
	{
		if (tmp->fd_in >= 3)
			close(tmp->fd_in);
		tmp->fd_in = v->fd;
		if (v->flag == 230)
		{
			tmp->fd_in = -2;
			v->flag = 0;
		}
		if (!ft_strcmp(v->tmp_value, "<<"))
		{
			tmp->file_name = ft_strdup(v->command);
			free(v->command);
		}
	}
}

int	var_redirect(t_list *list, t_vars *v)
{
	v->tmp_value = ft_strdup(is_redir(list));
	if (list->link && !ft_strcmp(list->link->type, "space"))
		list = list->link->link;
	else if (list->link)
		list = list->link;
	v->str = NULL;
	if (!ft_strcmp("VAR", list->type))
	{
		g_exit_status = 1;
		ft_printf("minishell: %s: ambiguous redirect\n", 2, list->content);
		return (1);
	}
	else if (!ft_strcmp("FILE", list->type))
	{
		while (list && !ft_strcmp("FILE", list->type))
		{
			v->str = ft_strjoin(v->str, list->content);
			list = list->link;
		}
		v->fd = open_file(v->str, v->tmp_value);
		if (v->fd == -1)
			return (1);
		free(v->str);
	}
	return (0);
}

t_cmd	*add_fd(t_vars *v, t_cmd *tmp, t_list *list)
{
	if (v->tmp_value && tmp)
	{
		close_fd(tmp, v);
		free(v->tmp_value);
		v->tmp_value = NULL;
	}
	if (list && !ft_strcmp(list->type, "PIPE") && tmp && tmp->link)
		tmp = tmp->link;
	return (tmp);
}

t_list	*if_redirect(t_cmd *tmp, t_vars *v, t_env **envr, t_vars *p)
{
	if (tmp->fd_in >= 3)
		close (tmp->fd_in);
	if (tmp->fd_out >= 3)
		close (tmp->fd_out);
	tmp->fd_in = -1;
	tmp->fd_out = -1;
	while (v->tmp1 && ft_strcmp(v->tmp1->type, "PIPE"))
	{
		if (!ft_strcmp(v->tmp1->type, "HEREDOC"))
		{
			if_heredoce(v, tmp, envr, p);
			free(v->tmp_value);
			tmp->file_name = ft_strdup(v->command);
			free(v->command);
		}
		v->tmp1 = v->tmp1->link;
	}
	if (tmp && tmp->link)
		tmp = tmp->link;
	free(v->str);
	v->tmp_value = NULL;
	return (v->tmp1);
}

void	open_files(t_list *list, t_cmd *tmp, t_env **envr)
{
	t_vars	v;
	t_vars	p;

	v.command = NULL;
	v.tmp_value = NULL;
	v.str = NULL;
	v.fd = 0;
	while (list)
	{
		v.tmp1 = list;
		if (!ft_strcmp(list->type, "HEREDOC"))
			if_heredoce(&v, tmp, envr, &p);
		else if (is_redir(list) && var_redirect(list, &v))
		{
			free(v.tmp_value);
			list = if_redirect(tmp, &v, envr, &p);
		}
		if (list && !ft_strcmp(list->content, "|") && tmp && tmp->link)
			tmp = tmp->link;
		if (v.fd >= 3)
			tmp = add_fd(&v, tmp, list);
		if (!list || g_exit_status == -1)
			break ;
		list = list->link;
	}
}
