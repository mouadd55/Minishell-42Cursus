/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/20 17:10:36 by yonadry           #+#    #+#             */
/*   Updated: 2023/06/03 15:12:59 by moudrib          ###   ########.fr       */
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
	t_list	*temp;

	temp = *list;
	while (temp)
	{
		if (!ft_strcmp("DOUBLE_Q", temp->type) || !ft_strcmp("SINGLE_Q",
				temp->type) || (!ft_strcmp("FILE", temp->type)
					&& (temp->content[0] == '\"' || temp->content[0] == '\'')))
		{
			free(temp->content);
			if (ft_strlen(temp->content) == 2)
				temp->content = ft_strdup("");
			else
				temp->content = ft_substr(temp->content, 1,
						ft_strlen(temp->content) - 2);
		}
		temp = temp->link;
	}
}

int	is_alpha_num(char c)
{
	if (ft_isalpha(c) || check_char("0123456789", c))
		return (1);
	return (0);
}

void	expand_in_quotes_3(t_list *temp, t_env *envr, t_vars *v, char **save)
{
	t_env	*env;

	v->i++;
	if (!temp->content[v->i] || !is_alpha_num(temp->content[v->i]))
		*save = ft_strjoin(*save, "$");
	else
	{
		v->j = v->i;
		while (temp->content[v->i] && is_alpha_num(temp->content[v->i]))
			v->i++;
		v->str = ft_substr(temp->content, v->j, v->i - v->j);
		env = envr;
		while (env)
		{
			if (!ft_strcmp(env->key, v->str))
			{
				*save = ft_strjoin(*save, env->value);
				v->flag = 0;
				break ;
			}
			env = env->link;
		}
		if (v->flag == 1)
			*save = ft_strjoin(*save, "");
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
			*save = ft_strjoin(*save, ft_substr(temp->content, v->j, v->i
						- v->j));
		}
		else if (temp->content[v->i] == '$')
			expand_in_quotes_3(temp, envr, v, save);
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
		if (check_char(temp->content, '$') && (!ft_strcmp(type,
				temp->type) || (!ft_strcmp("FILE", temp->type)
				&& temp->content[0] == '\"')))
		{
			expand_in_quotes_2(temp, envr, &v, &save);
			temp->content = ft_strdup(save);
			save = NULL;
		}
		temp = temp->link;
	}
}

void	expand_var_2(t_list **list, t_list **tmp, t_env *envr, t_vars *v)
{
	t_env	*tmp1;

	tmp1 = envr;
	if ((*tmp) && (*tmp)->content[0] == '$' && (*tmp)->link)
	{
		(*tmp) = (*tmp)->link;
		while (ft_isalpha((*tmp)->content[v->i]) || check_char("0123456789",
				(*tmp)->content[v->i]))
			v->i++;
		v->str = ft_substr((*tmp)->content, 0, v->i);
		tmp1 = envr;
		while (tmp1)
		{
			if (tmp1 && !ft_strcmp(tmp1->key, v->str))
			{
				(*tmp)->prev->content = ft_strdup(tmp1->value);
				(*tmp)->prev->content = ft_strjoin((*tmp)->prev->content,
						ft_strdup(&(*tmp)->content[v->i]));
				*tmp = del_node(list, *tmp);
				break ;
			}
			tmp1 = tmp1->link;
		}
	}
}
void remove_dollar(t_list **list)
{
	t_list *tmp;

	tmp = *list;
	while (tmp)
	{
		if (tmp && (tmp->content[0] == '$' && tmp->link
				&& tmp->link->content[0] != 32))
		{
			tmp->content  = ft_strjoin(tmp->content, tmp->link->content);
			free(tmp->type);
			if (!ft_strcmp(tmp->prev->content, "<<")
				|| !ft_strcmp(tmp->prev->prev->content, "<<"))
				tmp->type = ft_strdup("DELIMITER");
			else
				tmp->type = ft_strdup("VAR");
			if (tmp->link)
				tmp = del_node(list, tmp->link);
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
	while (tmp && ft_strcmp(tmp->type, "DELIMITER"))
	{
		expand_var_2(list, &tmp, envr, &v);
		tmp = tmp->link;
	}
	expand_in_quotes(list, envr, "DOUBLE_Q");
	if (rm_quotes)
		remove_quotes(list);
	tmp = *list;
	remove_dollar(&tmp);
}
