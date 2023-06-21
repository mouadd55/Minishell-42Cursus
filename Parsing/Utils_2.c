/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yonadry <yonadry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 16:58:02 by yonadry           #+#    #+#             */
/*   Updated: 2023/06/21 17:08:48 by yonadry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
