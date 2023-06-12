/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   final_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 20:43:41 by moudrib           #+#    #+#             */
/*   Updated: 2023/06/12 16:27:50 by moudrib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

int	count_cmds(t_list *list)
{
	int	commands;

	commands = 1;
	while (list)
	{
		if (!ft_strcmp(list->type, "PIPE"))
			commands++;
		list = list->link;
	}
	return (commands);
}

int	lstsize(t_cmd *lst)
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

char	*spaces_in_quotes_utils(char *str, int idx)
{
	int		i;
	char	*updated_str;

	i = 0;
	if (!str)
		return (0);
	updated_str = malloc(sizeof(char) * (ft_strlen(str) + 1));
	if (!updated_str)
		return (0);
	while (str[i])
	{
		if (is_space(str[i]) && idx == 1)
			updated_str[i] = str[i] * (-1);
		else if (str[i] < 0 && idx == 0)
			updated_str[i] = str[i] * (-1);
		else
			updated_str[i] = str[i];
		i++;
	}
	updated_str[i] = '\0';
	if (idx == 0)
		free(str);
	return (updated_str);
}

void	spaces_in_quotes(t_cmd **final_list)
{
	int			i;
	t_cmd	*tmp;

	tmp = *final_list;
	while (tmp)
	{
		i = -1;
		while (tmp->cmd && tmp->cmd[++i])
			tmp->cmd[i] = spaces_in_quotes_utils(tmp->cmd[i], 0);
		tmp = tmp->link;
	}
}

void	create_final_list(t_list *list, t_cmd **final_list)
{
	t_vars	v;

	v.tmp = NULL;
	v.str = NULL;
	while (list)
	{
		while (list && ft_strcmp(list->type, "PIPE"))
		{
			if (list->type[0] == 'S' || !ft_strcmp(list->type, "DOUBLE_Q"))
			{
				v.tmp = spaces_in_quotes_utils(list->content, 1);
				v.str = ft_strjoin(v.str, v.tmp);
				free(v.tmp);
				v.tmp = NULL;
			}
			if (list->type[0] == 'C' || !ft_strcmp(list->type, "FLAG"))
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
	spaces_in_quotes(final_list);
}
