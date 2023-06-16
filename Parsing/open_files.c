/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_files.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/29 11:57:03 by yonadry           #+#    #+#             */
/*   Updated: 2023/06/16 13:50:09 by moudrib          ###   ########.fr       */
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
		if (ft_strcmp(v.tmp1->type, "VAR"))
			v.str = ft_strjoin(v.str, v.tmp1->content);
		v.tmp1 = v.tmp1->link;
	}
	ft_destroy_list(&v.tmp1);
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
			// close(v->fd);
			free(v->str);
			free(v->val);
			free(v->tmp_str);
			return ;
		}
		if (check_char(v->str, '$'))
			v->str = expand_in_here_doc(v->str, envr, v->flag);
		if(v->str)
			ft_printf("%s\n", v->fd, v->str);
		free(v->str);
	}
}

void	open_heredoc_2(t_vars *v, t_env **envr, t_vars *p)
{
	char	*save;

	v->tmp_str = ft_strdup(v->val);
	v->tmp_value = ft_strdup(".");
	v->tmp_key = ft_strdup(v->val);
	free(v->val);
	v->val = ft_strjoin(v->tmp_value, v->tmp_key);
	free(v->tmp_key);
	if (!access(v->val, F_OK))
	{
		save = ft_itoa(rand());
		v->val = ft_strjoin(v->val, save);
		free(save);
	}
	p->val = ft_strdup(v->val);
	open_heredoc_3(v, envr);
}

void	open_heredoc(t_vars	*p, t_list *list, t_env **envr)
{
	t_vars	v;

	v.val = NULL;
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
			free(list->content);
			list->content = ft_strtrim(list->content, v.var);
			free(v.var);
		}
		v.val = ft_strjoin(v.val, list->content);
		list = list->link;
	}
	open_heredoc_2(&v, envr, p);
	p->fd = v.fd;
}

int var_redirect(t_list *list, t_vars *v)
{
	v->tmp_value = ft_strdup(is_redir(list));
	if (list->link && !ft_strcmp(list->link->type, "space"))
		list = list->link->link;
	else if (list->link)
		list = list->link;
	v->str = NULL;
	if (!ft_strcmp("VAR", list->type))
	{
		ft_printf("minishell: %s: ambiguous redirect\n", 2, list->content);
		return (1);
	}
	else if (!ft_strcmp("FILE", list->type))
	{
		while (list && !ft_strcmp("FILE", list->type))
		{
			v->str = ft_strjoin(v->str, list->content);
			list = list->link;
		}
		v->fd = open_file(v->str, v->tmp_value);
		if (v->fd == -1)
			return (1);
		free(v->str);
	}
	return (0);
}

t_cmd *add_fd(t_vars *v, t_cmd *tmp, t_list *list)
{
	if (v->tmp_value && tmp)
	{
		if (!ft_strcmp(v->tmp_value, ">") || !ft_strcmp(v->tmp_value, ">>"))
			tmp->fd_out = v->fd;
		else if (!ft_strcmp(v->tmp_value, "<") || !ft_strcmp(v->tmp_value, "<<"))
		{
			tmp->fd_in = v->fd;
			if (!ft_strcmp(v->tmp_value, "<<"))
			{
				tmp->file_name = ft_strdup(v->command);
				free(v->command);
			}
		}
		free(v->tmp_value);
		v->tmp_value = NULL;
	}
	if (list && !ft_strcmp(list->type , "PIPE")
		&& tmp && tmp->link)
		tmp = tmp->link;
	return (tmp);
}

void if_heredoce(t_vars *v, t_cmd *tmp, t_env **envr, t_vars *p)
{
	if(tmp->file_name)
	{
		if (tmp->fd_in >= 3)
			close(tmp->fd_in);
		unlink(tmp->file_name);
		free(tmp->file_name);
		tmp->file_name = NULL;
	}
	v->tmp_value = ft_strdup("<<");
	open_heredoc(p, v->tmp1, envr);
	v->fd = p->fd;
	v->command = ft_strdup(p->val);
	free(p->val);
}

t_list *if_redirect(t_cmd *tmp, t_vars *v, t_env **envr, t_vars *p)
{
	tmp->fd_in = -1;
	tmp->fd_out = -1;
	while (v->tmp1 && ft_strcmp(v->tmp1->type, "PIPE"))
	{
		if(!ft_strcmp(v->tmp1->type, "HEREDOC"))
		{
			if_heredoce(v, tmp, envr, p);
			tmp->file_name = ft_strdup(v->command);
			free(v->command);
		}
		v->tmp1 = v->tmp1->link;
	}
	if (tmp && tmp->link)
		tmp = tmp->link;
	free(v->str);
	v->tmp_value = NULL;
	return (v->tmp1);
}

void  open_files(t_list *list, t_cmd *tmp, t_env **envr)
{
	t_vars v;
	t_vars p;
	v.command = NULL;

	v.tmp_value = NULL;
	v.str = NULL;
	/*v.fd = 0; Valgrind: Conditional jump or move depends on uninitialised value(s) */
	while (list)
	{
		v.tmp1 = list;
		if (!ft_strcmp(list->type, "HEREDOC"))
			if_heredoce(&v, tmp, envr, &p);
		else if (is_redir(list) && var_redirect(list, &v))
		{
			free(v.tmp_value);
			list = if_redirect(tmp, &v, envr, &p);
		}
		if (v.fd >= 3)
			tmp = add_fd(&v, tmp, list);
		if (!list)
			break;
		list = list->link;
	}
}
