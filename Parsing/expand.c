/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yonadry <yonadry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/20 17:10:36 by yonadry           #+#    #+#             */
/*   Updated: 2023/06/21 16:50:27 by yonadry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	expand_var_2(t_list **list, t_list **tmp, t_env *envr, t_vars *v)
{
	if (*tmp && (*tmp)->content[0] == '$' && (*tmp)->link)
	{
		(*tmp) = (*tmp)->link;
		while (ft_isalpha((*tmp)->content[v->i]) || check_char("0123456789_",
				(*tmp)->content[v->i]))
			v->i++;
		v->str = ft_substr((*tmp)->content, 0, v->i);
		if (ft_getenv(envr, v->str))
		{
			free((*tmp)->prev->content);
			(*tmp)->prev->content = ft_strdup(ft_getenv(envr, v->str));
			v->command = ft_strdup(&(*tmp)->content[v->i]);
			(*tmp)->prev->content = ft_strjoin((*tmp)->prev->content,
					v->command);
			*tmp = del_node(list, *tmp);
			free(v->command);
		}
	}
}

void	remove_dollar2(t_list **list, t_list *tmp)
{
	if ((!ft_strcmp(tmp->content, "$") && tmp->link
			&& tmp->link->content[0] != 32))
	{
		tmp->content = ft_strjoin(tmp->content, tmp->link->content);
		free(tmp->type);
		if (tmp->prev && (!ft_strcmp(tmp->prev->content, "<<")
				|| (tmp->prev->prev && !ft_strcmp(tmp->prev->prev->content,
						"<<"))))
			tmp->type = ft_strdup("DELIMITER");
		else
			tmp->type = ft_strdup("VAR");
		if (tmp->link)
			tmp = del_node(list, tmp->link);
	}
}

void	remove_dollar(t_list **list)
{
	t_list	*tmp;

	tmp = *list;
	while (tmp)
	{
		if ((!ft_strcmp(tmp->content, "$") && tmp->link
				&& (!ft_strcmp(tmp->link->type, "DOUBLE_Q")
					|| !ft_strcmp(tmp->link->type, "SINGLE_Q"))))
		{
			free(tmp->content);
			tmp->content = ft_strdup(tmp->link->content);
			tmp = del_node(list, tmp->link);
		}
		else
			remove_dollar2(list, tmp);
		tmp = tmp->link;
	}
}

void	expand_exit_status(t_list *tmp)
{
	t_vars	v;

	while (tmp)
	{
		if (!ft_strcmp(tmp->content, "$?"))
		{
			v.str = ft_itoa(g_exit_status);
			free(tmp->content);
			tmp->content = ft_strdup(v.str);
			free(tmp->type);
			tmp->type = ft_strdup("COMMAND");
			free(v.str);
		}
		tmp = tmp->link;
	}
}

void	expand_var(t_list **list, t_env *envr, int rm_quotes)
{
	t_list	*tmp;
	t_vars	v;

	v.i = 0;
	tmp = *list;
	v.str = NULL;
	while (tmp && ft_strcmp(tmp->type, "DELIMITER"))
	{
		expand_var_2(list, &tmp, envr, &v);
		if (v.str)
		{
			free(v.str);
			v.str = NULL;
		}
		tmp = tmp->link;
	}
	expand_in_quotes(list, envr, "DOUBLE_Q");
	if (rm_quotes)
		remove_quotes(list);
	tmp = *list;
	remove_dollar(&tmp);
	expand_exit_status(tmp);
}
