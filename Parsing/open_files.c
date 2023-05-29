/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_files.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yonadry <yonadry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/29 11:57:03 by yonadry           #+#    #+#             */
/*   Updated: 2023/05/29 12:53:18 by yonadry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	open_file(char *file_name, char *type)
{
	int fd = 1;

	if (!ft_strcmp(type, ">"))
	    fd = open(file_name, O_CREAT | O_RDWR | O_TRUNC, 0777);
	if (!ft_strcmp(type, ">>"))
	    fd = open(file_name, O_CREAT | O_RDWR | O_APPEND, 0777);
	if (!ft_strcmp(type, "<"))
	    fd = open(file_name, O_RDONLY, 0775);
	if (fd == -1)
	{
		ft_printf_fd("Minishell: : Permission denied\n", 2, file_name);
		return (-1);
	}
	return (fd);
}

char *is_redir(t_list *list)
{
	if (!ft_strcmp(list->type, "APPEND")
		|| !ft_strcmp(list->type, "OUTFILE"))
		return (list->content);
	return (0);
}
 
int handle_file(t_list *list, t_command **f_lst,char *file_name, char *type)
{
	int fd;
	while (list)
	{
		if (!ft_strcmp("VAR", list->type))
		{
			ft_printf_fd("Minishell: %s: ambiguous redirect", 2,list->content);
			return(-1);
		}
		if (!ft_strcmp("FILE", list->type))
			file_name = ft_strjoin(file_name, list->content);
		if (list->type[0] == 's' || !list->link
			|| (list->link))
		{
			fd = open_file(file_name, type);
			if (fd == -1)
				return (-1);
		}
		list = list->link;
	}
	printf("|type : |%s| fd : %d|\n", type,fd);
	if (list && (!ft_strcmp(type, ">>") || !ft_strcmp(type, ">")))
	{
		(*f_lst)->fd_out = fd;
	}
	else if (list && !ft_strcmp(type, "<"))
		(*f_lst)->fd_in = fd;
	return (fd);
}

int open_files(t_list *list, t_command **final_list)
{
	char *file_name;
	char *type;
	int fd;

	fd = 1;
	while (list)
	{
		if (is_redir(list))
		{
			file_name = NULL;
			type = ft_strdup(is_redir(list));
			if (list->link && !ft_strcmp(list->link->type, "space"))
				list = list->link->link;
			else if (list->link)
				list = list->link;
			handle_file(list,final_list ,file_name, type);
			if (fd == -1)
				return (-1);
		}
		if (!list)
			break;
		list = list->link;
	}
	return (fd);
}