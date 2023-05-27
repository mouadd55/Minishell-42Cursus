/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   final_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 20:43:41 by moudrib           #+#    #+#             */
/*   Updated: 2023/05/26 21:33:04 by moudrib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_command	*lstnew_final(char **command, int fd_in, int fd_out)
{
	t_command	*head;

	head = (t_command *)malloc(sizeof(t_command));
	if (!head)
		return (NULL);
	head->cmd = command;
    head->fd_in = fd_in;
    head->fd_out = fd_out;
	head->link = NULL;
	head->prev = NULL;
	return (head);
}

t_command	*lstlast_final(t_command *head)
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

void	lstadd_back_final(t_command **head, t_command *new)
{
	t_command	*tmp;

	if (!*head || !head)
		*head = new;
	else
	{
		tmp = lstlast_final(*head);
		tmp->link = new;
		new->prev = tmp;
	}
}

int count_commands(t_list *list)
{
    int commands;

    commands = 1;
    while (list)
    {
        if (!ft_strcmp(list->type, "PIPE"))
            commands++;
        list = list->link;
    }
    return (commands);
}


int	lstsize(t_command *lst)
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


void    create_final_list(t_list *list, t_command **final_list)
{
    t_vars	v;
    int		commands;

    v.str = NULL;
    commands = count_commands(list);
    while (list)
    {
        while (list && ft_strcmp(list->type, "PIPE"))
        {
            if (list->type[0] == 'C' || list->type[0] == 'D' 
                || list->type[0] == 'S' ||  !ft_strcmp(list->type, "FLAG"))
                v.str = ft_strjoin(v.str, list->content);
            else if (list->type[0] == 's')
                v.str = ft_strjoin(v.str, " ");
            list = list->link;
        }
        lstadd_back_final(final_list, lstnew_final(ft_split(v.str, " "), 0, 1));
        free(v.str);
        v.str = NULL;
        if (list)
            list = list->link;
    }
}
