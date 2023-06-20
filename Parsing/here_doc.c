#include "../minishell.h"

char	*expand_in_here_doc(char *input, t_env **envr, int istrue)
{
	t_vars	v;

	v.str = NULL;
	v.tmp1 = ft_split_input(input);
	lexer(&v.tmp1);
	if (istrue)
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
		if (v->str)
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
	v.flag = 1;
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
			v.flag = 0;
			// free(list->content);
			list->content = ft_strtrim(list->content, v.var);
			free(v.var);
		}
		v.val = ft_strjoin(v.val, list->content);
		list = list->link;
	}
	open_heredoc_2(&v, envr, p);
	p->fd = v.fd;
}

void	if_heredoce(t_vars *v, t_cmd *tmp, t_env **envr, t_vars *p)
{
	if (tmp->file_name)
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
