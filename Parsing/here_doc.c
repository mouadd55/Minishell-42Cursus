/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 15:31:54 by yonadry           #+#    #+#             */
/*   Updated: 2023/06/22 09:12:42 by moudrib          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*expand_in_here_doc(char *input, t_env **envr, int istrue)
{
	t_vars	v;

	v.str = NULL;
	v.tmp1 = ft_split_input(input);
	free(input);
	v.tmp2 = v.tmp1;
	lexer(&v.tmp1);
	if (istrue)
	{
		expand_in_quotes(&v.tmp1, *envr, "SINGLE_Q");
		expand_var(&v.tmp1, *envr, 0);
	}
	while (v.tmp1 && ft_strcmp(v.tmp1->type, "VAR"))
	{
		v.str = ft_strjoin(v.str, v.tmp1->content);
		v.tmp1 = v.tmp1->link;
	}
	ft_destroy_list(&v.tmp2);
	return (v.str);
}

void	open_heredoc_3(t_vars *v, t_env **envr)
{
	v->fd = open(v->val, O_CREAT | O_RDWR | O_APPEND, 0777);
	signal(SIGINT, &sig_hand);
	v->str = readline("Heredoc> ");
	while (v->str)
	{
		if (!v->str || !ft_strcmp(v->str, v->tmp_str)
			|| g_exit_status == -1)
		{
			free(v->str);
			free(v->val);
			free(v->tmp_str);
			return ;
		}
		if (check_char(v->str, '$'))
			v->str = expand_in_here_doc(v->str, envr, v->flag);
		if (v->str)
			ft_printf("%s\n", v->fd, v->str);
		free(v->str);
		v->str = readline("Heredoc> ");
	}
	free(v->tmp_str);
	free(v->val);
}

int	open_heredoc_2(t_vars *v, t_env **envr, t_vars *p)
{
	v->tmp_str = ft_strdup(v->val);
	v->tmp_value = ft_strdup(".");
	if (!ft_strlen(v->val))
		v->tmp_key = ft_strdup("tmp");
	else
		v->tmp_key = ft_strdup(v->val);
	free(v->val);
	v->val = ft_strjoin(v->tmp_value, v->tmp_key);
	free(v->tmp_key);
	if (!access(v->val, F_OK))
		v->val = ft_strdup(filename_gen(v->val));
	p->val = ft_strdup(v->val);
	open_heredoc_3(v, envr);
	return (v->fd);
}

int	open_heredoc(t_vars *p, t_list *list, t_env **envr)
{
	t_vars	v;

	v.val = NULL;
	v.flag = 1;
	while (list && !ft_strcmp(list->type, "DELIMITER"))
	{
		if (list->content[0] == '\'' || list->content[0] == '\"')
		{
			if (list->content[0] == '\'')
				v.var = ft_strdup("\'");
			else if (list->content[0] == '\"')
				v.var = ft_strdup("\"");
			v.flag = 0;
			v.str = ft_strdup(list->content);
			free(list->content);
			list->content = ft_strtrim(v.str, v.var);
			free(v.str);
			free(v.var);
		}
		v.val = ft_strjoin(v.val, list->content);
		list = list->link;
	}
	return (open_heredoc_2(&v, envr, p));
}

void	if_heredoce(t_vars *v, t_cmd *tmp, t_env **envr, t_vars *p)
{
	v->flag = 230;
	if (tmp->file_name)
	{
		if (tmp->fd_in >= 3)
			close(tmp->fd_in);
		unlink(tmp->file_name);
		free(tmp->file_name);
		tmp->file_name = NULL;
	}
	v->tmp_value = ft_strdup("<<");
	if (v->tmp1->link && !ft_strcmp(v->tmp1->link->type, "space"))
		v->tmp1 = v->tmp1->link->link;
	else if (v->tmp1->link)
		v->tmp1 = v->tmp1->link;
	open_heredoc(p, v->tmp1, envr);
	v->fd = p->fd;
	v->command = ft_strdup(p->val);
	free(p->val);
}
