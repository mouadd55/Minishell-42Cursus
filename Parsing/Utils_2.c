/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 16:58:02 by yonadry           #+#    #+#             */
/*   Updated: 2023/06/21 18:34:11 by moudrib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	split_string(t_vars *v, t_cmd *final_list, t_env **envr, int size)
{
	v->tmp1 = ft_split_input(v->str);
	lexer(&v->tmp1);
	if (size == 1)
		check_cmd(&v->tmp1, envr, final_list);
	ft_destroy_list(&v->tmp1);
	free(v->str);
	v->str = NULL;
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
		tmp1 = ft_lstlast(tmp)->prev;
		free(tmp1->link->type);
		free(tmp1->link->content);
		free(tmp1->link);
		tmp1->link = NULL;
	}
	else
		del_node_2(&tmp, &tmp1, del_node);
	return (tmp1);
}
