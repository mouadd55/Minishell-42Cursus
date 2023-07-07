/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 16:58:02 by yonadry           #+#    #+#             */
/*   Updated: 2023/07/07 13:43:47 by moudrib          ###   ########.fr       */
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

char	*filename_gen(char *d)
{
	int			a;
	char		*name;
	char		*gen;
	char		*save;

	a = 0;
	name = NULL;
	save = ft_strdup(d);
	while (1)
	{
		d = ft_strdup(save);
		gen = ft_itoa(a);
		name = ft_strjoin(d, gen);
		if (access(name, F_OK))
		{
			free(save);
			free(gen);
			return (name);
		}
		free(gen);
		a++;
	}
	free (save);
}

int	open_file(char *file_name, char *type)
{
	int	fd;

	if (!ft_strcmp(type, ">"))
		fd = open(file_name, O_CREAT | O_RDWR | O_TRUNC, 0777);
	if (!ft_strcmp(type, ">>"))
		fd = open(file_name, O_CREAT | O_RDWR | O_APPEND, 0777);
	if (!ft_strcmp(type, "<"))
		fd = open(file_name, O_RDONLY, 0777);
	if (fd == -1)
	{
		g_exit_status = 1;
		perror(file_name);
	}
	return (fd);
}
