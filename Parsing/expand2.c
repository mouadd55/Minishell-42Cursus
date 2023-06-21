/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yonadry <yonadry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 15:36:46 by yonadry           #+#    #+#             */
/*   Updated: 2023/06/21 16:56:34 by yonadry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	remove_quotes(t_list **list)
{
	t_vars	v;

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

void	expand_in_quotes_3(t_list *temp, t_env *envr, t_vars *v, char **save)
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
