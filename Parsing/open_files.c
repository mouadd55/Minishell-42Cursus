/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_files.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yonadry <yonadry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/29 11:57:03 by yonadry           #+#    #+#             */
/*   Updated: 2023/05/31 14:19:49 by yonadry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	open_file(char *file_name, char *type)
{
	int fd;

	if (!ft_strcmp(type, ">"))
	    fd = open(file_name, O_CREAT | O_RDWR | O_TRUNC, 0777);
	if (!ft_strcmp(type, ">>"))
	    fd = open(file_name, O_CREAT | O_RDWR | O_APPEND, 0777);
	if (!ft_strcmp(type, "<"))
	    fd = open(file_name, O_RDONLY, 0777);
	if (fd == -1)
		perror(file_name);
	return (fd);
}

char *is_redir(t_list *list)
{
	if (!ft_strcmp(list->type, "APPEND")
		|| !ft_strcmp(list->type, "OUTFILE")
		|| !ft_strcmp(list->type, "INFILE"))
		return (list->content);
	return (0);
}
 
// t_list *handle_file(t_list *list, t_command *f_lst, char *file_name, char *type)
// {
// 	int fd;

// 	while (list)
// 	{
// 		if (!ft_strcmp("FILE", list->type))
// 			file_name = ft_strjoin(file_name, list->content);
// 		if (file_name && (list->type[0] == 's' || (!list->link)))
// 		{
// 			fd = open_file(file_name, type);
// 			file_name = NULL;
// 		}
// 		list = list->link;
// 	}
// 	if ((!ft_strcmp(type, ">>") || !ft_strcmp(type, ">")))
// 		f_lst->fd_out = fd;
// 	else if (!ft_strcmp(type, "<"))
// 		f_lst->fd_in = fd;
// 	if (list && !ft_strcmp(list->content, "|"))
// 		return (list);
// 	return (list);
// }

void open_files(t_list *list, t_command **final_list)
{
	char *file_name;
	char *type = NULL;
	t_command *tmp;
	t_list *temp;
	int fd;

	tmp = *final_list;
	while (list)
	{
		if (is_redir(list))
		{
			type = ft_strdup(is_redir(list));
			if (list->link && !ft_strcmp(list->link->type, "space"))
				list = list->link->link;
			else if (list->link)
				list = list->link;
			if (!ft_strcmp("VAR", list->type))
			{
				ft_printf_fd("Minishell: %s: ambiguous redirect\n", 2,list->content);
				tmp->fd_out = -1;
				tmp->fd_in = -1;
				while (list && ft_strcmp("PIPE", list->type))
					list = list->link;
				if (list && list->link)
					list = list->link;
				if (tmp->link)
					tmp = tmp->link;
				type = NULL;
			}
			else if (list && !ft_strcmp("FILE", list->type))
			{
				file_name = NULL;
				while (list && !ft_strcmp("FILE", list->type))
				{
					file_name = ft_strjoin(file_name, list->content);
					temp = list;
					list = list->link;
				}
				list = temp;
				if (file_name)
					fd = open_file(file_name, type);
			}
		}
		if (type && tmp)
		{
			if (!ft_strcmp(type, ">") || !ft_strcmp(type, ">>"))
				tmp->fd_out = fd;
			else if (!ft_strcmp(type, "<"))
				tmp->fd_in = fd;
			type = NULL;
			fd = -1;
		}
		if (list && !ft_strcmp(list->type , "PIPE"))
			tmp = tmp->link;
		if (!list)
			break;
		list = list->link;
	}
}