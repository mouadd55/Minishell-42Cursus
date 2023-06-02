/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_files.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yonadry <yonadry@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/29 11:57:03 by yonadry           #+#    #+#             */
/*   Updated: 2023/06/02 20:09:37 by yonadry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
		perror(file_name);
	return (fd);
}

char	*is_redir(t_list *list)
{
	if (!ft_strcmp(list->type, "APPEND") || !ft_strcmp(list->type, "OUTFILE")
		|| !ft_strcmp(list->type, "INFILE"))
		return (list->content);
	return (0);
}

char	*expand_in_here_doc(char *input, t_env **envr, int istrue)
{
	t_vars	v;

	v.str = NULL;
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

void	open_heredoc_3(t_vars *v, t_env **envr)
{
	v->fd = open(v->val, O_CREAT | O_RDWR | O_APPEND, 0777);
	while (1)
	{
		v->str = readline("Heredoc> ");
		if (!v->str || !ft_strcmp(v->str, v->tmp_str))
		{
			if (!v->str)
			{
				ft_printf_fd("\nMinishell: warning: ", 2);
				ft_printf_fd("here-document delimited by end-of-file (wanted '%s')\n",
					2, v->tmp_str);
			}
			close(v->fd);
			unlink(v->val);
			free(v->str);
			free(v->val);
			free(v->tmp_str);
			return ;
		}
		if (check_char(v->str, '$'))
			v->str = expand_in_here_doc(v->str, envr, v->flag);
		ft_printf_fd("%s\n", v->fd, v->str);
		free(v->str);
	}
}

void	open_heredoc_2(t_vars *v, t_env **envr)
{
	char	*save;

	v->tmp_str = ft_strdup(v->val);
	v->tmp_value = ft_strdup(".");
	v->tmp_key = ft_strdup(v->val);
	free(v->val);
	v->val = ft_strjoin(v->tmp_value, v->tmp_key);
	free(v->tmp_key);
	if (open(v->val, O_RDONLY) != -1)
	{
		save = ft_itoa(rand());
		v->val = ft_strjoin(v->val, save);
		free(save);
	}
	open_heredoc_3(v, envr);
}

void	open_heredoc(t_list *list, t_command **final_list, t_env **envr)
{
	t_vars	v;

	v.val = NULL;
	v.flag = 0;
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
	open_heredoc_2(&v, envr);
}

t_list	*open_the_file(t_vars *v, t_list *list, t_command **tmp)
{
	if (list && !ft_strcmp("FILE", list->type))
	{
		v->str = NULL;
		while (list && !ft_strcmp("FILE", list->type))
		{
			v->str = ft_strjoin(v->str, list->content);
			v->tmp1 = list;
			list = list->link;
		}
		list = v->tmp1;
		if (v->str)
		{
			v->fd = open_file(v->str, v->tmp_str);
			free(v->str);
			if (v->fd == -1)
			{
				(*tmp)->fd_in = -1;
				(*tmp)->fd_out = -1;
				while (list && ft_strcmp("PIPE", list->type))
					list = list->link;
				return (list);
			}
		}
	}
}

t_list	*open_var_error(t_vars *v, t_list *list, t_command **tmp)
{
	if (is_redir(list))
	{
		v->str = NULL;
		v->tmp_str = ft_strdup(is_redir(list));
		if (list->link && !ft_strcmp(list->link->type, "space"))
			list = list->link->link;
		else if (list->link)
			list = list->link;
		if (!ft_strcmp("VAR", list->type))
		{
			ft_printf_fd("minishell: %s: ambiguous redirect\n", 2,
					list->content);
			(*tmp)->fd_out = -1;
			(*tmp)->fd_in = -1;
			while (list && ft_strcmp("PIPE", list->type))
				list = list->link;
			free(v->tmp_str);
			v->tmp_str = NULL;
			return (list);
		}
		else
			list = open_the_file(v, list, tmp);
		free(v->tmp_str);
	}
	return (list);
}

void	open_files(t_list *list, t_command **final_list, t_env **envr)
{
	t_vars		v;
	t_command	*tmp;

	tmp = *final_list;
	v.str = NULL;
	while (list)
	{
		if (!ft_strcmp(list->type, "HEREDOC"))
			open_heredoc(list, final_list, envr);
		list = open_var_error(&v, list, &tmp);
		if (v.tmp_str && tmp)
		{
			if (!ft_strcmp(v.tmp_str, ">") || !ft_strcmp(v.tmp_str, ">>"))
				tmp->fd_out = v.fd;
			else if (!ft_strcmp(v.tmp_str, "<"))
				tmp->fd_in = v.fd;
			v.tmp_str = NULL;
			v.fd = -1;
		}
		if (!list)
			break ;
		list = list->link;
	}
}
