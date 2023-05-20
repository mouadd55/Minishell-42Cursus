/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_for_export.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 21:55:46 by moudrib           #+#    #+#             */
/*   Updated: 2023/05/20 15:36:29 by moudrib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_some_variables(t_vars *v)
{
	free(v->val);
	v->val = NULL;
	free(v->var);
	v->var = NULL;
}

t_list	*plus_cases(t_list *tmp)
{
	if (tmp && tmp->content[0] == '+' && check_type(tmp->type))
	{
		free(tmp->type);
		if ((tmp->prev->content[0] != ' '
				&& tmp->link && tmp->link->content[0] == '='))
			tmp->type = ft_strdup("PLUS");
		else
		{
			tmp->type = ft_strdup("VAR");
			tmp = tmp->link;
			while (tmp && tmp->content[0] != ' ' && check_type(tmp->type))
			{
				if (tmp->type[0] != 's')
				{
					free(tmp->type);
					tmp->type = ft_strdup("VAR");
				}
				tmp = tmp->link;
			}
		}
		if (tmp && tmp->type[0] != 's' && check_type(tmp->type))
			tmp = tmp->link;
	}
	return (tmp);
}

t_list	*equal_cases(t_list *tmp)
{
	if (tmp && tmp->content[0] == '=' && check_type(tmp->type))
	{
		if (tmp->prev->type[0] == 's')
		{
			while (tmp && tmp->type[0] != 's' && check_type(tmp->type))
			{
				free(tmp->type);
				tmp->type = ft_strdup("VAR");
				tmp = tmp->link;
			}
		}
		else
		{
			free(tmp->type);
			tmp->type = ft_strdup("EQUAL");
			tmp = tmp->link;
		}
	}
	while (tmp && tmp->type[0] != 's' && check_type(tmp->type))
	{
		free(tmp->type);
		tmp->type = ft_strdup("value");
		tmp = tmp->link;
	}
	return (tmp);
}

void	lexer_for_export(t_list **list)
{
	t_list	*tmp;

	tmp = *list;
	while (tmp && check_type(tmp->type))
	{
		while (tmp && (tmp->content[0] != '=' && tmp->content[0] != '+')
			&& check_type(tmp->type))
		{
			if (tmp->type[0] != 's')
			{
				free(tmp->type);
				tmp->type = ft_strdup("VAR");
			}
			tmp = tmp->link;
		}
		tmp = plus_cases(tmp);
		tmp = equal_cases(tmp);
		if (tmp && tmp->type[0] == 's' && check_type(tmp->type))
			tmp = tmp->link;
	}
}
