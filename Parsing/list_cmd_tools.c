/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_cmd_tools.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 09:53:13 by moudrib           #+#    #+#             */
/*   Updated: 2023/06/13 09:54:52 by moudrib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_cmd	*lstnew_final(char **command, int fd_in, int fd_out)
{
	t_cmd	*head;

	head = (t_cmd *)malloc(sizeof(t_cmd));
	if (!head)
		return (NULL);
	head->cmd = command;
	head->fd_in = fd_in;
	head->fd_out = fd_out;
	head->link = NULL;
	head->prev = NULL;
	head->file_name = NULL;
	return (head);
}

t_cmd	*lstlast_final(t_cmd *head)
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

void	lstadd_back_final(t_cmd **head, t_cmd *new)
{
	t_cmd	*tmp;

	if (!*head || !head)
		*head = new;
	else
	{
		tmp = lstlast_final(*head);
		tmp->link = new;
		new->prev = tmp;
	}
}

int	lstsize_cmd(t_cmd *lst)
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

void	*ft_destroy_final(t_cmd **head)
{
	t_cmd	*tmp;

	if (!head || !*head)
		return (0);
	tmp = *head;
	while (tmp)
	{
		tmp = (*head)->link;
		if ((*head)->cmd)
			ft_free_arr((*head)->cmd);
		if ((*head)->fd_in >= 3)
			close((*head)->fd_in);
		if ((*head)->fd_out >= 3)
			close((*head)->fd_out);
		if ((*head)->file_name)
		{
			unlink((*head)->file_name);
			free((*head)->file_name);
		}
		free(*head);
		(*head) = tmp;
	}
	return (0);
}
