/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 20:32:32 by yonadry           #+#    #+#             */
/*   Updated: 2023/05/31 15:32:51 by moudrib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_space(int c)
{
	if ((c >= 9 && c <= 13) || c == 32)
		return (1);
	return (0);
}

void	switch_space(char *input, int x)
{
	t_vars	v;

	v.i = 0;
	while (input[v.i] && x)
	{
		if (input[v.i] == '\'')
		{
			while (input[++v.i] && input[v.i] != '\'')
				if (is_space(input[v.i]))
					input[v.i] *= -1;
		}
		else if (input[v.i] == '\"')
		{
			while (input[++v.i] && input[v.i] != '\"')
				if (is_space(input[v.i]))
					input[v.i] *= -1;
		}
		v.i++;
	}
	while (input[v.i] && !x)
	{
		if (input[v.i] < 0)
			input[v.i] *= -1;
		v.i++;
	}
}

char	is_quote(char input)
{
	if (input == '\"' || input == '\'')
		return (input);
	return (0);
}

char	is_special(char c)
{
	if (check_char("()=+|><$", c))
		return (c);
	return (0);
}

void	handle_redirect(t_list **lst, t_vars *v)
{
	if ((v->arr[v->i][v->j] == '>' && v->arr[v->i][v->j + 1] == '>')
		|| (v->arr[v->i][v->j] == '<' && v->arr[v->i][v->j + 1] == '<'))
	{
		ft_lstadd_back(lst, ft_lstnew(ft_substr(&v->arr[v->i][v->j], 0, 2)));
		v->j += 2;
	}
	else
	{
		ft_lstadd_back(lst, ft_lstnew(ft_substr(&v->arr[v->i][v->j], 0, 1)));
		v->j++;
	}
}

void	handle_quotes(t_vars *v, t_list **lst, char c)
{
	if (v->arr[v->i][v->j] && !is_special(v->arr[v->i][v->j]))
	{
		if (v->arr[v->i][v->j] && is_quote(v->arr[v->i][v->j]))
		{
			v->start = v->j;
			v->j++;
			while (v->arr[v->i][v->j] && v->arr[v->i][v->j] != c)
				v->j++;
			ft_lstadd_back(lst, ft_lstnew(ft_substr(v->arr[v->i], v->start, v->j
						- v->start + 1)));
			v->j++;
		}
		else if (v->arr[v->i][v->j] && !is_quote(v->arr[v->i][v->j]))
		{
			v->start = v->j;
			while (v->arr[v->i][v->j] && !is_quote(v->arr[v->i][v->j])
				&& !is_special(v->arr[v->i][v->j]))
				v->j++;
			ft_lstadd_back(lst, ft_lstnew(ft_substr(v->arr[v->i], v->start, v->j
						- v->start)));
		}
	}
	if (v->arr[v->i][v->j] && is_special(v->arr[v->i][v->j]))
		handle_redirect(lst, v);
}

static void	split_input(t_list **lst, t_vars *v)
{
	if (v->arr[v->i] && (check_char(v->arr[v->i], '\"')
			|| check_char(v->arr[v->i], '\'')))
	{
		while (v->arr[v->i][v->j])
			handle_quotes(v, lst, v->arr[v->i][v->j]);
	}
	else
	{
		while (v->arr[v->i][v->j])
		{
			if (v->arr[v->i][v->j] && is_special(v->arr[v->i][v->j]))
				handle_redirect(lst, v);
			else
			{
				v->start = v->j;
				while (v->arr[v->i][v->j] && !is_special(v->arr[v->i][v->j]))
					v->j++;
				v->end = v->j - v->start;
				ft_lstadd_back(lst, ft_lstnew(ft_substr(v->arr[v->i], v->start,
							v->end)));
			}
		}
	}
}

t_list	*ft_split_input(char *input)
{
	t_list	*lst;
	t_vars	v;
	t_list	*tmp;

	v.i = 0;
	v.j = 0;
	lst = NULL;
	switch_space(input, 1);
	v.arr = ft_split(input, " \t");
	while (v.arr[v.i])
	{
		v.j = 0;
		split_input(&lst, &v);
		v.i++;
		if (v.arr[v.i])
			ft_lstadd_back(&lst, ft_lstnew(ft_strdup(" ")));
	}
	ft_free_arr(v.arr);
	tmp = lst;
	while (tmp)
	{
		switch_space(tmp->content, 0);
		tmp = tmp->link;
	}
	return (lst);
}
