/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 16:45:14 by moudrib           #+#    #+#             */
/*   Updated: 2023/06/02 15:19:15 by moudrib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_list	*ft_lstnew(char *content)
{
	t_list	*head;

	head = (t_list *)malloc(sizeof(t_list));
	if (!head)
		return (NULL);
	head->content = content;
	head->type = NULL;
	head->link = NULL;
	head->prev = NULL;
	return (head);
}

t_list	*ft_lstlast(t_list *head)
{
	if (!head)
		return (NULL);
	while (head)
	{
		if (head->link == NULL)
			return (head);
		head = head->link;
	}
	return (NULL);
}

void	ft_lstadd_back(t_list **head, t_list *new)
{
	t_list	*tmp;

	if (!*head || !head)
		*head = new;
	else
	{
		tmp = ft_lstlast(*head);
		tmp->link = new;
		new->prev = tmp;
	}
}

void	*ft_destroy_list(t_list **head)
{
	t_list	*tmp;

	if (!head || !*head)
		return (0);
	tmp = *head;
	while (tmp)
	{
		tmp = (*head)->link;
		free((*head)->content);
		if ((*head)->type)
			free((*head)->type);
		free(*head);
		(*head) = tmp;
	}
	return (0);
}

int	ft_lstsize(t_list *lst)
{
	int	counter;

	counter = 0;
	while (lst)
	{
		counter++;
		lst = lst->link;
	}
	return (counter);
}
