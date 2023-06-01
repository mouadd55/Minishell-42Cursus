/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_files.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yonadry <yonadry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/29 11:57:03 by yonadry           #+#    #+#             */
/*   Updated: 2023/05/31 22:47:38 by yonadry          ###   ########.fr       */
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
 
char *expand_in_here_doc(char *input, t_env **envr, int istrue)
{
	t_vars v;

	v.str = NULL;
	if (!input)
		return;
	v.tmp1 = ft_split_input(input);
	lexer(&v.tmp1);
	if (!istrue)
	{
		expand_in_quotes(&v.tmp1, *envr, "SINGLE_Q");
		expand_var(&v.tmp1, *envr, 0);
	}
	while (v.tmp1)
	{
		v.str = ft_strjoin(v.str, v.tmp1->content);
		v.tmp1 = v.tmp1->link;
	}
	return (v.str);
}

void open_heredoc(t_list *list, t_command **final_list, t_env **envr)
{
	t_vars v;

	v.val = NULL;
	v.flag = 0;
	while (list)
	{
		if (!ft_strcmp(list->type, "HEREDOC"))
		{
			if (list->link && !ft_strcmp(list->link->type, "space"))
				list = list->link->link;
			else if (list->link)
				list = list->link;
			while (list && !ft_strcmp(list->type, "DELIMITER"))
			{
				if (list->content[0] == '\'' || list->content[0] == '\"')
				{
					if (list->content[0] == '\'')
						v.var = ft_strdup("\'");
					else if (list->content[0] == '\"')
						v.var = ft_strdup("\"");
					list->content = ft_strtrim(list->content, v.var);
					v.flag = 1;
					free(v.var);
				}
				v.val = ft_strjoin(v.val, list->content);
				list = list->link;
			}
			v.fd = open(".heredoc", O_CREAT | O_RDWR | O_APPEND, 0777);
			while (1)
			{
				v.str = readline("Heredoc> ");
				if (!v.str || !ft_strcmp(v.str, v.val))
				{
					if (!v.str)
						ft_printf_fd("Minishell: warning: here-document delimited by end-of-file (wanted '%s')", 2, v.val);
					free(v.str);
					free(v.val);
					close(v.fd);
					break;
				}
				if (check_char(v.str, '$'))
					v.str = expand_in_here_doc(v.str, envr, v.flag);
				ft_printf_fd("%s\n", v.fd, v.str);
				free(v.str);
			}
		}
		unlink(".heredoc");
		if (!list)
			break;
		list = list->link;
	}
}

void open_files(t_list *list, t_command **final_list, t_env **envr)
{
	t_vars v;
	char *type;
	t_command *tmp;

	type = NULL;
	tmp = *final_list;
	v.str = NULL;
	open_heredoc(list, final_list, envr);
	// while (list)
	// {
	// 	if (is_redir(list))
	// 	{
	// 		type = ft_strdup(is_redir(list));
	// 		if (list->link && !ft_strcmp(list->link->type, "space"))
	// 			list = list->link->link;
	// 		else if (list->link)
	// 			list = list->link;
	// 		if (!ft_strcmp("VAR", list->type))
	// 		{
	// 			ft_printf_fd("minishell: %s: ambiguous redirect\n", 2,list->content);
	// 			tmp->fd_out = -1;
	// 			tmp->fd_in = -1;
	// 			while (list && ft_strcmp("PIPE", list->type))
	// 				list = list->link;
	// 			if (list && list->link)
	// 				list = list->link;
	// 			if (tmp->link)
	// 				tmp = tmp->link;
	// 			type = NULL;
	// 		}
	// 		else if (list && !ft_strcmp("FILE", list->type))
	// 		{
	// 			v.str = NULL;
	// 			while (list && !ft_strcmp("FILE", list->type))
	// 			{
	// 				v.str = ft_strjoin(v.str, list->content);
	// 				v.tmp1 = list;
	// 				list = list->link;
	// 			}
	// 			list = v.tmp1;
	// 			if (v.str)
	// 				v.fd = open_file(v.str, type);
	// 		}
	// 	}
	// 	if (type && tmp)
	// 	{
	// 		if (!ft_strcmp(type, ">") || !ft_strcmp(type, ">>"))
	// 			tmp->fd_out = v.fd;
	// 		else if (!ft_strcmp(type, "<"))
	// 			tmp->fd_in = v.fd;
	// 		type = NULL;
	// 		v.fd = -1;
	// 	}
	// 	if (list && !ft_strcmp(list->type , "PIPE"))
	// 		tmp = tmp->link;
	// 	if (!list)
	// 		break;
	// 	list = list->link;
	// }
}