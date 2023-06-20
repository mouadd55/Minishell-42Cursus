/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yonadry <yonadry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/20 17:10:36 by yonadry           #+#    #+#             */
/*   Updated: 2023/06/16 13:54:55 by yonadry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	index_list(t_list **list)
{
	t_list	*tmp;
	int		count;

	tmp = *list;
	count = 0;
	while (tmp)
	{
		tmp->pos = count;
		count++;
		tmp = tmp->link;
	}
}

void	del_node_2(t_list **tmp, t_list **tmp1, t_list *del_node)
{
	while (*tmp)
	{
		if (*tmp == del_node)
		{
			*tmp1 = (*tmp)->prev;
			(*tmp1)->link = (*tmp)->link;
			(*tmp)->link->prev = *tmp1;
			free((*tmp)->type);
			free((*tmp)->content);
			free(*tmp);
			*tmp = *tmp1;
		}
		*tmp = (*tmp)->link;
	}
}

t_list	*del_node(t_list **list, t_list *del_node)
{
	t_list	*tmp;
	t_list	*tmp1;

	tmp1 = NULL;
	tmp = *list;
	if (del_node == tmp)
	{
		tmp = tmp->link;
		free(tmp->prev->type);
		free(tmp->prev->content);
		free(tmp->prev);
		tmp->prev = NULL;
		return (tmp);
	}
	else if (del_node == ft_lstlast(tmp))
	{
		tmp1 = ft_lstlast(tmp);
		tmp1 = tmp1->prev;
		free(tmp1->link->type);
		free(tmp1->link->content);
		free(tmp1->link);
		tmp1->link = NULL;
	}
	else
		del_node_2(&tmp, &tmp1, del_node);
	return (tmp1);
}

void	remove_quotes(t_list **list)
{
	t_vars v;

	v.tmp1 = *list;
	while (v.tmp1)
	{
		if (!ft_strcmp("DOUBLE_Q", v.tmp1->type) || !ft_strcmp("SINGLE_Q",
				v.tmp1->type) || (!ft_strcmp("FILE", v.tmp1->type)))
		{
			if (v.tmp1->content[0] == '\"')
			{
				v.command = ft_strtrim(v.tmp1->content, "\"");
				free(v.tmp1->content);
				v.tmp1->content = ft_strdup(v.command);
				free(v.command);
			}
			else if (v.tmp1->content[0] == '\'')
			{
				v.command = ft_strtrim(v.tmp1->content, "\'");
				free(v.tmp1->content);
				v.tmp1->content = ft_strdup(v.command);
				free(v.command);
			}
		}
		v.tmp1 = v.tmp1->link;
	}
}

int	is_alpha_num(char c)
{
	if (ft_isalpha(c) || check_char("0123456789", c))
		return (1);
	return (0);
}

void	 expand_in_quotes_3(t_list *temp, t_env *envr, t_vars *v, char **save)
{
	v->i++;
	if (temp->content[v->i] == '?')
	{
		v->var = ft_itoa(g_exit_status);
		*save = ft_strjoin(*save, v->var);
		v->i++;
		free(v->var);
	}
	else if (!temp->content[v->i] || !is_alpha_num(temp->content[v->i]))
		*save = ft_strjoin(*save, "$");
	else
	{
		v->j = v->i;
		while (temp->content[v->i] && is_alpha_num(temp->content[v->i]))
			v->i++;
		v->str = ft_substr(temp->content, v->j, v->i - v->j);
		if (ft_getenv(envr, v->str))
		{
			*save = ft_strjoin(*save, ft_getenv(envr, v->str));
			v->flag = 0;
		}
		if (v->flag == 1)
			*save = ft_strjoin(*save, "");
		free(v->str);
	}
}

void	expand_in_quotes_2(t_list *temp, t_env *envr, t_vars *v, char **save)
{
	while (temp->content[v->i])
	{
		v->flag = 1;
		if (temp->content[v->i] != '$')
		{
			v->j = v->i;
			while (temp->content[v->i] && temp->content[v->i] != '$')
				v->i++;
			v->command = ft_substr(temp->content, v->j, v->i - v->j);
			*save = ft_strjoin(*save, v->command);
			free(v->command);
		}
		else if (temp->content[v->i] == '$')
		{
			expand_in_quotes_3(temp, envr, v, save);
		}
	}
}

void	expand_in_quotes(t_list **list, t_env *envr, char *type)
{
	t_list	*temp;
	t_vars	v;
	char	*save;

	v.i = 0;
	save = NULL;
	temp = *list;
	v.flag = 1;
	while (temp)
	{
		v.i = 0;
		if (check_char(temp->content, '$') && (!ft_strcmp(type, temp->type)
				|| (!ft_strcmp("FILE", temp->type)
					&& temp->content[0] == '\"')))
		{
			expand_in_quotes_2(temp, envr, &v, &save);
			free(temp->content);
			temp->content = ft_strdup(save);
			free(save);
			save = NULL;
		}
		temp = temp->link;
	}
}

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
void	remove_dollar(t_list **list)
{
	t_list	*tmp;

	tmp = *list;
	while (tmp)
	{
		if ((!ft_strcmp(tmp->content, "$") && tmp->link && (!ft_strcmp(tmp->link->type,
						"DOUBLE_Q") || !ft_strcmp(tmp->link->type,
						"SINGLE_Q"))))
		{
			free(tmp->content);
			tmp->content = ft_strdup(tmp->link->content);
			tmp = del_node(list, tmp->link);
		}
		else if ((!ft_strcmp(tmp->content, "$") && tmp->link
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
		tmp = tmp->link;
	}
}

void expand_exit_status(t_list *tmp)
{
	t_vars v;

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
